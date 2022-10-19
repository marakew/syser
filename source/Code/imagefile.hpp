
#ifndef _IMAGEFILE_HPP_
#define _IMAGEFILE_HPP_

#include "define.h"
#include "pageimage.hpp"
#include "fileio.hpp"
#include "alt/altfile.hpp"

#define PAGE_SIZE	4096

#define FILE_OBJ_DESTROY  0x00010000
#define FILE_OBJ_HANDLE   0x00020000
#define FILE_OBJ_FILE     0x00040000

#define FILE_OBJ_COMPRESS 0x01000000
#define FILE_OBJ_BUFFER   0x02000000
#define FILE_OBJ_MEMORY   0x04000000

//0x06000000

struct CImageFile : public CPageImage
{
	CImageFile();

	virtual bool LoadPage(ULONG_PTR Address, void *Buffer) override;
	virtual bool SavePage(ULONG_PTR Address, const void *Buffer) override;
	virtual unsigned long GetPageAttribute(ULONG_PTR Address) override;
	virtual bool Create(unsigned long Size, unsigned char *Buffer, bool bCompress);
	virtual bool Create(const char *FileName, unsigned long Size);
	virtual bool Open(const char *FileName, HANDLE HandleOpen);
	virtual void Close();
	virtual unsigned long ReadImageMemory(ULONG_PTR Address, void *Buffer, unsigned long Size);
	virtual unsigned long WriteImageMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size);
	virtual void RelocLoad(ULONG_PTR BaseAddr);
	virtual void SetRelocRead(unsigned long (*RMFunc)(ULONG_PTR, void*, unsigned long, void*), void *RMObj);
	virtual void SetRelocWrite(unsigned long (*WRFunc)(ULONG_PTR, const void*, unsigned long, void*), void *WRObj);
	virtual bool OutOfBaseRange(ULONG_PTR BaseAddr);
	virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size);
	virtual unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size);
	virtual void XXX() {} //????

	static unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size, void *RMObj);
	static unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size, void *WRObj);

	void InitParam();
	void ChangeFileIO(CFileIO *pFileIO);
	bool ReadFile(void *Buffer, unsigned long Size);
	bool ReadFile(unsigned long Pos, void *Buffer, unsigned long Size);
	bool WriteFile(const void *Buffer, unsigned long Size);
	bool WriteFile(unsigned long Pos, const void *Buffer, unsigned long Size);

	unsigned long ReadString(ULONG_PTR Address, unsigned long long *Buffer, unsigned long MaxLen, unsigned long AlignLen);
	unsigned long ReadString(ULONG_PTR Address, unsigned long *Buffer, unsigned long MaxLen, unsigned long AlignLen);
	unsigned long ReadString(ULONG_PTR Address, unsigned short *Buffer, unsigned long MaxLen, unsigned long AlignLen);
	unsigned long ReadString(ULONG_PTR Address, char *Buffer, unsigned long MaxLen, unsigned long AlignLen);

	unsigned long GetFileLength();
	bool SetFileLength(unsigned long Length);

	bool DeleteMemory(unsigned long Pos, unsigned long Size);
	bool DeleteFile();
#if 0
	CImageFile(const CImageFile & other) { operator=(other); }
	CImageFile & operator=(const CImageFile & other)
	{
		CPageImage::operator=((CPageImage&)other);
		m_hHandle = other.m_hHandle;
		m_ReadMemory = other.m_ReadMemory;
		m_WriteMemory = other.m_WriteMemory;
		m_RMObj = this;
		m_WRObj = this;
		m_EntryPoint = other.m_EntryPoint;
		m_FileType = other.m_FileType;
		m_FileObjType = other.m_FileObjType;
		m_PageAttribute = other.m_PageAttribute;
		m_FileSize = other.m_FileSize;
		m_BaseOffset = other.m_BaseOffset;
		m_FilePointer = other.m_FilePointer;
		m_Buffer = other.m_Buffer;
		m_pFileIO =  other.m_pFileIO;
	};
#endif
	HANDLE m_hHandle;
	unsigned long (*m_ReadMemory)(ULONG_PTR, void*, unsigned long, void*);
	unsigned long (*m_WriteMemory)(ULONG_PTR, const void*, unsigned long, void*);
	void *m_RMObj;
	void *m_WRObj;
	ULONG_PTR m_EntryPoint;
	unsigned long m_FileType;
	unsigned long m_FileObjType;
	unsigned long m_PageAttribute;
	unsigned long m_FileSize;
	unsigned long m_BaseOffset;
	unsigned long m_FilePointer;
	unsigned char *m_Buffer;
	CFileIO *m_pFileIO;
};

struct CImageFileStream : public ALT::ALTFileStream, CImageFile
{
	virtual unsigned long Write(const void *Buffer, unsigned long Size) override;
	virtual unsigned long Read(void *Buffer, unsigned long Size) override;
};

#endif

