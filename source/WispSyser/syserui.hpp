
#ifndef _SYSERUI_HPP_
#define _SYSERUI_HPP_

#include "plugin.hpp"
#include "../EXEAnalyzer/symbol.hpp"
#include "../EXEAnalyzer/debugger.hpp"
#include "../EXEAnalyzer/x86debugger.hpp"
#include "../EXEAnalyzer/instrsym.hpp"
#include "../Code/alt/altexpcalc.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

	typedef TExpCalc<char,ULONG_PTR,1> TSyserExpCalc; //NoCase String

	struct CSyserCodeDoc : CCodeDoc, CInstrSym, TSyserExpCalc//TExpCalc<char,ULONG_PTR,1>
	{
		X86_CPU_REG_PTR m_CPUREGPTR;
		unsigned long m_OPCODE;
		unsigned long m_PID;
		unsigned long m_TID;
		unsigned long m_BPCOUNT;
		TAnsiString m_ExeFilePath;

		CSyserCodeDoc();

		static TExpCalc<char,ULONG_PTR,1>::EXPITEM * Reg(TListIter<TExpCalc<char,ULONG_PTR,1>::EXPITEM> It, TList<TExpCalc<char,ULONG_PTR,1>::EXPITEM> & Expression, TExpCalc<char,ULONG_PTR,1> *pThis);

		void InsertX86RegSym(X86_CPU_REG_PTR *pCPURegPtr);
		virtual bool Open(const char *szModule) override; //04 TODO PARAM
		virtual void Close() override;			//08
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override; //0C
		virtual ULONG_PTR GetInstrAddress(ULONG_PTR Address, long Count) override; //10

		virtual bool GetSymbol(ULONG_PTR Address, WCHAR *szName, int Length) override; //14
		virtual bool GetComment(ULONG_PTR Address, WCHAR *szName, int Length) override; //18
		virtual bool SetComment(ULONG_PTR Address, const WCHAR *szName) override; //1C
		//virtual bool GetFunction(ULONG_PTR Address, WCHAR *szName, int Length) override; //20
		virtual unsigned long GetPic(ULONG_PTR Address) override; //24

		virtual bool AddrToSym(ULONG_PTR Address, char *szSym, unsigned int SymLen) override;

		virtual bool GetSymbolBase(ULONG_PTR *BaseAddr) override;

		virtual bool GetValue(const char *szItem, ULONG_PTR *pValue) const override;
		virtual ULONG_PTR ReadValue(ULONG_PTR Address) override;

		void OnTestCondition();
		void GetInstrInfo(WCHAR *pInstrInfoStr);
		unsigned long ReadColorString(ULONG_PTR Address, unsigned long *pColorString, unsigned long MaxSize);
		void ContextCS(unsigned long *pColorString, int MaxLen);
		bool IsDasm(ULONG_PTR Address, unsigned long Size);
	};

	struct CSyserDI : public CDebugInterface
	{
		virtual void OnLoadModule(CDebugger *pDebugger, const char *szName, ULONG_PTR ImageBase, unsigned long ImageSize, unsigned long TimeStamp, unsigned long CheckSum) override;
		virtual void OnUnloadModule(CDebugger *pDebugger, ULONG_PTR ImageBase) override;
		virtual void OnTestCondition() override;

		virtual bool Close(bool bExit) override;
		virtual bool Plunge(unsigned int Code) override;

		virtual bool OnExit(bool bExit) override;
		virtual bool OnDebugException(unsigned int Code) override;

		virtual void DisplayMsg(const char *szMsg) override;
		virtual void UpdateContext() override;
		virtual bool TestCondition(const char *Condition, BREAK_POINT *pBP) override;
		virtual bool RunCmd(const char *szCmd) override;
		virtual void RunCmds(const char *szCmds) override;

		virtual CDebugger *GetDebugger() override;
	};

	struct CCallStack
	{
		struct Frame
		{
			ULONG_PTR Address;
			ULONG_PTR Return;
			ULONG_PTR FuncAddr;
			TAnsiString FuncName;
		};

		TList<Frame> m_Frames;
		TListIter<Frame> m_HeadIt;
		ULONG_PTR m_Address;

		CCallStack();
		void UpdateContext(ULONG_PTR Address);
	};

	struct MODULE_LOAD
	{
		unsigned long State;
		unsigned long Type;
	};

	struct APIFunctionArguments : public TList<TAnsiString>
	{
		unsigned long Read(ALT::ALTFileStream &Stream)
		{
			Clear();
			unsigned long Len = 4;
			unsigned long Count = 0;
			if (Stream.Read(&Count, Len) != Len) return 0;

			while (Count>0)
			{
				unsigned long PLen = Append(0)->Read(Stream);
				if (!PLen)
					return 0;
				Len += PLen;
				--Count;
			}
			return Len;
		}
	};

	struct APIFunction
	{
		TAnsiString Return;
		APIFunctionArguments Arguments;
		unsigned long Declaration;

		unsigned long Read(ALT::ALTFileStream &Stream)
		{
			unsigned long Ret = 0;
			unsigned long RLen = Return.Read(Stream);
			if (!RLen) return 0;
			Ret += RLen;
			unsigned long ALen= Arguments.Read(Stream);
			if (!ALen) return 0;
			Ret += ALen;
			unsigned long SLen = Stream.Read(&Declaration, 4);
			if (!SLen) return 0;
			Ret += SLen+4;
			return Ret;
		}
	};

	struct CAPIDef : public map<TAnsiNoCaseString, APIFunction> //multimap
	{
		unsigned long Read(ALT::ALTFileStream &Stream)
		{
			TAnsiNoCaseString Name;
			clear();	
			unsigned long Len = 4;
			unsigned long Count = 0;
			if (Stream.Read(&Count, Len) != Len) return 0;

			while (Count>0)
			{
				unsigned long NLen = Name.Read(Stream);
				if (!NLen) return 0;
				Len += NLen;

				//InsertNonUnique
				map<TAnsiNoCaseString, APIFunction>::IT it = insert(
						_Tmap_pair<TAnsiNoCaseString, APIFunction>(Name,APIFunction()));
				if (it == end()) return 0;

				unsigned long Ret = it->second.Read(Stream);
				if (!Ret)
					return 0;
				Len += Ret;
				--Count;
			}
			return Len;
		}
	};

	struct CSyserUI : public CSyserPluginUI
	{
		CAPIDef m_APIDef;
		CSyserCodeDoc m_SyserCodeDoc;
		CSymbolContainer m_SymbolContainer;
		CCommentContainer m_CommentContainer;
		CSyserDI m_DebugInterface;
		map<FILESIG, CSDSModule> m_SDSModulesMap;
		CCallStack m_CallStack;

		//redirect BP place 
		map<unsigned long long, BREAK_POINT> m_CodeBPMap;
		TList<BREAK_POINT> m_DataBPList;

		map<ULONG_PTR, BREAK_POINT::MODULE> m_ModuleBPMap;
		map<TAnsiNoCaseString, MODULE_LOAD> m_ModuleLoadBPMap;

		void SetCodeMode(int Mode)
		{
			m_SyserCodeDoc.m_Mode = Mode;
		}

		CSyserUI()
		{
			m_DebugInterface.m_pCodeDoc = &m_SyserCodeDoc;
			m_DebugInterface.m_pCommentContainer = &m_CommentContainer;
			m_DebugInterface.m_pSymbolContainer = &m_SymbolContainer;
		}

		~CSyserUI()
		{
		}

		virtual bool RegisterPluginModule(const WCHAR *ModuleName, SYSER_PLUGIN_MODULE *pPluginModule) override;
		virtual bool UnregisterPluginModule(const WCHAR *ModuleName) override;

		virtual int GetInstrLen(ULONG_PTR Address) override;

		bool CalcExp(const char *szExp, ULONG_PTR *pResult);
		virtual bool CalcExp(const WCHAR *szExp, ULONG_PTR *pResult) override;

		virtual bool InsertCmd(const WCHAR *szCmd, FPCmd pCmdProc, void *pUserData, const WCHAR *pComment, const WCHAR *pUsage) override;
		virtual void RemoveCmd(const WCHAR *szCmd) override;
		virtual int RunCmd(const WCHAR *szCmd) override;

		virtual void Outputf(const WCHAR *szFormat, ...) override;
		virtual void Output(const unsigned long *szMsg) override;

		virtual void *InsertMenu(void *hParentMenu, const WCHAR *szMenuName, FPMenuProc fpMenuProc) override;
		virtual bool RemoveMenu(void *hMenu) override;
		virtual bool EnableMenu(void *hMenu, bool bEnable) override;

		virtual void *GetMainTabWnd() override;
		virtual void *GetMainMenu() override;
		virtual void *GetWisp() override;

		virtual unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size) override;
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override;

		bool ReadUString(ULONG_PTR Address, WCHAR *pString, unsigned long Size);
		bool ReadAString(ULONG_PTR Address, char *pString, unsigned long Size);
		bool ReadString(ULONG_PTR Address, WCHAR *String, unsigned long Size, char *Encode);
		bool GetSymbol(ULONG_PTR Address, char *szName, int Length);
		bool GetSymbolOffset(ULONG_PTR Address, char *szName);
		unsigned long SetCommentModule(CCommentModule *pCommentModule);
		unsigned long RemoveCommentModule(CCommentModule *pCommentModule);
		unsigned long SetSymbolModule(CSymbolModule *pSymMod);

		unsigned long LoadSDSModuleSym(CSDSModule *pSDSMod);
		CSDSModule *FindSDSModule(ULONG_PTR ImageBase);
		bool UnloadSDSModule(CSDSModule *pSDSMod);
		CSDSModule *LoadSDSModule(const char *FileName);

		bool LoadPDBSym(const char *szModule, ULONG_PTR ImageBase, unsigned long ImageSize);

		CSymbolModule *LoadPESym(const char *szModule, CDbgModule *pDbgMod);
		CSymbolModule *LoadMap(const char *szMapFile, const char *szModule, CDbgModule *pDbgMod);
		unsigned long GetModuleName(char *szModuleName);
		bool IsGlobeModule(ULONG_PTR Address, const char *szFullFileName);

		bool ToggleInsertBP(ULONG_PTR Address);
		bool ToggleEnableBP(ULONG_PTR Address);
		bool InsertBP(ULONG_PTR Address, const char *szName, const char *szCondition, const char *szCommand);
		bool RemoveBP(ULONG_PTR Address);
		bool EnableModuleBP(ULONG_PTR Address, bool Enable);
		void EnableAllModuleBP(bool Enable);

		bool InsertModuleOnloadBP(const char *szName, unsigned int Type, unsigned int State);
		void EnableModuleOnloadBP(bool Enable);

		void LoadAPIDef();
		bool SaveComment();
		bool LoadComment();
	};

#endif