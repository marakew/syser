
#include "symbol.hpp"

// CSymbolModule

	CSymbolModule::CSymbolModule()
		: m_ImageBase(0)
		, m_ImageSize(0)
		, m_RefCount(0)
		, m_TimeStamp(0)
		, m_CheckSum(0)
	{
	}

	bool CSymbolModule::Add(ULONG_PTR Address, const char *Name) //00
	{
		map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_SymbolMap.insert(
			_Tmap_pair<ULONG_PTR,TAnsiNoCaseString>(Address, TAnsiNoCaseString(Name)));
		return it != m_SymbolMap.end();
	}

	bool CSymbolModule::Set(ULONG_PTR Address, const char *Name) //04
	{
		map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_SymbolMap.find(Address);
		if (it == m_SymbolMap.end())
			return Add(Address, Name);

		if (it->second == Name)
			return false;
		it->second = Name;
		return true;
	}

	int CSymbolModule::Del(const char *Name) //08
	{
		int Count = 0;
		map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_SymbolMap.begin();
		for (; it == m_SymbolMap.end(); )
		{
			if (it->second == Name)
			{
				m_SymbolMap.erase(it++), ++Count;
			} else ++it;
		}
		return Count > 0;
	}

	bool CSymbolModule::Del(ULONG_PTR Address) //0C
	{
		return m_SymbolMap.erase(Address) > 0;
	}

	bool CSymbolModule::GetAddr(const char *Name, ULONG_PTR *pAddress) //10
	{
		if (m_ImageBase == 0)
			return false;
		map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_SymbolMap.begin();
		//for (; it == m_SymbolMap.end(); ++it)
		while (it)
		{
			if (it->second == Name)
			{
				if (pAddress)
					*pAddress = m_ImageBase + it->first;
				return true;
			}
			++it;
		}
		return false;
	}
	
	
//CCommentModule

	CCommentModule::CCommentModule()
		: m_RefCount(0)
		, m_TimeStamp(0)
		, m_CheckSum(0)
	{
	}

	bool CCommentModule::Add(ULONG_PTR Address, const char *Name) //00
	{
		map<ULONG_PTR, TAnsiString>::IT it = m_CommentMap.insert(
			_Tmap_pair<ULONG_PTR, TAnsiString>(Address, TAnsiString(Name)));
		return it != m_CommentMap.end();
	}

	bool CCommentModule::Set(ULONG_PTR Address, const char *Name) //04
	{
		map<ULONG_PTR, TAnsiString>::IT it = m_CommentMap.find(Address);
		if (it == m_CommentMap.end())
			return Add(Address, Name);

		if (it->second == Name)
			return false;
		it->second = Name;
		return true;
	}

	bool CCommentModule::Del(ULONG_PTR Address) //08
	{
		return m_CommentMap.erase(Address) > 0;
	}
	
	unsigned long CCommentModule::Write(ALT::ALTFileStream &File)
	{
		unsigned long TotalLen = 0;
		unsigned long Len;
		Len = m_ModuleName.Write(File);
		TotalLen += Len;
		unsigned long Count = m_CommentMap.size();
		Len = File.Write(&Count, 4);
		TotalLen += Len;
		for (map<ULONG_PTR, TAnsiString>::IT it = m_CommentMap.begin();
			it != m_CommentMap.end(); ++it)
		{
			ULONG_PTR Addr = it->first;
			Len = File.Write(&Addr, sizeof(Addr));
			TotalLen += Len;
			Len = it->second.Write(File);
			TotalLen += Len;
		}
		return TotalLen;
	}

	unsigned long CCommentModule::Read(ALT::ALTFileStream &File)
	{
		unsigned long TotalLen = 0;
		unsigned long Len;
		Len = m_ModuleName.Read(File);
		if (!Len) return 0;
		TotalLen += Len;
		unsigned long Count;
		Len = File.Read(&Count, 4);
		if (Len == 0 || Count > 4096) return 0;
		TotalLen += Len;
		while (Count>0)
		{
			ULONG_PTR Addr;
			TotalLen += File.Read(&Addr, sizeof(Addr));
			map<ULONG_PTR, TAnsiString>::IT it = m_CommentMap.insert(_Tmap_pair<ULONG_PTR, TAnsiString>(Addr, TAnsiString()));
			if (it == m_CommentMap.end()) return 0;
			Len = it->second.Read(File);
			if (Len == 0)
			{
				m_CommentMap.erase(it);
				return 0;
			}
			TotalLen += Len;
			--Count;
		}
		return TotalLen;
	}

// CSymbolContainer

	CSymbolModule *CSymbolContainer::FindModule(unsigned int TimeStamp, unsigned int CheckSum) //00
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		map<unsigned long long, CSymbolModule>::IT it = m_SymbolModulesMap.find(key);
		//if (it == m_SymbolModulesMap.end())
		if (!it)
			return NULL;
		return &it->second;
	}

	CSymbolModule *CSymbolContainer::InsertModule(unsigned int TimeStamp, unsigned int CheckSum) //04
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		map<unsigned long long, CSymbolModule>::IT it =
			m_SymbolModulesMap.insert(
				_Tmap_pair<unsigned long long, CSymbolModule>(key, CSymbolModule()));
		if (!it)
			return NULL;
		it->second.m_TimeStamp = TimeStamp;
		it->second.m_CheckSum = CheckSum;
		return &it->second;
	}

	bool CSymbolContainer::RemoveModule(unsigned int TimeStamp, unsigned int CheckSum) //08
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		return m_SymbolModulesMap.erase(key) > 0;
	}

	void CSymbolContainer::Reset() //0C
	{
		map<unsigned long long, CSymbolModule>::IT it = m_SymbolModulesMap.begin();
		//for (; it != m_SymbolModulesMap.end(); )
		while (it)
		{
			if(it->second.m_RefCount > 0)
				++it;
			else
				m_SymbolModulesMap.erase(it++);
		}
	}

	bool CSymbolContainer::GetFullSymbol(ULONG_PTR Address, char *SymbolName, int Limit) //10
	{
		map<unsigned long long, CSymbolModule>::IT it = m_SymbolModulesMap.begin();
		//for (; it != m_SymbolModulesMap.end(); ++it)
		while (it)
		{
			if (it->second.m_ImageBase != 0 &&
				Address >= it->second.m_ImageBase &&
				Address < it->second.m_ImageBase+it->second.m_ImageSize)
			{
				map<ULONG_PTR, TAnsiNoCaseString>::IT si = it->second.m_SymbolMap.find(Address-it->second.m_ImageBase);
				if (si != it->second.m_SymbolMap.end())
				{
					int n = TGetFileTitle(it->second.m_ModuleName.operator const char*(), SymbolName);
					n += TStrCpy(SymbolName+n, "_");
					TStrCpyLimit(SymbolName+n, si->second.operator const char*(), Limit-n);
					return true;
				}
			}
			++it;
		}
		return false;
	}

	const char *CSymbolContainer::GetSymbol(ULONG_PTR Address) //14
	{
		map<unsigned long long, CSymbolModule>::IT it = m_SymbolModulesMap.begin();
		//for (; it != m_SymbolModulesMap.end(); ++it)
		while (it)
		{
			if (it->second.m_ImageBase != 0 &&
				Address >= it->second.m_ImageBase &&
				Address < it->second.m_ImageBase+it->second.m_ImageSize)
			{
				map<ULONG_PTR, TAnsiNoCaseString>::IT si = it->second.m_SymbolMap.find(Address-it->second.m_ImageBase);
				if (si != it->second.m_SymbolMap.end())
				{
					return si->second.operator const char*();
				}
			}
			++it;
		}
		return NULL;
	}
	

// CCommentContainer

	CCommentModule *CCommentContainer::FindModule(unsigned int TimeStamp, unsigned int CheckSum) //00 TimeStamp,CheckSum
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		map<unsigned long long, CCommentModule>::IT it = m_CommentModulesMap.find(key);
		//if (it == m_CommentModulesMap.end())
		if (!it)
			return NULL;
		return &it->second;
	}

	CCommentModule *CCommentContainer::InsertModule(unsigned int TimeStamp, unsigned int CheckSum) //04
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		map<unsigned long long, CCommentModule>::IT it =
			m_CommentModulesMap.insert(
				_Tmap_pair<unsigned long long, CCommentModule>(key, CCommentModule()));
		if (!it)
			return NULL;
		it->second.m_TimeStamp = TimeStamp;
		it->second.m_CheckSum = CheckSum;
		return &it->second;
	}

	bool CCommentContainer::RemoveModule(unsigned int TimeStamp, unsigned int CheckSum) //08
	{
		unsigned long long key = ((unsigned long long)CheckSum)<<32|TimeStamp;
		return m_CommentModulesMap.erase(key) > 0;
	}

	void CCommentContainer::Reset() //0C
	{
		map<unsigned long long, CCommentModule>::IT it = m_CommentModulesMap.begin();
		//for (; it != m_CommentModulesMap.end(); )
		while (it)
		{
			if(it->second.m_RefCount > 0)
				++it;
			else
				m_CommentModulesMap.erase(it++);
		}
	}
	
	unsigned long CCommentContainer::Write(ALT::ALTFileStream &File)
	{
		unsigned long TotalLen = 0;
		unsigned long Len;
		unsigned long SIG = 0x1000;
		Len = File.Write(&SIG, 4);
		TotalLen += Len;
		unsigned long Count = m_CommentModulesMap.size();
		Len = File.Write(&Count, 4);
		TotalLen += Len;
		for (map<unsigned long long, CCommentModule>::IT it = m_CommentModulesMap.begin();
			it != m_CommentModulesMap.end(); ++it)
		{
			Len = File.Write(&it->first, sizeof(it->first));
			TotalLen += Len;
			Len = it->second.Write(File);
			TotalLen += Len;
		}
		return TotalLen;
	}

	unsigned long CCommentContainer::Read(ALT::ALTFileStream &File)
	{
		unsigned long TotalLen = 0;
		unsigned long Len;
		unsigned long SIG;
		Len = File.Read(&SIG, 4);
		if (Len != 4 || SIG != 0x1000) return 0;
		TotalLen += Len;
		unsigned long Count;
		Len = File.Read(&Count, 4);
		if (Len == 0 || Count > 0x10000) return 0;
		TotalLen += Len;
		//map<unsigned long long, CCommentModule>::IT it;
		while (Count>0)
		{
			unsigned long long Key;
			Len = File.Read(&Key, sizeof(Key));
			if (Len == 0) return 0;
			TotalLen += Len;
			map<unsigned long long, CCommentModule>::IT it = m_CommentModulesMap.insert(_Tmap_pair<unsigned long long, CCommentModule>(Key, CCommentModule()));
			if (it == m_CommentModulesMap.end()) return 0;
			it->second.m_TimeStamp = Key;
			it->second.m_CheckSum = Key>>32;
			Len = it->second.Read(File);
			if (Len == 0)
			{
				m_CommentModulesMap.erase(it);
				return 0;
			}
			TotalLen += Len;
			--Count;
		}
		return TotalLen;
	}

//CDbgModule

	CDbgModule::CDbgModule()
		: m_pSymbolModule(nullptr)
		, m_pCommentModule(nullptr)
		, m_pName(nullptr)
		, m_ImageBase(0)
		, m_ImageSize(0)
		, m_TimeStamp(0)
		, m_CheckSum(0)
		, m_LoadPDB(true)
	{
	}

	bool CDbgModule::GetSymbolAddress(const char *szName, ULONG_PTR *pAddress) //00
	{
		if (!m_pSymbolModule)
			return false;

		for (map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_pSymbolModule->m_SymbolMap.begin();
			it != m_pSymbolModule->m_SymbolMap.end(); ++it)
		{
			if (it->second == szName)
			{
				if (pAddress)
					*pAddress = it->first;
				return true;
			}
		}
		return false;
	}

	bool CDbgModule::GetSymboBaseAddr(const char *szName, ULONG_PTR *pAddress) //04
	{
		if (!GetSymbolAddress(szName, pAddress))
			return false;
		if (pAddress)
			*pAddress += m_ImageBase;
		return true;
	}

	bool CDbgModule::GetFullSymbol(ULONG_PTR Address, char *szName, unsigned int Len) //08
	{
		if (!m_pSymbolModule)
			return false;
		map<ULONG_PTR, TAnsiNoCaseString>::IT it =
			m_pSymbolModule->m_SymbolMap.find(Address-m_ImageBase);
		if (it == m_pSymbolModule->m_SymbolMap.end())
			return false;

		int n = TStrCpyLimit(szName, m_FileTitle.operator const char*(), Len);
		char *nextStr = szName+n;
		int nextLen = Len-n;
		if (nextLen <= 0)
			return false;
		if (m_FileTitle.m_nLength)
		{
			n = TStrCpyLimit(nextStr, "_", nextLen);
			nextStr += n;
			nextLen -= n;
			if (nextLen <= 0)
				return false;
		}
		TStrCpyLimit(nextStr, it->second.operator const char*(), nextLen);
		return true;
	}

	const char *CDbgModule::GetSymbol(ULONG_PTR Address)
	{
		if (!m_pSymbolModule)
			return NULL;
		map<ULONG_PTR, TAnsiNoCaseString>::IT it =
			m_pSymbolModule->m_SymbolMap.find(Address-m_ImageBase);
		if (it == m_pSymbolModule->m_SymbolMap.end())
			return NULL;
		return it->second;
	}

	const char *CDbgModule::GetComment(ULONG_PTR Address)
	{
		if (!m_pCommentModule)
			return NULL;
		map<ULONG_PTR, TAnsiString>::IT it =
			m_pCommentModule->m_CommentMap.find(Address-m_ImageBase);
		if (it == m_pCommentModule->m_CommentMap.end())
			return NULL;
		return it->second;
	}

	bool CDbgModule::GetSymbolBase(ULONG_PTR *pAddress) //14
	{
		if (!m_pSymbolModule)
			return false;
		map<ULONG_PTR, TAnsiNoCaseString>::IT it = m_pSymbolModule->m_SymbolMap.find_near(*pAddress-m_ImageBase);
		if (it == m_pSymbolModule->m_SymbolMap.end())
			return false;
		*pAddress = m_ImageBase + it->first; //???
		return true;
	}

	void CDbgModule::SetSymbolModule(CSymbolModule *pSymbolModule) //18
	{
		m_pSymbolModule = pSymbolModule;
		if (m_pSymbolModule)
			++m_pSymbolModule->m_RefCount;
	}

	void CDbgModule::RemoveSymbolModule(CSymbolContainer *pSymbolContainer) //1C
	{
		if (m_pSymbolModule)
		{
			--m_pSymbolModule->m_RefCount;
			if (pSymbolContainer)
			{
				if (m_pSymbolModule->m_RefCount <= 0)
					pSymbolContainer->RemoveModule(
						m_pSymbolModule->m_TimeStamp,
						m_pSymbolModule->m_CheckSum);
			}
			m_pSymbolModule = NULL;
		}
	}

	void CDbgModule::SetCommentModule(CCommentModule *pCommentModule) //20
	{
		m_pCommentModule = pCommentModule;
		if (m_pCommentModule)
			++m_pCommentModule->m_RefCount;
	}

	void CDbgModule::RemoveCommentModule(CCommentContainer *pCommentContainer) //24
	{
		if (m_pCommentModule)
		{
			--m_pCommentModule->m_RefCount;
			if (pCommentContainer)
			{
				if (m_pCommentModule->m_RefCount <= 0)
					pCommentContainer->RemoveModule(
						m_pCommentModule->m_TimeStamp,
						m_pCommentModule->m_CheckSum);
			}
			m_pCommentModule = NULL;
		}
	}
	

// CDbgModuleList

	CDbgModule *CDbgModuleList::FindModule(ULONG_PTR Address) //00
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(Address));
		if (it == m_DbgModuleMap.end())
			return NULL;
		return &it->second;
	}

	CDbgModule *CDbgModuleList::FindModule(const char *szName) //04
	{
		for (map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.begin();
			it != m_DbgModuleMap.end(); ++it)
		{
			if (TCmpModuleName(it->second.m_FullFileName.operator const char*(), szName))
				return &it->second;
		}

		char *pStr = TStrRChr(szName, '\\');
		if (!pStr) return NULL;

		for (map<CMemoryImage, CDbgModule>::IT its = m_DbgModuleMap.begin();
			its != m_DbgModuleMap.end(); ++its)
		{
			if (TCmpModuleName(its->second.m_FullFileName.operator const char*(), pStr))
				return &its->second;
		}
		return NULL;
	}

	CDbgModule *CDbgModuleList::FindModule(unsigned long TimeStamp, unsigned long CheckSum) //08
	{
		for (map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.begin();
			it != m_DbgModuleMap.end(); ++it)
		{
			if (it->second.m_TimeStamp == TimeStamp &&
			    it->second.m_CheckSum == CheckSum)
				return &it->second;
		}
		return NULL;
	}

	CDbgModule *CDbgModuleList::InsertModule(const char *szModule, ULONG_PTR Address, unsigned long Size, unsigned long TimeStamp, unsigned long CheckSum)
	{
		map<CMemoryImage, CDbgModule>::IT it =
				m_DbgModuleMap.insert(_Tmap_pair<CMemoryImage, CDbgModule>(
					CMemoryImage(Address, Address+Size-1), CDbgModule()));
		if (it == m_DbgModuleMap.end())
			return NULL;
		it->second.m_ImageBase = Address;
		it->second.m_ImageSize = Size;
		it->second.m_FullFileName = szModule;
		it->second.m_TimeStamp = TimeStamp;
		it->second.m_CheckSum = CheckSum;
		char FileTitle[268]; //TODO 260 MAX_FN_LEN
		TGetFileTitle(szModule, FileTitle);
		it->second.m_FileTitle = FileTitle;
		it->second.m_pName = TGetFileName(it->second.m_FullFileName.operator const char*()); //???
		return &it->second;
	}

	bool CDbgModuleList::RemoveModule(ULONG_PTR Address) //10
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(Address));
		if (it == m_DbgModuleMap.end())
			return false;
		if (it->second.m_pSymbolModule)
			--it->second.m_pSymbolModule->m_RefCount;
		m_DbgModuleMap.erase(it);
		return true;
	}

	bool CDbgModuleList::GetFullSymbol(ULONG_PTR Address, char *szName, int Len) //14
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(Address));
		if (it == m_DbgModuleMap.end())
			return false;
		return it->second.GetFullSymbol(Address, szName, Len);
	}

	const char *CDbgModuleList::GetSymbol(ULONG_PTR Address) //18
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(Address));
		if (it == m_DbgModuleMap.end())
			return NULL;
		return it->second.GetSymbol(Address);
	}

	bool CDbgModuleList::GetSymbolBase(ULONG_PTR *pAddress) //1C
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(*pAddress));
		if (it == m_DbgModuleMap.end())
			return false;
		return it->second.GetSymbolBase(pAddress);
	}

	const char *CDbgModuleList::GetComment(ULONG_PTR Address) //20
	{
		map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleMap.find(CMemoryImage(Address));
		if (it == m_DbgModuleMap.end())
			return NULL;
		return it->second.GetComment(Address);
	}
