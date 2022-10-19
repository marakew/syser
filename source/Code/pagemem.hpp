
#ifndef _PAGEMEM_HPP_
#define _PAGEMEM_HPP_

#include "define.h"
#include "alt/altmap.hpp"
#include "pageimage.hpp"

#define PAGE_SIZE 4096

//for CPolyFS

struct MEMORY_PAGE
{
	unsigned char *PhysAddr;
	unsigned long Attribute;
	unsigned long AccessReadCount;
	unsigned long AccessWriteCount;
	unsigned long AccessExecuteCount;
	unsigned long AccessLoaderWriteCount;
	unsigned long PageData;
};

struct MEMORY_PAGE_CACHE
{
	MEMORY_PAGE *MemoryPage;
	unsigned long BaseAddr;
	unsigned long AccessCount;
	bool bExist;
};

struct CPageMemory
{
public:
	CPageMemory();
	virtual ~CPageMemory();
 
	MEMORY_PAGE_CACHE m_PageDataCache1;
	MEMORY_PAGE_CACHE m_PageDataCache2;
	int m_PageUsedCount;
	int m_PageCount;
	unsigned char *m_PhysMemory;
	unsigned char *m_PageExistMap;
	MEMORY_PAGE *m_PageDir[1024];
	int m_WriteAccessPageCount;
	int m_FreePhysIndex;
	unsigned long m_Style;
	map<CMemoryImage, CPageImage*> m_ImageList;

	virtual bool Init(int PageCount, unsigned long Style);
	virtual void Release();
	virtual void RefreshCache(MEMORY_PAGE *pPage);
	virtual bool LoadPage(unsigned long Address, MEMORY_PAGE *pPage);
	virtual bool SavePage(unsigned long Address, MEMORY_PAGE *pPage);
	virtual bool ExchangePage();
	virtual int FlushAllPage();
	virtual bool MapImage(CPageImage *pImageFile);
	virtual bool UnmapImage(CPageImage *pImageFile);

	MEMORY_PAGE *GetDataPage(unsigned long Address, bool bWrite);

	CPageImage *GetImage(unsigned long Address);
	CPageImage *GetImage(const char *FileName);
	bool AllocatePage(unsigned long Address, unsigned long Attr, void *Buffer, unsigned long Size);
	bool AllocatePage(unsigned long Address, unsigned long Attr, unsigned char Data);
	bool ReleasePage(unsigned long Address);
	unsigned long ReleaseClearPage();
	bool IsPageExist(unsigned long Address);
	bool IsPageDirExist(unsigned long Address);

	static unsigned long ReadMemory(unsigned long Address, void *Buffer, unsigned long Size, CPageMemory *pPageMemory);
	static unsigned long WriteMemory(unsigned long Address, const void *Buffer, unsigned long Size, CPageMemory *pPageMemory);

	unsigned long ReadString(unsigned long Address, char *Buffer, unsigned long Size);
	unsigned long WriteString(unsigned long Address, const char *Buffer, unsigned long Size);

	unsigned long ReadString(unsigned long Address, unsigned short *Buffer, unsigned long Size);
	unsigned long WriteString(unsigned long Address, const unsigned short *Buffer, unsigned long Size);

	unsigned long ReadString(unsigned long Address, unsigned long *Buffer, unsigned long Size);
	unsigned long WriteString(unsigned long Address, const unsigned long *Buffer, unsigned long Size);


	unsigned long ReadMemX(unsigned long Address, void *Buffer, unsigned long Size);
	unsigned long WriteMemX(unsigned long Address, const void *Buffer, unsigned long Size);
	unsigned long ZeroMemX(unsigned long Fill, unsigned long size);

	bool ReadMemB(unsigned long Address, void *Buffer);
	bool ReadMemW(unsigned long Address, void *Buffer);
	bool ReadMemD(unsigned long Address, void *Buffer);

	bool WriteMemB(unsigned long Address, const void *Buffer);
	bool WriteMemW(unsigned long Address, const void *Buffer);
	bool WriteMemD(unsigned long Address, const void *Buffer);

	bool NewPage(MEMORY_PAGE *pPage);
	bool DeletePage(MEMORY_PAGE *pPage);
};

#endif
