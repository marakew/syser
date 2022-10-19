
#include "wispdib.hpp"
#include "wispbase.hpp"

	CWispDIB::CWispDIB()
		{
			m_PixelBuf.VideoBuffer = 0;
			m_Pal = 0;
			m_bAutoDelete = 0;
			m_RefCount = 0;
			m_PixelBuf.Height = 0;
			m_PixelBuf.Width = 0;
		}

	CWispDIB::~CWispDIB()
		{
			if (!m_bAutoDelete)
				Destroy();
		}

	void CWispDIB::CreatePaint()
		{
			m_PixelBuf.LineDistance = 4 * m_PixelBuf.Width;
			m_PixelBuf.VideoBufferSize = 4 * m_PixelBuf.Width * m_PixelBuf.Height;
			m_PixelBuf.VideoBuffer = new unsigned char[m_PixelBuf.VideoBufferSize];
			m_PaintRect.y = 0;
			m_PaintRect.x = 0;
			m_PixelBuf.Bpp = 32;
			m_PaintRect.cx = m_PixelBuf.Width;
			m_PaintRect.cy = m_PixelBuf.Height;
			m_PixelBuf.pWispDrawHAL = &gpCurWisp->m_Draw32Bit;
		}

	void CWispDIB::FillColor(unsigned long Color)
		{
			//TMemSet(&m_PixelBuf.VideoBuffer[1], Color, m_PixelBuf.VideoBufferSize / (m_PixelBuf.Bpp/8));

			int Len = m_PixelBuf.VideoBufferSize / (m_PixelBuf.Bpp/8);
			while (Len>0)
			{
				((DWORD*)m_PixelBuf.VideoBuffer)[Len] = Color;
				--Len;
			}
		}

	bool CWispDIB::Create(int Width, int Height, unsigned int Type, WISP_POINT *pHotPT, unsigned long ColorKey)
		{
			m_PixelBuf.Width = Width;
			m_PixelBuf.Height = Height;
			m_ColorKey = ColorKey;
			m_Type = Type;
			if (pHotPT)
			{
				m_HotPT.y = pHotPT->x;
				m_HotPT.x = pHotPT->y;

			} else
			{
				m_HotPT.y = 0;
				m_HotPT.x = 0;
			}

			CreatePaint();
			return true;
		}

	void CWispDIB::SetType(unsigned int Type)
		{
			m_Type = Type;
		}

	void CWispDIB::SetColorKey(unsigned long ColorKey)
		{
			if (m_Type != 2) //COLORREF_PF_555
				m_Type = 2; //COLORREF_PF_555
			m_ColorKey = ColorKey;
		}

	void CWispDIB::SetHotPoint(WISP_POINT *pHotPT)
		{
			m_HotPT.x = pHotPT->x;
			m_HotPT.y = pHotPT->y;
		}

	void CWispDIB::SetHotPoint(int x, int y)
		{
			m_HotPT.x = x;
			m_HotPT.y = y;
		}

	bool CWispDIB::GetPixel(int PosX, int PosY, unsigned long *pPixel)
		{
			if (PosX < 0 || PosY < 0)
				return false;

			if (PosX >= m_PixelBuf.Width || PosY >= m_PixelBuf.Height)
				return false;

			*pPixel = *(unsigned long*)((unsigned char*)m_PixelBuf.VideoBuffer+PosX*(32/8) + PosY*m_PixelBuf.LineDistance);
			return true;
		}

	void CWispDIB::Blt(const WISP_RECT & SrcRect, CWispDIB *pDesDIB, int x, int y)
		{
			if (!m_PixelBuf.VideoBuffer)
				return;
			
			WISP_RECT DrawRC;
			WISP_RECT ClipRC;

			ClipRC.x = 0;
			ClipRC.y = 0;
			ClipRC.cx = pDesDIB->m_PixelBuf.Width;
			ClipRC.cy = pDesDIB->m_PixelBuf.Height;

			DrawRC.x = x;
			DrawRC.y = y;
			DrawRC.cx = SrcRect.cx;
			DrawRC.cy = SrcRect.cy;

			if (!ClipRect(&DrawRC, &ClipRC))
				return;
			
			for (int dy = 0; dy < DrawRC.cy; ++dy)
			{
				for (int dx = 0; dx < DrawRC.cx; ++dx)
				{
	((DWORD*)pDesDIB->m_PixelBuf.VideoBuffer)[ x+dx + (y+dy)*pDesDIB->m_PixelBuf.Width ] =
	((DWORD*)m_PixelBuf.VideoBuffer)[ SrcRect.x+dx + (SrcRect.y+dy)*m_PixelBuf.Width ];
				}
			}
			
		}

#pragma pack(push,1)
struct BITMAP_FILE_HEADER
{
  /* 0x0000 */ unsigned short bfType;	//0x4D42
  /* 0x0002 */ unsigned short bfSizeLow;
  /* 0x0004 */ unsigned short bfSizeHigh;
  /* 0x0006 */ unsigned short bfReserved1;
  /* 0x0008 */ unsigned short bfReserved2;
  /* 0x000a */ unsigned short bfOffBitsLow;
  /* 0x000c */ unsigned short bfOffBitsHigh;
}; /* size: 0x000e */

struct BITMAP_INFO_HEADER
{
  /* 0x0000 */ unsigned long biSize;
  /* 0x0004 */ long biWidth;
  /* 0x0008 */ long biHeight;
  /* 0x000c */ unsigned short biPlanes;
  /* 0x000e */ unsigned short biBitCount;
  /* 0x0010 */ unsigned long biCompression;
  /* 0x0014 */ unsigned long biSizeImage;
  /* 0x0018 */ long biXPelsPerMeter;
  /* 0x001c */ long biYPelsPerMeter;
  /* 0x0020 */ unsigned long biClrUsed;
  /* 0x0024 */ unsigned long biClrImportant;
}; /* size: 0x0028 */
#pragma pack(pop)

	bool CWispDIB::Load(const char *FileName, int Index)
		{
			CImageFile BmpFile;

			BmpFile.ChangeFileIO(&gpCurWisp->m_PFSFileIO);

			if (gpCurWisp->m_PFSFileIO.GetFileAttr(FileName) & 0x20) //PFS_ARCHIVE
			{
				unsigned long Size = gpCurWisp->m_PFSFileIO.GetFileSize(FileName);
				if (Size == 0 || Size == -1)
					return false;

				if (!BmpFile.Create(Size, 0, 0))
					return false;

				gpCurWisp->m_PFSFileIO.ReadFromFile(FileName, BmpFile.m_Buffer, Size);
			} else
			if (!BmpFile.Open(FileName, 0))
				return false;

			m_Pal = 0;

			BITMAP_FILE_HEADER bmFileHead;
			BmpFile.ReadFile(0, &bmFileHead, sizeof(bmFileHead));
			if (bmFileHead.bfType != 0x4D42)
			{
				BmpFile.Close();
				return LoadIcon(FileName, Index);
			}

			int dwSize = MAKE_DWORD(bmFileHead.bfSizeHigh,bmFileHead.bfSizeLow) - sizeof(bmFileHead);
			BITMAP_INFO_HEADER *pbmInfo = (BITMAP_INFO_HEADER *)new unsigned char[dwSize+4];

			if (!BmpFile.ReadFile(pbmInfo, dwSize) ||
				pbmInfo->biCompression || 
				pbmInfo->biBitCount == 16 ||
				pbmInfo->biSize != sizeof(BITMAP_INFO_HEADER) )
			{
				BmpFile.Close();
				delete [](unsigned char*)pbmInfo;
				return false;
			}

			m_ColorKey = 0;
			m_HotPT.y = 0;
			m_HotPT.x = 0;
			m_PixelBuf.Width = pbmInfo->biWidth;
			m_PixelBuf.Height = ABS(pbmInfo->biHeight);
			m_Type = 1; //BMP COLORREF_PF_PAL

			int Bpp = ALIGNUP(pbmInfo->biBitCount, 8);
			int PicW = ALIGN_SIZE(Bpp * m_PixelBuf.Width, 4);
			int PicH = m_PixelBuf.Height;

			CreatePaint();

			if (pbmInfo->biBitCount == 8)
			{
				m_Pal = new unsigned long[256];
				for (int i = 0; i < 256; ++i)
					m_Pal[i] = _RT_PDWORD(RT_PDWORD(pbmInfo, sizeof(BITMAP_INFO_HEADER)))[i];

				unsigned char *pPixel = (unsigned char *)pbmInfo + MAKE_DWORD(bmFileHead.bfOffBitsHigh,bmFileHead.bfOffBitsLow) + PicW*PicH - sizeof(BITMAP_FILE_HEADER);

				if (gpCurWisp->m_FrameBufferInfo.Bpp == 8)
				{
					int Off = 0;
					for (int h = 0; h < m_PixelBuf.Height; ++h)
					{
						pPixel -= PicW;
						for (int w = 0; w < m_PixelBuf.Width; ++w)
							((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = pPixel[w];
					}
				} else
				{
					int Off = 0;
					for (int h = 0; h < m_PixelBuf.Height; ++h)
					{
						pPixel -= PicW;
						for (int w = 0; w < m_PixelBuf.Width; ++w)
							((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = m_Pal[pPixel[w]];
					}
				}	
			} else
			{
				unsigned char *pPixel = (unsigned char *)pbmInfo + sizeof(BITMAP_INFO_HEADER) + PicW*PicH;
				int Off = 0;
				for (int h = 0; h < m_PixelBuf.Height; ++h)
				{
					pPixel -= PicW;
					unsigned char *pOrgPixel = pPixel;
					for (int w = 0; w < m_PixelBuf.Width; ++w)
					{
						((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = GET_DWORD(pPixel, 0) & 0xffffff;
						pPixel += Bpp;
					}
					pPixel = pOrgPixel;
				}
			}

			BmpFile.Close();
			delete [](unsigned char*)pbmInfo;
			return true;
		}

#pragma pack(push,1)
struct ICON_DIR
{
  /* 0x0000 */ unsigned short idReserved;
  /* 0x0002 */ unsigned short idType;
  /* 0x0004 */ unsigned short idCount;
}; /* size: 0x0006 */

struct ICON_DIR_ENTRY
{
  /* 0x0000 */ unsigned char bWidth;
  /* 0x0001 */ unsigned char bHeight;
  /* 0x0002 */ unsigned char bColorCount;
  /* 0x0003 */ unsigned char bReserved;
  /* 0x0004 */ unsigned short wPlanes;
  /* 0x0006 */ unsigned short wBitCount;
  /* 0x0008 */ unsigned long dwBytesInRes;
  /* 0x000c */ unsigned long dwImageOffset;
}; /* size: 0x0010 */
#pragma pack(pop)
#if 0
	void CWispDIB::LoadIcon(const ICON_DIR_ENTRY & IconDirEntry)
		{
			m_PixelBuf.Width = IconDirEntry.bWidth;
			m_PixelBuf.Height = IconDirEntry.bHeight;
			m_HotPT.y = 0;
			m_HotPT.x = 0;
			m_Type = 2; //ICO COLORREF_PF_555
			m_ColorKey = 0;
			int Bpp = ALIGNUP(IconDirEntry.wBitCount, 8);
			CreatePaint();

			BITMAP_INFO_HEADER *pIcon = (BITMAP_INFO_HEADER *)new unsigned char[IconDirEntry.dwBytesInRes];
			IconFile.ReadFile(IconDirEntry.dwImageOffset, pIcon, IconDirEntry.dwBytesInRes);
			if (Bpp == 0)
				Bpp = ALIGNUP(pIcon->biBitCount, 8);

			int PicW = ALIGN_SIZE(Bpp * m_PixelBuf.Width, 4);
			int PicH = m_PixelBuf.Height;

			if (IconDirEntry.wBitCount == 8)
			{
				m_Pal = new unsigned long[256];
				for (int i = 0; i < 256; ++i)
					m_Pal[i] = _RT_PDWORD(RT_PDWORD(pIcon, sizeof(BITMAP_INFO_HEADER)))[i];

				unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + 256*4 + PicW*PicH;

				if (gpCurWisp->m_FrameBufferInfo.Bpp == 8)
				{
					int Off = 0;
					for (int h = 0; h < m_PixelBuf.Height; ++h)
					{
						pPixel -= PicW;
						for (int w = 0; w < m_PixelBuf.Width; ++w)
							((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = pPixel[w];
					}
				} else
				{
					int Off = 0;
					for (int h = 0; h < m_PixelBuf.Height; ++h)
					{
						pPixel -= PicW;
						for (int w = 0; w < m_PixelBuf.Width; ++w)
							((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = m_Pal[pPixel[w]];
					}
				}	
			} else
			{
				unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + PicW*PicH;
				int Off = 0;
				for (int h = 0; h < m_PixelBuf.Height; ++h)
				{
					pPixel -= PicW;
					unsigned char *pOrgPixel = pPixel;
					for (int w = 0; w < m_PixelBuf.Width; ++w)
					{
						((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = GET_DWORD(pPixel, 0) & 0xffffff;
						pPixel += Bpp;
					}
					pPixel = pOrgPixel;
				}
			}
			delete [](unsigned char*)pIcon;
		}
#endif
	bool CWispDIB::LoadIcon(const char *FileName, int Index)
		{
			CImageFile IconFile;

			IconFile.ChangeFileIO(&gpCurWisp->m_PFSFileIO);

			if (gpCurWisp->m_PFSFileIO.GetFileAttr(FileName) & 0x20) //???
			{
				unsigned long Size = gpCurWisp->m_PFSFileIO.GetFileSize(FileName);
				if (Size == 0 || Size == -1)
					return false;

				if (!IconFile.Create(Size, 0, 0))
					return false;

				gpCurWisp->m_PFSFileIO.ReadFromFile(FileName, IconFile.m_Buffer, Size);
			} else
			if (!IconFile.Open(FileName, 0))
				return false;

			ICON_DIR IconDir;
			IconFile.ReadFile(&IconDir, sizeof(IconDir));

			if (IconDir.idType != 1 || IconDir.idReserved)
			{
				IconFile.Close();
				return false;
			}

			int DirOffset = sizeof(IconDir);

			if (Index < 0)
				Index = IconDir.idCount-1;

			for (int i = 0; i < IconDir.idCount; ++i)
			{
				ICON_DIR_ENTRY IconDirEntry;
				IconFile.ReadFile(DirOffset, &IconDirEntry, sizeof(ICON_DIR_ENTRY));
				if (i == Index)
				{
					//LoadIcon(IconDirEntry);

					m_PixelBuf.Width = IconDirEntry.bWidth;
					m_PixelBuf.Height = IconDirEntry.bHeight;
					m_HotPT.y = 0;
					m_HotPT.x = 0;
					m_Type = 2; //ICO COLORREF_PF_555
					m_ColorKey = 0;
					int Bpp = ALIGNUP(IconDirEntry.wBitCount, 8);
					CreatePaint();

					BITMAP_INFO_HEADER *pIcon = (BITMAP_INFO_HEADER *)new unsigned char[IconDirEntry.dwBytesInRes];
					IconFile.ReadFile(IconDirEntry.dwImageOffset, pIcon, IconDirEntry.dwBytesInRes);
					if (Bpp == 0)
						Bpp = ALIGNUP(pIcon->biBitCount, 8);

					int PicW = ALIGN_SIZE(Bpp * m_PixelBuf.Width, 4);
					int PicH = m_PixelBuf.Height;

					if (IconDirEntry.wBitCount == 8)
					{
						m_Pal = new unsigned long[256];
						for (int i = 0; i < 256; ++i)
							m_Pal[i] = _RT_PDWORD(RT_PDWORD(pIcon, sizeof(BITMAP_INFO_HEADER)))[i];

						unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + 256*4 + PicW*PicH;

						if (gpCurWisp->m_FrameBufferInfo.Bpp == 8)
						{
							int Off = 0;
							for (int h = 0; h < m_PixelBuf.Height; ++h)
							{
								pPixel -= PicW;
								for (int w = 0; w < m_PixelBuf.Width; ++w)
									((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = pPixel[w];
							}
						} else
						{
							int Off = 0;
							for (int h = 0; h < m_PixelBuf.Height; ++h)
							{
								pPixel -= PicW;
								for (int w = 0; w < m_PixelBuf.Width; ++w)
									((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = m_Pal[pPixel[w]];
							}
						}	
					} else
					{
						unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + PicW*PicH;
						int Off = 0;
						for (int h = 0; h < m_PixelBuf.Height; ++h)
						{
							pPixel -= PicW;
							unsigned char *pOrgPixel = pPixel;
							for (int w = 0; w < m_PixelBuf.Width; ++w)
							{
								((DWORD*)m_PixelBuf.VideoBuffer)[Off++] = GET_DWORD(pPixel, 0) & 0xffffff;
								pPixel += Bpp;
							}
							pPixel = pOrgPixel;
						}
					}
					delete [](unsigned char*)pIcon;

					break;
				}
				DirOffset += sizeof(ICON_DIR_ENTRY);
			}

			IconFile.Close();
			return IconDir.idCount>0;
		}

	void CWispDIB::Destroy()
		{
			SafeDeleteAry(m_PixelBuf.VideoBuffer);
			SafeDeleteAry(m_Pal);

			if (m_bAutoDelete)
				delete this;
		}

	bool CWispDIB::Write(const char *FileName)
		{
			if (!m_PixelBuf.VideoBuffer)
				return false;

			BITMAP_INFO_HEADER bmpHeader;

			bmpHeader.biWidth = m_PixelBuf.Width;
			bmpHeader.biHeight = m_PixelBuf.Height;
			bmpHeader.biSize = sizeof(BITMAP_INFO_HEADER);
			bmpHeader.biPlanes = 1;
			bmpHeader.biBitCount = m_PixelBuf.Bpp;

			int dwDibSize = bmpHeader.biHeight * bmpHeader.biWidth * (bmpHeader.biBitCount / 8);

			BITMAP_FILE_HEADER bmpFileHeader;

			bmpFileHeader.bfType = 0x4D42;
			bmpFileHeader.bfOffBitsLow = sizeof(BITMAP_INFO_HEADER)+sizeof(BITMAP_FILE_HEADER); //0x36;
			bmpFileHeader.bfOffBitsHigh = 0;
			bmpFileHeader.bfSizeLow  = LOW_WORD(dwDibSize + bmpFileHeader.bfOffBitsLow);
			bmpFileHeader.bfSizeHigh = HIGH_WORD(dwDibSize + bmpFileHeader.bfOffBitsLow);
			bmpFileHeader.bfReserved2 = 0;
			bmpFileHeader.bfReserved1 = 0;

			bmpHeader.biCompression = 0;
			bmpHeader.biSizeImage = dwDibSize;
			bmpHeader.biXPelsPerMeter = 0;
			bmpHeader.biYPelsPerMeter = 0;
			bmpHeader.biClrUsed = 0;
			bmpHeader.biClrImportant = 0;

			unsigned long *pPic = new unsigned long[bmpHeader.biHeight * bmpHeader.biWidth];
			if (!pPic)
				return false;

			CImageFileStream BmpFile;

			if (!BmpFile.Create(FileName, 0))
			{
				delete []pPic;
				return false;
			}

			BmpFile.Write(&bmpFileHeader, sizeof(BITMAP_FILE_HEADER));
			BmpFile.Write(&bmpHeader, sizeof(BITMAP_INFO_HEADER));

			int PicW = m_PixelBuf.Width;
			int PicH = m_PixelBuf.Height;

			int dbDibSize = dwDibSize/4;

			unsigned long *pPixel = pPic+(PicH*PicW);

			int Off = 0;
			for (int h = 0; h < m_PixelBuf.Height; ++h)
			{
				pPixel -= PicW;
				for (int w = 0; w < m_PixelBuf.Width; ++w)
					pPixel[w] = ((DWORD*)m_PixelBuf.VideoBuffer)[Off++];
			}

			BmpFile.Write(pPic, dbDibSize * 4);
			BmpFile.Close();
			delete []pPic;
			return true;
		}

	void CWispDIB::BltTiled(CWispDIB *pDIB) //???
		{
			//TODO
		}
//

	CWispDIBList::CWispDIBList()
		{
			m_DIBList = 0;
			m_RefCount = 0;
		}
	CWispDIBList::~CWispDIBList()
		{
			Destroy();
		}

	CWispDIB *CWispDIBList::GetDIB(int ID)
		{
			if (ID > m_Count) return nullptr; //ASSERT
			return &m_DIBList[ID]; //???
		}

	bool CWispDIBList::Load(const char *FileName, int TileWidth, int TileHeight, int Count)
		{
			CWispDIB WholeDIB;

			if (!WholeDIB.Load(FileName, -1))
				return false;

			if (!Count)
				Count = WholeDIB.m_PixelBuf.Width * WholeDIB.m_PixelBuf.Height / (TileHeight * TileWidth);

			m_DIBList = new CWispDIB[Count];

			if (WholeDIB.m_PixelBuf.Height > 0 && WholeDIB.m_PixelBuf.Width > 0) //???
			{
				int n = 0;
				for (int y = 0; y < WholeDIB.m_PixelBuf.Height; y += TileHeight)
					for (int x = 0; x < WholeDIB.m_PixelBuf.Width; x += TileWidth)
					{
						if (n >= Count) break;

						m_DIBList[n].Create(TileWidth, TileHeight, 1, 0, 0);
						WISP_RECT Rect;
						Rect.x = x;
						Rect.y = y;
						Rect.cx = TileWidth;
						Rect.cy = TileHeight;
						WholeDIB.Blt(Rect, &m_DIBList[n], 0, 0);
						n++;
					}
			}

			WholeDIB.Destroy();
			m_Count = Count;
		
			return true;
		}	

	bool CWispDIBList::LoadIcon(const char *FileName)
		{
			CImageFile IconFile;

			IconFile.ChangeFileIO(&gpCurWisp->m_PFSFileIO);
			if (!IconFile.Open(FileName, 0))
				return false;

			ICON_DIR IconDir;
			IconFile.ReadFile(&IconDir, sizeof(IconDir));

			if (IconDir.idType != 1 || IconDir.idReserved)
			{
				IconFile.Close();
				return false;
			}

			m_Count = IconDir.idCount;
			m_DIBList = new CWispDIB[m_Count];

			int DirOffset = sizeof(IconDir);

			for (int i = 0; i < m_Count; ++i)
			{
				ICON_DIR_ENTRY IconDirEntry;
				IconFile.ReadFile(DirOffset, &IconDirEntry, sizeof(ICON_DIR_ENTRY));
				//if (i == Index)
				//{

				m_DIBList[i].m_PixelBuf.Width = IconDirEntry.bWidth;
				m_DIBList[i].m_PixelBuf.Height = IconDirEntry.bHeight;
				m_DIBList[i].m_HotPT.y = 0;
				m_DIBList[i].m_HotPT.x = 0;
				m_DIBList[i].m_Type = 2; //ICO COLORREF_PF_555
				m_DIBList[i].m_ColorKey = 0;
				int Bpp = ALIGNUP(IconDirEntry.wBitCount, 8);
				m_DIBList[i].CreatePaint();

				BITMAP_INFO_HEADER *pIcon = (BITMAP_INFO_HEADER *)new unsigned char[IconDirEntry.dwBytesInRes];
				IconFile.ReadFile(IconDirEntry.dwImageOffset, pIcon, IconDirEntry.dwBytesInRes);
				if (Bpp == 0)
					Bpp = ALIGNUP(pIcon->biBitCount, 8);

				int PicW = ALIGN_SIZE(Bpp * m_DIBList[i].m_PixelBuf.Width, 4);
				int PicH = m_DIBList[i].m_PixelBuf.Height;

				if (IconDirEntry.wBitCount == 8)
				{
					m_DIBList[i].m_Pal = new unsigned long[256];
					for (int i = 0; i < 256; ++i)
						m_DIBList[i].m_Pal[i] = _RT_PDWORD(RT_PDWORD(pIcon, sizeof(BITMAP_INFO_HEADER)))[i];
	
					unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + 256*4 + PicW*PicH;

					if (gpCurWisp->m_FrameBufferInfo.Bpp == 8)
					{
						int Off = 0;
						for (int h = 0; h < m_DIBList[i].m_PixelBuf.Height; ++h)
						{
							pPixel -= PicW;
							for (int w = 0; w < m_DIBList[i].m_PixelBuf.Width; ++w)
								((DWORD*)m_DIBList[i].m_PixelBuf.VideoBuffer)[Off++] = pPixel[w];
						}
					} else
					{
						int Off = 0;
						for (int h = 0; h < m_DIBList[i].m_PixelBuf.Height; ++h)
						{
							pPixel -= PicW;
							for (int w = 0; w < m_DIBList[i].m_PixelBuf.Width; ++w)
								((DWORD*)m_DIBList[i].m_PixelBuf.VideoBuffer)[Off++] = m_DIBList[i].m_Pal[pPixel[w]];
						}
					}	
				} else
				{
					unsigned char *pPixel = (unsigned char *)pIcon + sizeof(BITMAP_INFO_HEADER) + PicW*PicH;
					int Off = 0;
					for (int h = 0; h < m_DIBList[i].m_PixelBuf.Height; ++h)
					{
						pPixel -= PicW;
						unsigned char *pOrgPixel = pPixel;
						for (int w = 0; w < m_DIBList[i].m_PixelBuf.Width; ++w)
						{
							((DWORD*)m_DIBList[i].m_PixelBuf.VideoBuffer)[Off++] = GET_DWORD(pPixel, 0) & 0xffffff;
							pPixel += Bpp;
						}
						pPixel = pOrgPixel;
					}
				}
	
				delete [](unsigned char*)pIcon;

				//	break;
				//}
				DirOffset += sizeof(ICON_DIR_ENTRY);
			}
			IconFile.Close();
			return m_Count > 0;
		}

	void CWispDIBList::Destroy()
		{
			SafeDeleteAry(m_DIBList);
		}

	void CWispDIBList::SetType(unsigned int Type)
		{
			for (int i = 0; i < m_Count; ++i)
			{
				m_DIBList[i].SetType(Type);
			}
		}

	void CWispDIBList::SetColorKey(unsigned long ColorKey)
		{
			for (int i = 0; i < m_Count; ++i)
			{
				m_DIBList[i].SetColorKey(ColorKey);
			}
		}

//
	void CWispDIBLib::ReleaseAll()
		{
			m_DIBMap.clear();
			m_DIBListMap.clear();
		}

	void CWispDIBLib::Remove(const char *FileName)
	{
		map<TAnsiString, CWispDIBList>::IT it = m_DIBListMap.find(FileName);
		if (it != m_DIBListMap.end())
		{
			it->second.m_RefCount--;
			if (it->second.m_RefCount < 1)
				m_DIBListMap.erase(it);
		}

	}

	CWispDIBList *CWispDIBLib::GetDIBList(const char *FileName, int TileWidth, int TileHeight, int Count)
		{
			map<TAnsiString, CWispDIBList>::IT it = m_DIBListMap.find(FileName);
			if (it != m_DIBListMap.end())
			{
				it->second.m_RefCount++;
				return &it->second;
			}
	{
			map<TAnsiString, CWispDIBList>::IT it = m_DIBListMap.insert(_Tmap_pair<TAnsiString, CWispDIBList>(FileName, CWispDIBList()));
		//	if (p.second == false) //??? TODO remove
		//	{
		//		p.first->second.m_RefCount++;
		//		return &p.first->second;
		//	}

			if (it->second.Load(FileName, TileWidth, TileHeight, Count))
			{
				it->second.m_RefCount++;
				return &it->second;
			}
			m_DIBListMap.erase(it);
	}
			return nullptr;
		}

	CWispDIBList *CWispDIBLib::LoadDIBList(const char *FileName, int TileWidth, int TileHeight, int Count, unsigned long ColorKey)
		{
			CWispDIBList *pDIBList = GetDIBList(FileName, TileWidth, TileHeight, Count);
			if (!pDIBList)
				return nullptr;
			pDIBList->SetColorKey(ColorKey);
			return pDIBList;
		}


	CWispDIB *CWispDIBLib::GetDIB(const char *FileName, int Index)
		{
			char szFilePath[MAX_FN_LEN];

			TStrCpy(szFilePath, FileName);

			char *pName = TStrRChr(szFilePath, '\\');
			if (!pName)
				pName = szFilePath;

			char *delim = TStrChr(pName, '#');
			if (delim)
			{
				int x, y;
				if (sscanf(delim, "#%d#%d:%d", &x, &y, &Index) != 3)
					return nullptr;
				*delim = 0;
				return LoadDIBList(szFilePath, x, y, 0, 0xFF00FF)->GetDIB(Index);
			}

			delim = TStrRChr(pName, ':');
			if (delim)
			{
				*delim = 0;
				if (!USDecStrToNum(delim+1, &Index))	//???
					return nullptr;
			}

			map<TAnsiString, CWispDIB>::IT it = m_DIBMap.find(FileName);
			if (it != m_DIBMap.end())
			{
				it->second.m_RefCount++;
				return &it->second;
			}
	{
			map<TAnsiString, CWispDIB>::IT it = m_DIBMap.insert(_Tmap_pair<TAnsiString, CWispDIB>(FileName, CWispDIB()));
		//	if (p.second == false) //??? TODO remove
		//	{
		//		p.first->second.m_RefCount++;
		//		return &p.first->second;
		//	}

			if (it->second.Load(szFilePath, Index))
			{
				it->second.m_RefCount++;
				return &it->second;
			}
			m_DIBMap.erase(it);
	}
			return nullptr;
		}

	CWispDIB *CWispDIBLib::LoadDIB(const char *FileName, int Index, unsigned long ColorKey)
		{
			CWispDIB *pDIB = GetDIB(FileName, Index);
			if (!pDIB)
				return nullptr;
			pDIB->SetColorKey(ColorKey);
			return pDIB;
		}

