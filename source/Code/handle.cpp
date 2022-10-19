
#include <string.h>
#include "handle.hpp"

	CHandle::CHandle()
	{
		memset(m_RegisterType, 0, sizeof(m_RegisterType));
		memset(m_HandleInfoDir, 0, sizeof(m_HandleInfoDir));
	}

	ULONG_PTR CHandle::CreateHandle(void *HandleInfo, unsigned long Type)
	{
		for (unsigned long nDir = 0; nDir < 64; ++nDir)
		{
			if (!m_HandleInfoDir[nDir])
			{
				m_HandleInfoDir[nDir] = new HANDLE_INFO[1024];
				memset(m_HandleInfoDir[nDir], 0, sizeof(HANDLE_INFO)*1024);
			}

			for (unsigned long nSlot = 0; nSlot < 1024; ++nSlot)
			{
				if (!m_HandleInfoDir[nDir][nSlot].Attribute)
				{
					HANDLE_INFO *pFreeSlot = &m_HandleInfoDir[nDir][nSlot];
					pFreeSlot->HandleInfo = HandleInfo;
					pFreeSlot->Attribute = (Type << 16) | 1;
					return (Type << 16) | (nDir << 10) | nSlot;
				}
			}
		}
		return 0;
	}

	bool CHandle::CloseHandle(ULONG_PTR Handle)
	{
		unsigned long Type = GetHandleType(Handle);
		unsigned long nSlot = GetHandleSlot(Handle);
		unsigned long nDir = GetHandleDir(Handle);

		if (!m_HandleInfoDir[nDir])
			return false;

		if (!(m_HandleInfoDir[nDir][nSlot].Attribute & 1))
			return false;

		if ((unsigned long)m_HandleInfoDir[nDir][nSlot].Attribute >> 16 != Type)
			return false;

		if (Type >= 256 || !m_RegisterType[Type].bExist)
			return false;

		HANDLE_INFO *pHandleInfo = &m_HandleInfoDir[nDir][nSlot];

		if (m_RegisterType[Type].RelHIFunc)
			m_RegisterType[Type].RelHIFunc(m_RegisterType[Type].Param, pHandleInfo->HandleInfo);
		else delete pHandleInfo->HandleInfo;

		pHandleInfo->HandleInfo = 0;
		pHandleInfo->Attribute = 0;

		for (unsigned long n = 0; n < 1024; ++n)
		{
			if (m_HandleInfoDir[nDir][n].Attribute & 1)
				return true;
		}

		delete []m_HandleInfoDir[nDir];
		m_HandleInfoDir[nDir] = NULL;
		return true;
	}

	void CHandle::CloseAllHandle()
	{
		for (unsigned long nDir = 0; nDir < 64; ++nDir)
		{
			HANDLE_INFO *pInfo = m_HandleInfoDir[nDir];
			if (pInfo)
			{
				for (unsigned long nSlot = 0; (signed int)nSlot < 1024; ++nSlot) //???
				{
					unsigned long Type = (unsigned long)pInfo[nSlot].Attribute >> 16;
					if (Type < 256 && m_RegisterType[Type].bExist && pInfo[nSlot].Attribute & 1)
					{
						if (m_RegisterType[Type].RelHIFunc)
							m_RegisterType[Type].RelHIFunc(m_RegisterType[Type].Param, pInfo[nSlot].HandleInfo);
						else	delete pInfo[nSlot].HandleInfo;
					}
				}
				delete []pInfo;
				m_HandleInfoDir[nDir] = NULL;
			}
		}
	}

	void CHandle::Reset()
	{
		CloseAllHandle();
		memset(m_RegisterType, 0, sizeof(m_RegisterType));
		memset(m_HandleInfoDir, 0, sizeof(m_HandleInfoDir));
	}


	bool CHandle::IsHandleExist(ULONG_PTR Handle) const
	{
		unsigned long Type = GetHandleType(Handle);
		unsigned long nSlot = GetHandleSlot(Handle);
		unsigned long nDir = GetHandleDir(Handle);

		return m_HandleInfoDir[nDir]
			&& m_HandleInfoDir[nDir][nSlot].Attribute & 1
			&& (unsigned long)m_HandleInfoDir[nDir][nSlot].Attribute >> 16 == Type;
	}

	void *CHandle::GetHandleInfo(ULONG_PTR Handle) const
	{
		unsigned long Type = GetHandleType(Handle);
		unsigned long nSlot = GetHandleSlot(Handle);
		unsigned long nDir = GetHandleDir(Handle);

		if (Type >= 256 || !m_RegisterType[Type].bExist)
			return nullptr;

		if (IsHandleExist(Handle))
			return m_HandleInfoDir[nDir][nSlot].HandleInfo;
		return nullptr;
	}

	unsigned long CHandle::GetHandleType(ULONG_PTR Handle) const
	{
		return Handle >> 16;
	}

	unsigned long CHandle::GetHandleSlot(ULONG_PTR Handle) const
	{
		return Handle & 1023;
	}

	unsigned long CHandle::GetHandleDir(ULONG_PTR Handle) const
	{
		return (unsigned short)Handle >> 10;
	}

	ULONG_PTR CHandle::GetFirstHandle(unsigned long Type) const
	{
		if (Type >= 256 || !m_RegisterType[Type].bExist)
			return 0;

		for (unsigned long nDir = 0; nDir < 64; ++nDir)
		{
			if (m_HandleInfoDir[nDir])
			{
				for (unsigned long nSlot = 0; nSlot < 1024; ++nSlot)
				{
					if ( (unsigned long)m_HandleInfoDir[nDir][nSlot].Attribute >> 16 == Type
						&& m_HandleInfoDir[0][0].Attribute & 1)
					{
						return (Type << 16) | (nDir << 10) | nSlot;
					}
				}
			}
		}
		return 0;
	}

	ULONG_PTR CHandle::GetNextHandle(ULONG_PTR Handle) const
	{
		unsigned long Type = GetHandleType(Handle);

		if (Type >= 256 || !m_RegisterType[Type].bExist)
			return 0;

		unsigned long nFirstSlot = GetHandleSlot(Handle) + 1;

		for (unsigned long nDir = GetHandleDir(Handle); nDir < 64; ++nDir)
		{
			if (m_HandleInfoDir[nDir])
			{
				for (unsigned long nSlot = nFirstSlot; nSlot < 1024; ++nSlot)
				{
					if ( (unsigned long)m_HandleInfoDir[nDir][nSlot].Attribute >> 16 == Type
						&& m_HandleInfoDir[GetHandleDir(Handle)][nFirstSlot].Attribute & 1)
					{
						return (Type << 16) | (nDir << 10) | nSlot;
					}
				}
			}
			nFirstSlot = 0;
		}
		return 0;
	}

	unsigned long CHandle::Register(REL_HI_FUNC RelHIFunc, void *Param)
	{
		for (unsigned long Type = 1; Type < 256; ++Type)
			if (!m_RegisterType[Type].bExist)
			{
				m_RegisterType[Type].RelHIFunc = RelHIFunc;
				m_RegisterType[Type].Param = Param;
				m_RegisterType[Type].bExist = true;
				return Type;
			}
		return 0;
	}

	bool CHandle::Unregister(unsigned long Type)
	{
		if (Type >= 256 || !m_RegisterType[Type].bExist)
			return false;

		m_RegisterType[Type].RelHIFunc = 0;
		m_RegisterType[Type].Param = nullptr;
		m_RegisterType[Type].bExist = false;
		return true;
	}

