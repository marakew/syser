
#include "wispfont.hpp"
#include "../Code/imagefile.hpp"
#include "../Code/txtfile.hpp"
#include "../Code/sysdep.hpp"
#include "wispbase.hpp"

	CWispFont::CWispFont()
	{
		m_WideHeight = 0;
		m_WideWidth = 0;
		m_WideBufSize = 0;
		m_WideBuffer = 0;

		m_Height = 0;
		m_Width = 0;
		m_BufSize = 0;
		m_Buffer = 0;
	}

	CWispFont::~CWispFont() { Destroy(); }

	void MirrorBitmap(unsigned char *Buffer, unsigned int width, unsigned int height)
	{
		unsigned long sizebuf = (width*height+7)/8;
		unsigned char *buffer = new unsigned char[sizebuf];

		memcpy(buffer, Buffer, sizebuf);

		unsigned int bitoffset = 0;
		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{
				if (GET_BIT(buffer, bitoffset+width-1-w))
					SET_BIT(Buffer, bitoffset+w);
				else	CLR_BIT(Buffer, bitoffset+w);
			}
			bitoffset += width;
		}

		delete []buffer;
	}

	bool CWispFont::LoadBDF(const char *FileName)
	{
		CTXTFile File;

		m_Buffer = 0;
		m_BufSize = 0;
		m_Width = 0;
		m_Height = 0;

		if (!File.Open(FileName, 0))
			return false;

		unsigned int codepoint = 0;
		unsigned int w = 0;
		unsigned int h = 0;
		unsigned int cx = 0;
		unsigned int cy = 0;

		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int xoff = 0;
		unsigned int yoff = 0;

		int Start = 0;
		int Bitmap = 0;
		int off = 0;
		unsigned int bitoffset = 0;
		unsigned long long QWord = 0;

		TListIter<char*> It = File.m_StrList.Begin();
		while (It != File.m_StrList.End())
		{
			//STARTFONT version

			if (::sscanf(*It, "FONTBOUNDINGBOX %d %d %d %d", &w, &h, &cx, &cy) == 4)
			{
				m_Width = w;
				m_Height = h;
				m_BytesPerChar = (m_Width * m_Height + 7) / 8;
				m_BufSize = m_BytesPerChar * 256;
				if (!m_BufSize) break;
				m_Buffer = new unsigned char[m_BufSize];
				if (!m_Buffer) break;
				memset(m_Buffer, 0, m_BufSize);
			} else
			
			//FONT name
			//CHARS n

			//STARTCHAR name
			if (::sscanf(*It, "ENCODING %ud", &codepoint) == 1)
			{

			} else
			if (::sscanf(*It, "BBX %d %d %d %d", &width, &height, &xoff, &yoff) == 4)
			{
				if (width == m_Width && height == m_Height)
				{
					if (codepoint >= 0 && codepoint < 256)
						Start = 1;
				}
			} else
			if (!TStrNCmp(*It, "BITMAP", 6))
			{
				Bitmap = 1;
				off = 0;
			} else
			if (!TStrNCmp(*It, "ENDCHAR", 7))
			{
				if (Bitmap == 1 && Start == 1)
				{
					MirrorBitmap(&m_Buffer[m_BytesPerChar*codepoint], width, height);
				}
				Start = 0;
				Bitmap = 0;
				bitoffset = 0;
			} else
			if (Bitmap == 1 && Start == 1 && off>=0 && off<height && ::sscanf(*It, "%llx", &QWord) == 1)
			{
				QWord >>= TStrLen(*It)*4-width;

				unsigned char *buffer = &m_Buffer[m_BytesPerChar*codepoint];

				for (int w = 0; w < width; ++w)
				{
					if (QWord&1)
					{
						SET_BIT(buffer, w+bitoffset);
					} else
					{
						CLR_BIT(buffer, w+bitoffset);
					}
					QWord>>=1;
				}
				++off;
				bitoffset += width;
			}

			++It;
		}
		File.Close();

		if (!m_BufSize)
			return false;
		if (!m_Buffer)
			return false;

		TMemSet(m_CharLenAry, m_Width, 256); //???

		//m_BytesPerChar = (m_Width * m_Height + 7) / 8;
		m_CharLenAry[10] = 0;
		m_CharLenAry[13] = 0;
		m_CharLenAry[0] = 0;

		return true;
	}

	bool CWispFont::LoadChar(WISP_PHYS_FONT_INFO *pPhysFont)
	{
		CImageFile ImageFile;

		ImageFile.ChangeFileIO(&gpCurWisp->m_PFSFileIO);

		m_Buffer = 0;
		m_BufSize = 0;
		m_Width = 0;
		m_Height = 0;

	//	char szModuleName[MAX_FN_LEN];
	//	GetModulePath(szModuleName, true);
	//	TStrCat(szModuleName, pPhysFont->FontFN);
	//	if (!ImageFile.Open(szModuleName, 0))

		if (!ImageFile.Open(pPhysFont->FontFN, 0))
			return false;

		m_BufSize = ImageFile.GetFileLength();
		if (!m_BufSize)
		{
			ImageFile.Close();
			return false;
		}

		m_Buffer = new unsigned char[m_BufSize];
		if (!m_Buffer)
		{
			m_BufSize = 0;
			ImageFile.Close();
			return false;
		}

		if (!ImageFile.ReadFile(0, m_Buffer, m_BufSize))
		{
			m_BufSize = 0;
			SafeDeleteAry(m_Buffer);
			ImageFile.Close();
			return false;
		}

		m_Width = pPhysFont->Width;
		m_Height = pPhysFont->Height;
		ImageFile.Close();

		TMemSet(m_CharLenAry, 8, 256);

		m_BytesPerChar = (m_Width * m_Height + 7) / 8;
		m_CharLenAry[10] = 0;
		m_CharLenAry[13] = 0;
		m_CharLenAry[0] = 0;

		return true;		
	}

	bool CWispFont::LoadWideChar(WISP_PHYS_FONT_INFO *pPhysFont)
	{
		CImageFile ImageFile;

		ImageFile.ChangeFileIO(&gpCurWisp->m_PFSFileIO);

		m_WideBuffer = 0;
		m_WideBufSize = 0;
		m_WideWidth = 0;
		m_WideHeight = 0;

		if (!ImageFile.Open(pPhysFont->WideFontFN, 0))
			return false;

		m_WideBufSize = ImageFile.GetFileLength();
		if (!m_WideBufSize)
		{
			ImageFile.Close();
			return false;
		}

		m_WideBuffer = new unsigned char[m_WideBufSize]; //unsigned short ???
		if (!m_WideBuffer)
		{
			m_WideBufSize = 0;
			ImageFile.Close();
			return false;
		}

		if (!ImageFile.ReadFile(0, m_WideBuffer, m_WideBufSize))
		{
			m_WideBufSize = 0;
			SafeDeleteAry(m_WideBuffer);
			ImageFile.Close();
			return false;
		}

		m_WideWidth = pPhysFont->Width*2;
		m_WideHeight = pPhysFont->Height;
		ImageFile.Close();

		return true;		
	}

	void CWispFont::Destroy()
	{
		SafeDeleteAry(m_Buffer);
		SafeDeleteAry(m_WideBuffer);
	}

	unsigned char *CWispFont::GetCharPixelBuffer(WCHAR Char)
	{
		if (Char < 256) return m_Buffer + m_BytesPerChar * Char;

		int Offset = 2 * m_BytesPerChar * Char;
		if (m_WideBuffer && Offset < m_WideBufSize)
			return m_WideBuffer + Offset;
		return m_Buffer + m_BytesPerChar * 0x3F; //'?'
	}

	int CWispFont::GetCharWidth(WCHAR Char)
	{
		return (Char >= 256 && m_WideBuffer) ? m_WideWidth : m_Width;
	}

	int CWispFont::GetCharHeight(WCHAR Char)
	{
		return (Char >= 256 && m_WideBuffer) ? m_WideHeight : m_Height;
	}
