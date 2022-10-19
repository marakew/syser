
#ifndef _HANDLE_HPP_
#define _HANDLE_HPP_

#include "define.h"

//for CPFSFileIO

//	(Type << 16) | (nDir << 10) | nSlot;

typedef bool (*REL_HI_FUNC)(void *, void *);

struct HANDLE_INFO
{
	void *HandleInfo;
	unsigned long Attribute;
};

struct HANDLE_TYPE_INFO
{
	REL_HI_FUNC RelHIFunc;
	void *Param;
	bool bExist;
};

struct CHandle
{
	HANDLE_INFO *m_HandleInfoDir[64];
	HANDLE_TYPE_INFO m_RegisterType[256];

	CHandle();

	ULONG_PTR CreateHandle(void *HandleInfo, unsigned long Type);
	bool CloseHandle(ULONG_PTR Handle);
	void CloseAllHandle();
	void Reset();
	ULONG_PTR GetFirstHandle(unsigned long Type) const;
	ULONG_PTR GetNextHandle(ULONG_PTR Handle) const;
	bool IsHandleExist(ULONG_PTR Handle) const;
	void *GetHandleInfo(ULONG_PTR Handle) const;
	unsigned long GetHandleType(ULONG_PTR Handle) const;
	unsigned long GetHandleSlot(ULONG_PTR Handle) const;
	unsigned long GetHandleDir(ULONG_PTR Handle) const;

	unsigned long Register(REL_HI_FUNC RelHIFunc, void *Param);
	bool Unregister(unsigned long Type);
};

#endif
