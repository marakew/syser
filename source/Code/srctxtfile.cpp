
#include "srctxtfile.hpp"

	bool CSrcTXTFile::OpenEx(const char *FileName, HANDLE OpenHandle, unsigned long Mode)
	{
		if (!CImageFile::Open(FileName, OpenHandle))
			return false;

		char *Buffer = new char[m_FileSize+1];
		Buffer[m_FileSize] = 0;

		CImageFile::ReadFile(0, Buffer, m_FileSize);
		m_Mode = Mode;
		if (m_FileSize)
		{
			unsigned long Start = 0;
			unsigned long Index = 0;
			while (Index < m_FileSize)
			{
				if (Buffer[Index] == 13 || Buffer[Index] == 10)
				{
					unsigned long Skip = Index+1;
					if (Buffer[Index] == 13 && Buffer[Index+1] == 10)
						Skip = Index+2;
					
					unsigned long Len = ((m_Mode & 1) ? Skip : Index) - Start;
					char *Line = new char[Len+1];
					TStrNCpy(Line, &Buffer[Start], Len);
					Line[Len] = 0;
					m_StrList.Append(&Line);
					if (m_Mode & 2)
					{
						m_Lines.Append(&Start);
					}
					Start = Skip;
					Index = Skip;
				} else
				{
					++Index;
				}
			}

			if (Index != Start)
			{
				unsigned long Len = Index - Start;
				char *Line = new char[Len+1];
				TStrNCpy(Line, &Buffer[Start], Len);
				Line[Len] = 0;
				m_StrList.Append(&Line);
				if (m_Mode & 2)
				{
					m_Lines.Append(&Start);
				}
			}
		}
		delete []Buffer;
		return true;
	}
