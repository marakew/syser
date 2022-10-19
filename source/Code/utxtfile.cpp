
#include "utxtfile.hpp"

	UCTXTFile::UCTXTFile()
	{
	}

	UCTXTFile::~UCTXTFile()
	{
		TListIter<WCHAR*> It = m_StrList.Begin();
		while (It != m_StrList.End())
		{
			delete []*It;
			++It;
		}
		m_StrList.Clear();
	}

	bool UCTXTFile::Open(const char *FileName, HANDLE OpenHandle)
	{
		if (!CImageFile::Open(FileName, OpenHandle))
			return false;

		WCHAR *Buffer = new WCHAR[m_FileSize + 1];
		Buffer[m_FileSize] = 0;
		CImageFile::ReadFile(0, Buffer, m_FileSize);
		if (*(unsigned short*)Buffer == 0xFEFF)
		{
			delete []Buffer;
			Close();
			return false;
		}
		SetLineEndIdentity(Buffer+1, m_FileSize);
		delete []Buffer;
		return true;
	}
	bool UCTXTFile::Save(const char *FileName)
	{
		bool Exist = false;
		if (FileName == m_FileName)
		{
			CImageFile::Close();
			Exist = true;
		}
		int Len = 0;
		{
		TListIter<WCHAR*> It = m_StrList.Begin();
		while (It != m_StrList.End())
		{
			Len += TStrLen(*It);
			++It;
		}
		}
		WCHAR *Buffer = new WCHAR[Len+1];
		Buffer[Len] = 0;
		WCHAR *pStr = Buffer;
		{
		TListIter<WCHAR*> It = m_StrList.Begin();
		while (It != m_StrList.End())
		{
			TStrCpy(pStr, *It);
			TStrCat(pStr, "\r\n");
			pStr += TStrLen(pStr);
			++It;
		}
		}
		m_pFileIO->WriteToFile(FileName, Buffer, Len*2);
		return Exist ? CImageFile::Open(FileName, 0) : true;
	}
	void UCTXTFile::Close()
	{
		CImageFile::Close();
		TListIter<WCHAR*>It = m_StrList.Begin();
		while (It != m_StrList.End())
		{
			delete []*It;
			++It;
		}
		m_StrList.Clear();
	}
	int UCTXTFile::SetLineEndIdentity(const WCHAR *Buffer, unsigned long long Length)
	{
		unsigned int CurLine = 0;
		unsigned int LastLine = 0;
		while (CurLine < Length/2)
		{
			if (Buffer[CurLine] != 13 && Buffer[CurLine] != 10)
			{
				++CurLine;
			} else
			{
				unsigned int Start = CurLine;
				if (Buffer[CurLine] != 13 || Buffer[CurLine + 1] != 10)
					++CurLine;
				else	CurLine = Start + 2;
				WCHAR *NewStr = new WCHAR[Start - LastLine + 1];
				TStrNCpy(NewStr, &Buffer[LastLine], Start - LastLine);
				NewStr[Start - LastLine] = 0;
				m_StrList.Append(&NewStr);
				LastLine = CurLine;
			}
		}

		if (CurLine != LastLine)
		{
			unsigned int Start = CurLine;
			WCHAR *NewStr = new WCHAR[CurLine - LastLine + 1];
			TStrNCpy(NewStr, &Buffer[LastLine], Start - LastLine);
			NewStr[Start - LastLine] = 0;
			m_StrList.Append(&NewStr);
		}
		return m_StrList.Size();
	}
	bool UCTXTFile::Insert(unsigned long Line, unsigned long Length, const WCHAR *String)
	{
		TListIter<WCHAR*> Iter = m_StrList.Begin();
		while (Line)
		{
			if (Iter == m_StrList.End())
				return false;
			--Line;
			++Iter;
		}

		int TotalLen = TStrLen(*Iter);
		if (TotalLen <= Length)
			return false;

		TotalLen += TStrLen(String);
		WCHAR *Buffer = new WCHAR[TotalLen+1];
		WCHAR *pStr = Buffer;
		TStrNCpy(Buffer, *Iter, Length);
		Buffer[Length] = 0;
		pStr += TStrLen(pStr);
		TStrCpy(pStr, String);
		pStr += TStrLen(pStr);
		TStrCpy(pStr, (*Iter)+Length);

		delete []*Iter;

		m_StrList.Remove(Iter);

		int CurLine = 0;
		int LastLine = 0;
		while (CurLine < TotalLen)
		{
			if (Buffer[CurLine] != 13 && Buffer[CurLine] != 10)
			{
				++CurLine;
			} else
			{
				int Start = CurLine;
				if (Buffer[CurLine] != 13 || Buffer[CurLine+1] != 10)
					++CurLine;
				else	CurLine = Start + 2;
				WCHAR *NewStr = new WCHAR[Start-LastLine+1];
				TStrNCpy(NewStr, &Buffer[LastLine], Start-LastLine);
				NewStr[Start-LastLine] = 0;
				m_StrList.InsertAfter(Iter, &NewStr);
				LastLine = CurLine;
			}
		}
		delete []Buffer;
		return true;
	}
	bool UCTXTFile::Replace(unsigned long Line, unsigned long Length, const WCHAR *String, unsigned long NewPos)
	{
		TListIter<WCHAR*> Iter = m_StrList.Begin();
		while (Line)
		{
			if (Iter == m_StrList.End())
				return false;
			--Line;
			++Iter;
		}

		int TotalLen = TStrLen(*Iter);
		if (TotalLen <= NewPos+Length)
			return false;

		TotalLen += TStrLen(String);
		WCHAR *Buffer = new WCHAR[TotalLen+1];
		WCHAR *pStr = Buffer;
		TStrNCpy(Buffer, *Iter, Length);
		Buffer[Length] = 0;
		pStr += TStrLen(pStr);
		TStrCpy(pStr, String);
		pStr += TStrLen(pStr);
		TStrCpy(pStr, (*Iter)+NewPos+Length);

		delete []*Iter;

		m_StrList.Remove(Iter);

		int CurLine = 0;
		int LastLine = 0;
		while (CurLine < TotalLen)
		{
			if (Buffer[CurLine] != 13 && Buffer[CurLine] != 10)
			{
				++CurLine;
			} else
			{
				int Start = CurLine;
				if (Buffer[CurLine] != 13 || Buffer[CurLine+1] != 10)
					++CurLine;
				else	CurLine = Start + 2;
				WCHAR *NewStr = new WCHAR[Start-LastLine+1];
				TStrNCpy(NewStr, &Buffer[LastLine], Start-LastLine);
				NewStr[Start-LastLine] = 0;
				m_StrList.InsertAfter(Iter, &NewStr);
				LastLine = CurLine;
			}
		}
		delete []Buffer;
		return true;
	}
