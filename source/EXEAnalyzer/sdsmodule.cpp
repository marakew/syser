
#include "sdsmodule.hpp"
#include "../Code/imagefile.hpp"

	CSDSModule::CSDSModule()
		: m_LoadOK(false)
		, m_Size(0)
		, m_Handle(0)
		, m_Strings(nullptr)
		, m_Buffer(nullptr)
		, m_pSDSFile(nullptr)
		, m_pDbgModule(nullptr) //+++
		, m_MemSize(0)
		, m_dwProcessId(0)
	{
	}

	CSDSModule::~CSDSModule()
	{
		Close();
	}

	FILESIG CSDSModule::GetFILESIG()
	{
		FILESIG FileSig(0, 0);
		//FileSig.TimeStamp = 0;
		//FileSig.CheckSum = 0;
		if (m_pSDSFile)
		{
			FileSig.TimeStamp = m_pSDSFile->TimeStamp;
			FileSig.CheckSum = m_pSDSFile->CheckSum;
		}
		return FileSig;
	}

	FILESIG CSDSModule::GetFILESIG(const char *FullFileName)
	{
		FILESIG FileSig(0, 0);
		//FileSig.TimeStamp = 0;
		//FileSig.CheckSum = 0;
		HANDLE Handle;
		if (!gpFileIO->OpenFile(FullFileName, &Handle, 0x10000))
			return FileSig;

		SDSFILE SDSFile;
		memset(&SDSFile, 0, sizeof(SDSFile));
		unsigned long Size = gpFileIO->ReadFile(Handle, &SDSFile, SDSFILE_SIZE, 0);
		gpFileIO->CloseHandle(Handle);
		if (Size == SDSFILE_SIZE &&
		   SDSFile.Magic == SDSFILE_MAGIC &&
		  SDSFile.Version == SDSFILE_VERSION)
		{
			FileSig.TimeStamp = SDSFile.TimeStamp;
			FileSig.CheckSum = SDSFile.CheckSum;
		}
		return FileSig;
	}

	void CSDSModule::Dump(SDSFILE *SDSFile)
	{
#if 0
		setbuf(stdout, NULL);
		printf(" Magic %08x \n", SDSFile->Magic);
		printf(" Version %d \n", SDSFile->Version);
		printf(" FileSize %d \n", SDSFile->FileSize);
		printf(" GlobalsLen %d \n", SDSFile->GlobalsLen);
		printf(" HeaderSize %d \n", SDSFile->HeaderSize);
		printf(" Globals %08x \n", SDSFile->Globals); //BASICTYPE** //???
		printf(" FilesLNLen %d \n", SDSFile->FilesLNLen);
		printf(" FilesLN %08x \n", SDSFile->FilesLN); //FilesLN*
		printf(" StringsLen %d \n", SDSFile->StringsLen);
		printf(" Strings %08x \n", SDSFile->Strings); //char**
		printf(" StringsEntry %08x \n", SDSFile->StringsEntry); //char*
		printf(" SrcFileNamesLen %d \n", SDSFile->SrcFileNamesLen);
		printf(" SrcFileNames %08x \n", SDSFile->SrcFileNames); //SrcFileNameID*
		printf(" PublicSymbolsLen %d \n", SDSFile->PublicSymbolsLen);
		printf(" PublicSymbols %08x \n", SDSFile->PublicSymbols); //PUBSYM*
		printf(" CompilandUnits %08x \n", SDSFile->CompilandUnits); // NOT USES
		printf(" CompilandUnitsLen %d \n", SDSFile->CompilandUnitsLen); // NOT USES
		printf(" LocalsLen %d \n", SDSFile->LocalsLen);
		printf(" Locals %08x \n", SDSFile->Locals); //DATATYPE*
		printf(" X1 %d \n", SDSFile->X1);
		printf(" FunctionsLen %d \n", SDSFile->FunctionsLen);
		printf(" FunctionsEntry %08x \n", SDSFile->FunctionsEntry); //FUNCTION*
		printf(" Functions %08x \n", SDSFile->Functions); //FUNCTION**
		printf(" Sources %08x \n", SDSFile->Sources); //??? ULONG_PTR
		printf(" TimeStamp %08x \n", SDSFile->TimeStamp);
		printf(" CheckSum %08x \n", SDSFile->CheckSum);
		printf(" SUB %d \n", SDSFile->SUB);
		printf(" ModuleName %08x \n", SDSFile->ModuleName); //char*
		printf(" ModuleNameLen %d \n", SDSFile->ModuleNameLen);
		printf(" PDBFullFileName %08x \n", SDSFile->PDBFullFileName); //char*
		printf(" PDBFullFileNameLen %d \n", SDSFile->PDBFullFileNameLen);
		printf(" FullFileName %08x \n", SDSFile->FullFileName); //char*
		printf(" FullFileNameLen %d \n", SDSFile->FullFileNameLen);
		printf(" CRCSUM %08x \n", SDSFile->CRCSUM);
		printf(" SourcesCount %d \n", SDSFile->SourcesCount);
		fflush(stdout);
		//unsigned long X[40];
#endif
	}

	bool CSDSModule::Load(const char *FullFileName)
	{
		if (m_LoadOK) return true;

		if (!gpFileIO->OpenFile(FullFileName, &m_Handle, 0x1'0000))
			return false;

		SDSFILE SDSFile;
		memset(&SDSFile, 0, sizeof(SDSFile));
		if (gpFileIO->ReadFile(m_Handle, &SDSFile, SDSFILE_SIZE, 0) != SDSFILE_SIZE)
		{
			gpFileIO->CloseHandle(m_Handle);
			return false;
		}

		if (SDSFile.Version != SDSFILE_VERSION)
		{
			gpFileIO->CloseHandle(m_Handle);
			return false;
		}

		m_Size = gpFileIO->GetFileLength(m_Handle);
		m_Buffer = new unsigned char[m_Size];
		if (!m_Buffer)
		{
			gpFileIO->CloseHandle(m_Handle);
			return false;
		}

		if (gpFileIO->ReadFile(m_Handle, m_Buffer, m_Size, 0) != m_Size)
		{
			gpFileIO->CloseHandle(m_Handle);
			delete []m_Buffer;
			return false;
		}

		if (!CheckCRC())
		{
			gpFileIO->CloseHandle(m_Handle);
			delete []m_Buffer;
			return false;
		}

		m_pSDSFile = (SDSFILE*)m_Buffer;
		Dump(m_pSDSFile);
		Relocate(m_pSDSFile);
		gpFileIO->CloseHandle(m_Handle);
		InitGlobalsNameID();
		m_SDSFileName = FullFileName;
		m_LoadOK = true;
		return true;
	}

	void CSDSModule::Close()
	{
		if (m_LoadOK)
		{
			if (m_Buffer) delete []m_Buffer;
			m_GlobalsNameID.clear();
			m_LoadOK = false;
		}
	}

	bool CSDSModule::CheckCRC()
	{
		SDSFILE *SDSFile = (SDSFILE *)m_Buffer;
		unsigned long long FileCheckSum = SDSFile->CRCSUM;
		unsigned long long CheckSum = 0;
		SDSFile->CRCSUM = 0;
		for (unsigned long n = 0; n < m_Size/4; ++n)
		{
			CheckSum += ((unsigned long*)m_Buffer)[n];
		}
		SDSFile->CRCSUM = FileCheckSum;
		return ((unsigned long)CheckSum) == FileCheckSum;
	}

//#define TXTDEBUG

#ifdef TXTDEBUG
static FILE *f = NULL;
#endif
	void CSDSModule::Relocate(SDSFILE *SDSFile)
	{
		SDSELEM Elem;
		Elem.type = 0;
		Elem.rva = 0;
		Elem.name = nullptr;
#ifdef TXTDEBUG
	f = fopen("LOG", "w");
fprintf(f,"StringsLen %d\n", SDSFile->StringsLen);
fflush(f);
#endif
		ULONG_PTR *StringIndexes = (ULONG_PTR*)((char*)SDSFile + SDSFile->StringIndexes);
		//m_Strings = (char**)((char*)SDSFile + SDSFile->StringIndexes);
		char *Strings = (char*)((char*)SDSFile + SDSFile->StringsEntry);
		m_Strings = (char**)StringIndexes;
		for (unsigned long n = 0; n < SDSFile->StringIndexesLen; ++n)
		{
			//m_Strings[n] += (ULONG_PTR)((char*)SDSFile + SDSFile->StringsEntry);
			m_Strings[n] = Strings + StringIndexes[n];
#ifdef TXTDEBUG
//fprintf(f,"%d %s\n", n, (char*)m_Strings[n]);
//fflush(f);
#endif
		}

		SDSFile->SrcFileNames = (ULONG_PTR)((char*)SDSFile + SDSFile->SrcFileNames);
		SDSFile->Functions = (ULONG_PTR)((char*)SDSFile + SDSFile->Functions);
		SDSFile->Globals = (ULONG_PTR)((char*)SDSFile + SDSFile->Globals);
		SDSFile->Locals = (ULONG_PTR)((char*)SDSFile + SDSFile->Locals);
		SDSFile->PublicSymbols = (ULONG_PTR)((char*)SDSFile + SDSFile->PublicSymbols);
		SDSFile->FilesLN = (ULONG_PTR)((char*)SDSFile + SDSFile->FilesLN);

		if (SDSFile->PDBFullFileName)
			SDSFile->PDBFullFileName = (ULONG_PTR)((char*)SDSFile + SDSFile->PDBFullFileName);
		else	SDSFile->PDBFullFileName = (ULONG_PTR)((char*)m_Strings[0]);
#ifdef TXTDEBUG
fprintf(f,"PDBFullFileName %s\n", (char*)SDSFile->PDBFullFileName);
fflush(f);
#endif
		if (SDSFile->ModuleName)
			SDSFile->ModuleName = (ULONG_PTR)((char*)SDSFile + SDSFile->ModuleName);
		else	SDSFile->ModuleName = (ULONG_PTR)((char*)m_Strings[0]);
#ifdef TXTDEBUG
fprintf(f,"ModuleName %s\n", (char*)SDSFile->ModuleName);
fflush(f);
#endif
		if (SDSFile->FullFileName)
			SDSFile->FullFileName = (ULONG_PTR)((char*)SDSFile + SDSFile->FullFileName);
		else	SDSFile->FullFileName = (ULONG_PTR)((char*)m_Strings[0]);
#ifdef TXTDEBUG
fprintf(f,"FullFileName %s\n", (char*)SDSFile->FullFileName);
fflush(f);
#endif

#ifdef TXTDEBUG
fprintf(f,"FilesLN\n");
fflush(f);
#endif
		for (unsigned long n = 0; n < SDSFile->FilesLNLen; ++n)
		{
			FilesLN *FileLN = (FilesLN*)SDSFile->FilesLN;
#ifdef TXTDEBUG
//fprintf(f,"N %d, address %08x FileID %d UnitID %d statement %d length %d linenumber %d\n",
//		n, FileLN[n].address, FileLN[n].FileID, FileLN[n].UnitID, FileLN[n].statement, FileLN[n].length, FileLN[n].lineNumber);
//fflush(f);
#endif
		}

		for (unsigned long n = 0; n < SDSFile->SrcFileNamesLen; ++n)
		{
			SrcFileNameID *srcfilename = (SrcFileNameID*)SDSFile->SrcFileNames;
			if (srcfilename[n].Text)
				srcfilename[n].Text += (unsigned long)SDSFile;
			srcfilename[n].FileName = (char*)(m_Strings[(unsigned long)(srcfilename[n].FileName)]);
#ifdef TXTDEBUG
//fprintf(f,"N %d, FileName %s\n", n, srcfilename[n].FileName);
//fflush(f);
#endif
		}
#ifdef TXTDEBUG
fprintf(f,"GLOBAL %d\n", SDSFile->GlobalsLen);
fflush(f);
#endif
#if 1
		for (unsigned long n = 0; n < SDSFile->GlobalsLen; ++n)
		{
			BASICTYPE **type = (BASICTYPE **)SDSFile->Globals;
#ifdef TXTDEBUG
//fprintf(f,"N %d type %08x\n", n, type[n]);
//fflush(f);
#endif
#if 1
#if 1
			if (type[n] == 0)
			{
#ifdef TXTDEBUG
fprintf(f,"N %d, fixup\n", n);
fflush(f);
#endif
				type[n] = type[0];
				continue;
			}
#endif
			type[n] = (BASICTYPE*)((char*)SDSFile + (unsigned long)type[n]);
#ifdef TXTDEBUG
//fprintf(f,"N %d, _GlobalName %d Type %d Size %d\n", n, type[n]->Elem.Name, type[n]->Elem.Type, type[n]->Elem.Size);
//fflush(f);
#endif
			if (((unsigned long)type[n]->Elem.Name) > SDSFile->FileSize)
			{
#ifdef TXTDEBUG
fprintf(f,"N %d, fixup Name\n", n);
fflush(f);
#endif
				continue;
			}
			type[n]->Elem.Name = (char*)m_Strings[(unsigned long)type[n]->Elem.Name];
#ifdef TXTDEBUG
fprintf(f,"N %d, Type %d Size %d GlobalName %s \n", n, type[n]->Elem.Type, type[n]->Elem.Size, type[n]->Elem.Name);
fflush(f);
#endif

			if (type[n]->Elem.Type == ELEM_ENUM)
			{
				ENUMTYPE *et = (ENUMTYPE*)type[n];
				for (unsigned long i = 0; i < et->Length; ++i)
				{
					et->Values[i].Name = (char*)m_Strings[(unsigned long)et->Values[i].Name];
				}
			} else
			if (type[n]->Elem.Type == ELEM_STRUCT ||
			    type[n]->Elem.Type == ELEM_CLASS ||
			    type[n]->Elem.Type == ELEM_UNION)
			{
				UDTYPE *udt = (UDTYPE*)type[n];
				if (udt->DataLen)
					udt->Data = (unsigned long*)((char*)udt + (unsigned long)udt->Data);
				else	udt->Data = 0;
				if (udt->FunctionLen)
					udt->Function = (unsigned long*)((char*)udt + (unsigned long)udt->Function);
				else	udt->Function = 0;
				if (udt->BaseClassLen)
					udt->BaseClass = (unsigned long*)((char*)udt + (unsigned long)udt->BaseClass);
				else	udt->BaseClass = 0;
				if (udt->PubSymbolsLen)
					udt->PubSymbols = (CLASSPUBSYM*)((char*)udt + (unsigned long)udt->PubSymbols);
				else	udt->PubSymbols = 0;

			}
#endif
		}
#endif
#ifdef TXTDEBUG
fprintf(f,"FUNC\n");
fflush(f);
#endif
		Elem.type = 0;

		for (unsigned long n = 1; n < SDSFile->FunctionsLen; ++n)
		{
			FUNCTION **func = (FUNCTION**)SDSFile->Functions;
#ifdef TXTDEBUG
fprintf(f,"FUNC %08x\n", func[n]);
fflush(f);
#endif
		#if 0
			if (func[n] == 0)// || ((unsigned long)func[n]) > SDSFile->FileSize) //+++ >= ???
			{
fprintf(f,"N %d, fixup\n");
fflush(f);
				func[n] = func[0];
				continue;
			}
		#endif
			func[n] = (FUNCTION*)((char*)SDSFile + (unsigned long)func[n]);
			if (func[n]->nstack)// && func[n]->stack) //+++
				func[n]->stack = (unsigned long*)((char*)func[n]->stack + (unsigned long)func[n]);
			else	func[n]->stack = nullptr;

			if (func[n]->nblocks)// && func[n]->blocks) //+++
				func[n]->blocks = (unsigned long*)((char*)func[n]->blocks + (unsigned long)func[n]);
			else	func[n]->blocks = nullptr;
#ifdef TXTDEBUG
fprintf(f,"N %d, _FuncName %u, rva %x, vrva %x, id %d, C %d,S %d,I %d,P %d,V %d,A %d,L %d S(%d),B(%d)\n", n, (unsigned long)func[n]->name, func[n]->rva, func[n]->virtualOffset, func[n]->ID,
			func[n]->CplusplusExceptionHandling, func[n]->SystemExceptionHandling, func[n]->Intro, func[n]->Pure, func[n]->Virtual, func[n]->Access, func[n]->LocationType, func[n]->nstack, func[n]->nblocks);
fflush(f);		if ((unsigned long)func[n]->name > SDSFile->StringsLen) continue;
#endif
			func[n]->name = (char*)m_Strings[(unsigned long)(func[n]->name)];
			//f->name = (char*)m_Strings[(unsigned long)(f->name)];
#ifdef TXTDEBUG
fprintf(f,"N %d, FuncName %s\n", n, func[n]->name);
fflush(f);
#endif
			Elem.name = func[n]->name;
			Elem.rva = func[n]->rva;
			Elem.hash ^= n;
			m_Elems.Append(&Elem);
		}
#ifdef TXTDEBUG
fprintf(f,"DATA\n");
fflush(f);
#endif
		Elem.type = 1;

		for (unsigned long n = 0; n < SDSFile->LocalsLen; ++n)
		{
			DATATYPE *data = (DATATYPE*)SDSFile->Locals;
#ifdef TXTDEBUG
fprintf(f,"N %d, DataName %u LocationType %d\n", n, (unsigned long)data[n].name, data[n].locationType);
fflush(f);
#endif
			data[n].name = (char*)m_Strings[(unsigned long)(data[n].name)];
#ifdef TXTDEBUG
fprintf(f,"N %d, DataName %s\n", n, data[n].name);
fflush(f);
#endif
			if (data[n].locationType == LocIsStatic)
			{
#ifdef TXTDEBUG
fprintf(f,"N %d, rva %08x\n", n, data[n].Static.rva);
fflush(f);
#endif
			Elem.name = data[n].name;
			Elem.rva = data[n].Static.rva;
			Elem.hash ^= n;
			m_Elems.Append(&Elem);
			}

		}
#ifdef TXTDEBUG
fprintf(f,"PUB\n");
fflush(f);
#endif
		Elem.type = 2;

		for (unsigned long n = 0; n < SDSFile->PublicSymbolsLen; ++n)
		{
			PUBSYM *pubsym = (PUBSYM*)SDSFile->PublicSymbols;
			pubsym[n].name = (char *)m_Strings[(unsigned long)(pubsym[n].name)];
#ifdef TXTDEBUG
fprintf(f,"N %d, PUB rva %x length %u options %08x Name %s\n", n, pubsym[n].rva, pubsym[n].length, pubsym[n].options, pubsym[n].name);
fflush(f);
#endif
			pubsym[n].moduleName = (char *)m_Strings[(unsigned long)(pubsym[n].moduleName)];
#ifdef TXTDEBUG
fprintf(f,"N %d, PUB ModuleName %s\n", n, pubsym[n].moduleName);
fflush(f);
#endif
			Elem.name = pubsym[n].name;
			Elem.rva = pubsym[n].rva;
			Elem.hash ^= n;
			m_Elems.Append(&Elem);
		}
#ifdef TXTDEBUG
fclose(f);
#endif
	}

	void CSDSModule::InitGlobalsNameID()
	{
		for (unsigned long n = 1; n < m_pSDSFile->GlobalsLen; ++n)
		{
			BASICTYPE **type = (BASICTYPE **)m_pSDSFile->Globals;
			//m_GlobalsNameID.insert(_Tmap_pair<const char*,unsigned long>(type[n]->Elem.Name, n));
			m_GlobalsNameID.Insert(type[n]->Elem.Name, n);
		}
	}

	SrcFileNameID *CSDSModule::GetSrcFileName(const char *FileName) const
	{
		unsigned long Len = 0;
		SrcFileNameID *SrcFileName = GetSrcFiles(&Len);
		if (!SrcFileName) return nullptr; //+++

		for (unsigned long n = 0; n < Len; ++n)
		{
			const char *pFileName = TStrRChr(SrcFileName[n].FileName, '\\');
			if (pFileName)
			{
				++pFileName;
				if (!TStrICmp(pFileName, FileName)) return &SrcFileName[n];
			}
		}
		return nullptr;
	}

	SrcFileNameID *CSDSModule::GetSrcFileNameID(unsigned long FileID) const
	{
		unsigned long Len = 0;
		SrcFileNameID *SrcFileName = GetSrcFiles(&Len);
		if (!SrcFileName) return nullptr; //+++

		for (unsigned long n = 0; n < Len; ++n)
		{
			if (SrcFileName[n].FileID == FileID) return &SrcFileName[n];
		}
		return nullptr;
	}
	//unsigned long CSDSModule::GetSrcFilesCount() const { return m_pSrcDbg ? m_pSrcDbg->m_nSrcFilesCount : 0; }

	int CSDSModule::GetFilesLN(unsigned long FileID, map<unsigned long, MAPFilesLN> *FilesLNMap)
	{
		int Count = 0;

		unsigned long Len = 0;
		FilesLN *FileLN = GetFilesLN(&Len);
		if (!FileLN) return Count; //+++

		for (unsigned long n = 0; n < Len; ++n)
		{
			if (FileLN[n].FileID == FileID)
			{
				MAPFilesLN value;
				value.count = 1;
				value.fileln = &FileLN[n];
				if (FilesLNMap)
				{
					map<unsigned long,MAPFilesLN>::IT it = FilesLNMap->insert(_Tmap_pair<unsigned long,MAPFilesLN>(FileLN[n].lineNumber, &value));
					if (it == FilesLNMap->end())
					{
						map<unsigned long,MAPFilesLN>::IT it = FilesLNMap->find(FileLN[n].lineNumber);
						++(it->second.count);
					}
				}
				++Count;
			}
		}
		return Count;
	}

	//unsigned long
	int CSDSModule::GetFilesLN(ULONG_PTR Address, TList<FilesLN*> *ListFilesLN)
	{
		int Count = 0;

		unsigned long Len = 0;
		FilesLN *FileLN = GetFilesLN(&Len);
		if (!FileLN) return Count; //+++

		//TODO unsigned long nstart = binarysearch(0, Len, Address);
		for (unsigned long n = 0; n < Len; ++n)
		{
			if (Address >= FileLN[n].address && Address < FileLN[n].address + FileLN[n].length)
			{
				FilesLN *const fln = &FileLN[n];
				if (ListFilesLN) ListFilesLN->Append(&fln);
				++Count;
			}
		}			
		return Count;
	}

	int CSDSModule::GetFilesLN(unsigned long FileID, unsigned long lineNumber, TList<FilesLN*> *ListFilesLN)
	{
		int Count = 0;

		unsigned long Len = 0;
		FilesLN *FileLN = GetFilesLN(&Len);
		if (!FileLN) return Count; //+++

		for (unsigned long n = 0; n < Len; ++n)
		{
			if (FileLN[n].FileID == FileID && FileLN[n].lineNumber == lineNumber)
			{
				FilesLN *const fln = &FileLN[n];
				if (ListFilesLN) ListFilesLN->Append(&fln);
				++Count;
			}
		}			
		return Count;
	}

	FilesLN *CSDSModule::GetFilesLN(unsigned long FileID, unsigned long lineNumber)
	{
		unsigned long maxLineNumber = -1;
		FilesLN *filesLN = nullptr;

		unsigned long Len = 0;
		FilesLN *FileLN = GetFilesLN(&Len);
		if (!FileLN) return nullptr; //+++

		for (unsigned long n = 0; n < Len; ++n)
		{
			if (FileLN[n].FileID == FileID &&
				FileLN[n].lineNumber > lineNumber && FileLN[n].lineNumber < maxLineNumber)
			{
				maxLineNumber = FileLN[n].lineNumber;
				filesLN = &FileLN[n];
			}
		}
		return filesLN;
	}

	FUNCTION *CSDSModule::GetFunction(const char *FuncName, UDTYPE *Type, bool bSkipClassName)
	{
		if (Type &&
			(Type->Elem.Type == ELEM_STRUCT ||
			 Type->Elem.Type == ELEM_CLASS ||
			 Type->Elem.Type == ELEM_UNION)) //+++
		{
			int nClassNameLen = !bSkipClassName ? TStrLen(Type->Elem.Name)+2 : 0;

			UDTYPE *udt = Type;
			for (int n = 0; n < udt->FunctionLen; ++n)
			{
				FUNCTION *Func = GetFunctionByID(udt->Function[n]);
				if (Func && !TStrCmp(&Func->name[nClassNameLen], FuncName)) //+++
					return Func;
			}

			for (int n = 0; n < udt->BaseClassLen; ++n)
			{
				BASECLASSTYPE *bc = (BASECLASSTYPE *)GetGlobal(udt->BaseClass[n]);
				if (!bc) continue; //+++
				BASICTYPE *bcd = GetGlobal(bc->ID);
				if (!bcd) continue; //+++
				FUNCTION *Func = GetFunction(FuncName, (UDTYPE*)bcd, bSkipClassName);
				if (Func) return Func;
			}
		}
		return nullptr;
	}

	FUNCTION *CSDSModule::GetFunction(const char *FuncName, bool bCase)
	{
		if (!FuncName) return nullptr;
		if (!FuncName[0]) return nullptr;

		unsigned long Len = 0;
		FUNCTION **Func = GetFunctions(&Len);
		if (!Func) return nullptr; //+++

		for (unsigned long n = 1; n < Len; ++n)
		{
			if (bCase && !TStrCmp(Func[n]->name, FuncName))
				return Func[n];
			else
			if (!TStrICmp(Func[n]->name, FuncName))
				return Func[n];
		}
		return nullptr;
	}

	FUNCTION *CSDSModule::GetFunction(unsigned long Address)
	{
		unsigned long Len = 0;
		FUNCTION **Func = GetFunctions(&Len);
		if (!Func) return nullptr; //+++

		for (unsigned long n = 1; n < Len; ++n)
		{
			if (Address >= Func[n]->rva && Address < Func[n]->rva+Func[n]->length)
				return Func[n];
		}
		return nullptr;
	}

	FUNCTION *CSDSModule::GetFunction(unsigned long FileID, unsigned long LineNumber)
	{
		FilesLN *filesLN = nullptr;
		TList<FilesLN*> ListFilesLN;
		if (GetFilesLN(FileID, LineNumber, &ListFilesLN))
			filesLN = *ListFilesLN.Begin();
		else	filesLN = GetFilesLN(FileID, LineNumber);
		if (filesLN)
			return GetFunction(filesLN->address);
		return nullptr;
	}

	FUNCTION *CSDSModule::GetFunctionByID(unsigned long ID)
	{
		if (ID > m_pSDSFile->FunctionsLen)
			ID = 0;
		FUNCTION **funcs = (FUNCTION **)m_pSDSFile->Functions;
		return (FUNCTION *)funcs[ID];
	}

	DATATYPE *CSDSModule::GetLocal(unsigned long ID)
	{
		if (ID >= m_pSDSFile->LocalsLen)
			ID = 0;
		DATATYPE *data = (DATATYPE *)m_pSDSFile->Locals;
		return &data[ID]; //???
	}
	DATATYPE *CSDSModule::GetLocal(const char *Name, unsigned long DT)
	{
		unsigned long Len = 0;
		DATATYPE *data = GetLocal(&Len);
		if (!data) return nullptr; //+++

		for (unsigned long ID = 1; ID < Len; ++ID)
		{
			if (data[ID].dataKind == DT && !TStrCmp(data[ID].name, Name))
				return &data[ID];
		}
		return nullptr;
	}

	BASICTYPE *CSDSModule::GetGlobal(unsigned long ID)
	{
		if (ID >= m_pSDSFile->GlobalsLen)
			ID = 0;
		BASICTYPE **type = (BASICTYPE **)m_pSDSFile->Globals;
		return type[ID];
	}

	char *CSDSModule::GetOutput(BASICTYPE *Type, char *pStr, int *pLen)
	{
		if (pStr) *pStr = 0;
		if (!Type) return pStr;
		
		switch (Type->Elem.Type)
		{
		case ELEM_POINTER: {
			int Len = *pLen;
			TAnsiString String;
			POINTERTYPE *pt = (POINTERTYPE*)Type;
			BASICTYPE *subtype = GetGlobal(pt->ID);
			if (!subtype) return pStr; //???
			GetOutput(pt->ID, pStr, pLen);
			int l = pt->Num;
			String.Fill('*', l);
			if (subtype->Elem.Type == ELEM_FUNC_TYPE)
				l += 2;
			if (*pLen > l)
			{
				if (subtype->Elem.Type == ELEM_FUNC_TYPE)
				{
					const char *pStrCallConv = " "; // GetCallConv(subtype->callingConvention)
					char *p = new char[Len];
					TStrCpy(p, pStr);
					char *b = TStrStr(pStr, pStrCallConv);
					char *pStrFunc = TStrFunc(p);
					int bl = b ? (b-pStr) : (pStrFunc-p);
					pStr[bl] = '(';
					++bl;
					bl += TStrCpy(&pStr[bl], pStrCallConv);
					TStrCpy(&pStr[bl], String.operator const char*());
					TStrCat(pStr, ")");
					TStrCat(pStr, pStrFunc);
					delete []p;
				} else
				{
					TStrCat(pStr, String.operator const char*());
				}
			}
			*pLen -= l;
			} break;
		case ELEM_POINTERREF: {
			POINTERREFTYPE *pt = (POINTERREFTYPE*)Type;
			GetOutput(pt->ID, pStr, pLen);
			TSTRADD(pStr, "&", pLen); //???
			//if (*pLen > 1)
			//	TStrCat(pStr, "&");
			////*pLen -= l; //??? TODO
			} break;
		case ELEM_ARRAY: {
			ARRAYTYPE *at = (ARRAYTYPE *)Type;
			BASICTYPE *subtype = GetGlobal(at->ID);
			if (!subtype) return pStr;
			GetOutput(subtype, pStr, pLen);
			bool bFunc = IsPointerFunction(subtype);
			TAnsiString String;
			char Buffer[32];
			int l = 0;
			for (int n = 0; n < at->Size; ++n)
			{
				l += TSPrintf(Buffer, "%d", at->Values[n].Size) + 2;
				String += "[";
				String += Buffer;
				String += "]";
			}
			if (*pLen > l)
			{
				if (bFunc)
				{
					TAnsiString Str;
					char *pStrFunc = TStrFunc(pStr);
					Str = pStrFunc;
					--pStrFunc;
					*pStrFunc = 0;

					TStrCat(pStr, String.operator const char*());
					TStrCat(pStr, ")");
					TStrCat(pStr, Str.operator const char*());
				} else
				{
					TStrCat(pStr, String.operator const char*());
				}
			}					
			*pLen -= l;
			} break;
		case ELEM_ENUM: {
			char Buf[52];
			TStrCpy(Buf, "enum ");
			TSTRADD(pStr, Buf, pLen);
			goto PrintName;
			} break;
		case ELEM_STRUCT: {
			char Buf[52];
			TStrCpy(Buf, "struct ");
			TSTRADD(pStr, Buf, pLen);
			goto PrintName;
			} break;
		case ELEM_CLASS: { //++
			char Buf[52];
			TStrCpy(Buf, "class ");
			TSTRADD(pStr, Buf, pLen);
			goto PrintName;
			} break;
		case ELEM_UNION: {
			char Buf[52];
			TStrCpy(Buf, "union ");
			TSTRADD(pStr, Buf, pLen);
			goto PrintName;
			} break;
		case ELEM_FUNC_TYPE: {
			FUNCTYPE *ft = (FUNCTYPE*)Type;
			BASICTYPE *bt = (BASICTYPE*)GetGlobal(ft->FuncTypeID);
			if (!bt) return pStr;

			GetOutput(bt, pStr, pLen);

			const char *pStrCallConv = " "; // GetCallConv(ft->callingConvention)
			TSTRADD(pStr, pStrCallConv, pLen);
			TSTRADD(pStr, "(", pLen);

			TAnsiString String;
			int End = TStrLen(pStr);
			for (int n = 0; n < ft->nArgs; ++n)
			{
				End = TStrLen(pStr);
				GetOutput(GetGlobal(ft->ArgsIDs[n]), &pStr[End], pLen);
				End = TStrLen(pStr);
				if (n != ft->nArgs-1)
				{
					TSTRADD(&pStr[End], ",", pLen);
					++End;
				}
			}

			TSTRADD(&pStr[End], ")", pLen);
			} break;
		case ELEM_BASE_CLASS: {
			BASECLASSTYPE *bt = (BASECLASSTYPE*)Type;
			return GetOutput(bt->ID, pStr, pLen);
			} break;
		default:
		PrintName:
			{
			const char *str = Type->Elem.Name;
			if (!str) str = "no name";
			TSTRADD(&pStr[0], str, pLen);
			}
		}
		return pStr;
	}

	char *CSDSModule::GetOutput(unsigned long ID, char *pStr, int *pLen)
	{
		BASICTYPE *type = GetGlobal(ID);
		if (!type) return pStr; //+++
		return GetOutput(type, pStr, pLen);
	}

	bool CSDSModule::IsPointerFunction(BASICTYPE *Type)
	{
		if (Type && Type->Elem.Type == ELEM_POINTER)
		{
			POINTERTYPE *pt = (POINTERTYPE*)Type;
			return GetGlobal(pt->ID)->Elem.Type == ELEM_FUNC_TYPE;
		}
		return false;
	}

	//for src tip
	bool CSDSModule::GetOutputTypedef(TYPEDEFTYPE *Type, TAnsiString & String)
	{
		bool bFuncPtr = false;
		char UDTStr[20];
		memset(UDTStr, 0, sizeof(UDTStr));
		String.Empty();

		BASICTYPE *bt = (BASICTYPE*)GetGlobal(Type->ID);
		switch (bt->Elem.Type)
		{
		case ELEM_POINTER:{
			POINTERTYPE *pt = (POINTERTYPE *)bt;
			BASICTYPE *b = (BASICTYPE *)GetGlobal(pt->ID);
			if (b->Elem.Type == ELEM_FUNC_TYPE)
				bFuncPtr = true;
			} break;
		case ELEM_ENUM:
			TStrCpy(UDTStr, "");
			//TStrCpy(UDTStr, "enum ");
			break;
		case ELEM_STRUCT:
			TStrCpy(UDTStr, "");
			//TStrCpy(UDTStr, "struct ");
			break;
		case ELEM_UNION:
			TStrCpy(UDTStr, "");
			//TStrCpy(UDTStr, "union ");
			break;
		default:;
		}
		char Buffer[1024];
		int Len = 1023;
		GetOutput(Type->ID, Buffer, &Len);
		if (bFuncPtr)
		{
			char *pStrFunc = TStrFunc(Buffer);
			if (pStrFunc)
			{
				--pStrFunc;
				pStrFunc[0] = 0;
				String += "typedef ";
				String += Buffer;
				String += Type->Elem.Name;
				String += ")";
				String += pStrFunc+1;
			} else
			{
				String += "typedef ";
				String += UDTStr;
				String += " ";
				String += Type->Elem.Name;
			}
		} else
		{
			String += "typedef ";
			String += UDTStr;
			String += Buffer;
			String += " ";
			String += Type->Elem.Name;
		}
		return false;
	}

	//for src tip
	bool CSDSModule::GetOutputEnum(ENUMTYPE *Type, TAnsiString & String)
	{
		String.Empty();

		String += "enum ";
		String += Type->Elem.Name;
		String += "{";
		for (int n = 0; n < Type->Length; ++n)
		{
			String += "\t";
			String += Type->Values[n].Name;
			String += "\t=";
			char Buffer[52];
			TSPrintf(Buffer, "0x%08x\t,//%d",
				(unsigned long)Type->Values[n].Value,
				(unsigned long)Type->Values[n].Value); //??? %d
			String += Buffer;
		}
		String += "};";
		return false;
	}

	char *CSDSModule::GetOutputFunc(FUNCTION *pFunc, char *pStr, int *Len)
	{
		char prolog[10];
		int offset = 0;
		TStrCpy(prolog, "virtual ");
		if (pFunc->Virtual)
		{
			offset = TSTRADD(pStr, prolog, Len);
		}

		if (pFunc->ReturnTypeID)
			GetOutput(pFunc->ReturnTypeID, &pStr[offset], Len);

		if (pFunc->ReturnCallID)
		{
			BASICTYPE *subtype = GetGlobal(pFunc->ReturnCallID);
			if (subtype->Elem.Type == ELEM_FUNC_TYPE)
			{
				const char *pStrCallConv = " "; // GetCallConv(subtype->callingConvention)
				TSTRADD(pStr, pStrCallConv, Len);
			}
		}

		TSTRADD(pStr, pFunc->name, Len);
		TSTRADD(pStr, "(", Len);
		
		for (unsigned long n = 0; n < pFunc->nstack; ++n)
		{
			DATATYPE *data = GetLocal(pFunc->stack[n]);
			if (data && data->dataKind == DataIsParam)
			{
				GetOutput(data->TypeID, &pStr[TStrLen(pStr)], Len);

				TSTRADD(pStr, " ", Len);
				TSTRADD(pStr, data->name, Len);
				TSTRADD(pStr, ",", Len);
			}
		}

		//drop remain fake args
		int nStrArg = TStrLen(pStr);
		if (pStr[nStrArg-1] == ',')
			pStr[nStrArg-1] = 0;

		char epilog[10];
		if (pFunc->Pure)
			TStrCpy(epilog, ")=0");
		else	TStrCpy(epilog, ")");

		TSTRADD(pStr, epilog, Len);

		return pStr;
	}

	DATATYPE *CSDSModule::GetClassLocal(const char *Name, FUNCTION *pFunc)
	{
		if (!pFunc) return nullptr;
		if (!Name) return nullptr;
		if (!Name[0]) return nullptr;

		for (int n = 0; n < pFunc->nstack; ++n)
		{
			DATATYPE *data = GetLocal(pFunc->stack[n]);
			if (data && !TStrCmp(data->name, Name))
				return data;
		}

		for (int n = 0; n < pFunc->nblocks; ++n)
		{
			BLOCKTYPE *block = (BLOCKTYPE*)GetGlobal(pFunc->blocks[n]);
			if (block)
			{
				for (int i = 0; i < block->count; ++i)
				{
					DATATYPE *data = GetLocal(block->IDs[i]);
					if (data && !TStrCmp(data->name, Name))
						return data;
				}
			}
		}
		
		if (pFunc->ClassID)
		{
			unsigned long offset = 0;
			UDTYPE *type = (UDTYPE *)GetGlobal(pFunc->ClassID);
			if (type) //+++
				return GetClassMember(Name, type, &offset);
		}
		return nullptr;
	}

	DATATYPE *CSDSModule::GetClassMember(const char *Name, UDTYPE *Type, unsigned long *offset)
	{
		if (Type && (Type->Elem.Type == ELEM_STRUCT ||
		 	     Type->Elem.Type == ELEM_CLASS ||
			      Type->Elem.Type == ELEM_UNION))
		{
			for (int n = 0; n < Type->DataLen; ++n)
			{
				DATATYPE *data = GetLocal(Type->Data[n]);
				if (data && !TStrCmp(data->name, Name))
				{
					if (offset)
					{
						if (data->locationType == LocIsThisRel)
							*offset += data->ThisRel.offset;
						else	*offset = data->ThisRel.offset; //TODO ???
					}
					return data;
				}
			}

			for (int n = 0; n < Type->BaseClassLen; ++n)
			{
				BASECLASSTYPE *bc = (BASECLASSTYPE *)GetGlobal(Type->BaseClass[n]);
				DATATYPE *data = GetClassMember(Name, (UDTYPE*)GetGlobal(bc->ID), offset);
				if (data)
				{
					if (offset)
					{
						if (data->locationType == LocIsStatic)
							*offset = data->Static.rva;
						else
						if (data->locationType == LocIsThisRel)
							*offset += bc->Offset;
					}
					return data;
				}
			}				
		}
		return nullptr;
	}

	void *CSDSModule::GetGlobal(char *Name, FUNCTION *pFunc, bool *bFunc, unsigned long *offset)
	{
		if (offset) *offset = 0;
		if (bFunc) *bFunc = false;
		if (!pFunc) return nullptr;
		int Len = TStrLen(Name);
		int index = 0;
		while (index < Len)
		{
			if (Name[index] == '.')
			{
				Name[index] = 0;
				++index;
				//newindex = index;
				break;
			} else
			if (Name[index] == '-')
			{
				Name[index] = 0;
				index += 2;
				//newindex = index;
				break;
			}
			++index;
			//newindex = index;
		}

		DATATYPE *data = GetClassLocal(Name, pFunc);
		if (!data)
		{
			if (pFunc->ClassID)
				data = GetClassMember(Name, (UDTYPE*)GetGlobal(pFunc->ClassID), nullptr);
			if (!data)
			{
				data = GetLocal(Name, DataIsGlobal);
				if (!data) return nullptr;
				if (offset && data->locationType == LocIsStatic)
					*offset = data->Static.rva;
			}
		}

		if (data->TypeID == 0)
		{
			if (index != Len) return nullptr;

			FUNCTION *func = GetFunction(Name, true);
			if (func)
			{
				if (bFunc) *bFunc = true;
			}
			return func;
		}

		if (index == Len) return data;
		BASICTYPE *type = GetGlobal(data->TypeID);
		if (type->Elem.Type == ELEM_POINTER)
			type = GetGlobal(((POINTERTYPE*)type)->ID);
		if (type->Elem.Type == ELEM_POINTERREF)
			type = GetGlobal(((POINTERREFTYPE*)type)->ID);
		if (!(type && (type->Elem.Type == ELEM_UNION ||
			       type->Elem.Type == ELEM_CLASS ||
				type->Elem.Type == ELEM_STRUCT))) return nullptr;

		Name = &Name[index];
		char *SubName = Name;
		while (index < Len)
		{
			if (SubName[0] == '.')
			{
				SubName[0] = 0;
				data = GetClassMember(Name, (UDTYPE*)type, offset);
				if (!data) goto FUNC;
				++index;
				SubName += 1;

				if (index == Len) return data;
				type = GetGlobal(data->TypeID);
				if (type->Elem.Type == ELEM_POINTER)
					type = GetGlobal(((POINTERTYPE*)type)->ID);
				if (type->Elem.Type == ELEM_POINTERREF)
					type = GetGlobal(((POINTERREFTYPE*)type)->ID);
				if (!(type && (type->Elem.Type == ELEM_UNION ||
						type->Elem.Type == ELEM_CLASS ||
						type->Elem.Type == ELEM_STRUCT))) return nullptr;
				Name = SubName;
			} else
			if (SubName[0] == '-')
			{
				SubName[0] = 0;
				data = GetClassMember(Name, (UDTYPE*)type, offset);
				if (!data) goto FUNC;
				index += 2;
				SubName += 2;

				if (index == Len) return data;
				type = GetGlobal(data->TypeID);
				if (type->Elem.Type == ELEM_POINTER)
					type = GetGlobal(((POINTERTYPE*)type)->ID);
				if (type->Elem.Type == ELEM_POINTERREF)
					type = GetGlobal(((POINTERREFTYPE*)type)->ID);
				if (!(type && (type->Elem.Type == ELEM_UNION ||
						type->Elem.Type == ELEM_CLASS ||
						type->Elem.Type == ELEM_STRUCT))) return nullptr;
				Name = SubName;
			}
			++index;
			++SubName;
		}
		data = GetClassMember(Name, (UDTYPE*)type, offset);
		if (data) return data;
	FUNC:;
		FUNCTION *func = GetFunction(Name, (UDTYPE*)type, false);
		if (func)
		{
			if (bFunc) *bFunc = true;
		}
		return func;
	}

	void *CSDSModule::GetGlobal(char *Name, unsigned long FileID, unsigned long LineNumber, bool *bFunc, unsigned long *offset)
	{
		FUNCTION *func = GetFunction(FileID, LineNumber);
		return GetGlobal(Name, func, bFunc, offset);
	}

	BASICTYPE *CSDSModule::CSDSModule::GetGlobal(const char *Name, bool X)
	{
		if (!Name) return nullptr;
		if (!Name[0]) return nullptr;
		map<const char*, unsigned long, MY_char_compare>::IT it = m_GlobalsNameID.find(Name);
		if (it != m_GlobalsNameID.end())
		{
			return GetGlobal(it->second);
		}
		return nullptr;
	}

	const char *CSDSModule::GetEnumName(unsigned long ID, unsigned long long Value)
	{
		ENUMTYPE *et = (ENUMTYPE *)GetGlobal(ID);
		if (!et) return nullptr;
		for (int n = 0; n < et->Length; ++n)
		{
			if (et->Values[n].Value == Value)
				return et->Values[n].Name;
		}
		return nullptr;
	}

	const char *CSDSModule::GetName(BASICTYPE *Type)
	{
		if (Type)
			return Type->Elem.Name;
		return (const char*)m_Strings[0];
	}
