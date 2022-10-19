
#ifndef _PAGEIMAGE_HPP_
#define _PAGEIMAGE_HPP_

#include "define.h"
#include "tstrmem.hpp"

struct CMemoryImage
{
	ULONG_PTR ImageBase = 0;
	ULONG_PTR ImageHighBase = 0;

	CMemoryImage(ULONG_PTR Address) : ImageBase(Address), ImageHighBase(Address) {}
	CMemoryImage(ULONG_PTR Address, ULONG_PTR HighAddress) : ImageBase(Address), ImageHighBase(HighAddress) {}

	bool InBaseRange(ULONG_PTR Address)
	{
	        return Address >= ImageBase && Address < ImageHighBase;
	}
};

inline bool operator<(const CMemoryImage & l, const CMemoryImage & r)
{
	return (r.ImageBase == r.ImageHighBase) ?
		(l.ImageHighBase < r.ImageBase) : (l.ImageBase < r.ImageBase);
}

#define PAGE_READ 2
#define PAGE_WRITE 4

//0x0001 PAGE_ACCESS PAGE_MODIFY
//0x0002 PAGE_READ
//0x0004 PAGE_WRITE
//0x0008
//0x0010 PAGE_MAPPED
//0x0020 PAGE_DATA PAGE_ALLOCED
//0x0040
//0x0080
//0x1000
//0x2000
//0x4000 PAGE_DIRTY
//0x8000

struct CPageImage
{
	virtual bool LoadPage(ULONG_PTR Address, void *Buffer) = 0;
	virtual bool SavePage(ULONG_PTR Address, const void *Buffer) { return false; }
	virtual unsigned long GetPageAttribute(ULONG_PTR Address) { return 6; } //PAGE_WRITE PAGE_READ

	TAnsiString m_FileName;

	unsigned long m_ImageSize = 0;
	ULONG_PTR m_ImageBase = 0;
	ULONG_PTR m_ImageHighBase = 0;
};

#endif