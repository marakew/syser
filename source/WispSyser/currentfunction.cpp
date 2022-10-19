
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"

#include "currentfunction.hpp"

	CCurrentFunction::CCurrentFunction()
	{
		m_ImageBaseTRUE = 0;
		m_ImageBase = 0;
		m_ImageHighBase = 0;
		m_Length = 0;
		m_pFunc = nullptr;
		m_Class = nullptr;
		m_pSDSMod = nullptr;
	}

	CCurrentFunction::~CCurrentFunction()
	{
	}

	DATATYPE *CCurrentFunction::GetLocal(char *pStr, bool bNoCase)
	{
		TListIter<DATATYPE*> It = m_Vars.Begin();
		while (It != m_Vars.End())
		{
			DATATYPE *data = *It;
			if (bNoCase && !TStrCmp(pStr, data->name))
				return data;
			else
			if (!TStrICmp(pStr, data->name))
				return data;
			++It;
		}
		if (m_pFunc && m_pFunc->ClassID) //+++
		{
			BASICTYPE *Type = m_pSDSMod->GetGlobal(m_pFunc->ClassID);
			if (Type) //+++
			{
				DATATYPE *data = m_pSDSMod->GetClassMember(pStr, (UDTYPE*)Type, nullptr);
				if (data) return data;
			}
		}
		return m_pSDSMod->GetLocal(pStr, DataIsGlobal);
	}

	FUNCTION *CCurrentFunction::GetFunction(const char *FuncName, bool X)
	{
		if (m_pFunc && m_pFunc->ClassID) //+++
		{
			FUNCTION *Func = m_pSDSMod->GetFunction(FuncName, (UDTYPE*)m_Class, false);
			if (Func) return Func;
		}
		return m_pSDSMod->GetFunction(FuncName, true);
	}

	const char *CCurrentFunction::GetSymbol(unsigned long displacement, unsigned int base, DATATYPE **Type) const
	{
	#ifdef _X64_
		static const int Reg2Index[] = {
					CV_AMD64_RAX,
					CV_AMD64_RCX,
					CV_AMD64_RDX,
					CV_AMD64_RBX,
					CV_AMD64_RSP,
					CV_AMD64_RBP,
					CV_AMD64_RSI,
					CV_AMD64_RDI};
	#else
		static const int Reg2Index[] = {
					CV_REG_EAX,
					CV_REG_ECX,
					CV_REG_EDX,
					CV_REG_EBX,
					CV_REG_ESP,
					CV_REG_EBP,
					CV_REG_ESI,
					CV_REG_EDI};
	#endif
		int registerId = 0;
		if (base != -1)
		{
			if (base > 7) return 0;
	//		if (base > 32) return 0; //???
			registerId = Reg2Index[base];
		}

		if (Type) *Type = 0;

		TListIter<DATATYPE*> It = m_Vars.Begin();
		while (It != m_Vars.End())
		{
			DATATYPE *data = *It;
			if (base != -1 && data->locationType == LocIsRegRel && //??? >> 8
			    data->RegRel.offset == displacement &&
			    data->RegRel.registerId == registerId)
			{
				if (Type) *Type = *It;
				return data->name;
			} else
			if (data->locationType == LocIsStatic &&
			    (m_ImageBaseTRUE + data->Static.rva) == displacement)
			{
				if (Type) *Type = *It;
				return data->name;
			}
			++It;
		}
		return nullptr;
	}

	void CCurrentFunction::AddClassFunctions(BASICTYPE *Type)
	{
		if (Type && (Type->Elem.Type == ELEM_STRUCT ||
			      Type->Elem.Type == ELEM_CLASS ||
			      Type->Elem.Type == ELEM_UNION))
		{
			UDTYPE *udt = (UDTYPE*)Type;
			for (int n = 0; n < udt->FunctionLen; ++n)
			{
				FUNCTION *func = m_pSDSMod->GetFunctionByID(udt->Function[n]);
				if (func)
					m_ClassFunctions.Append(&func);
			}
		}
	}

	void CCurrentFunction::Update()
	{
		m_Vars.Clear();
		for (int n = 0; n < m_pFunc->nstack; ++n)
		{
			DATATYPE *data = m_pSDSMod->GetLocal(m_pFunc->stack[n]);
			if (data)
				m_Vars.Append(&data);
		}

		for (int n = 0; n < m_pFunc->nblocks; ++n)
		{
			BLOCKTYPE *block = (BLOCKTYPE *)m_pSDSMod->GetGlobal(m_pFunc->blocks[n]);
			if (block->Elem.Type == ELEM_BLOCK) //+++ ???
			for (int i = 0; i < block->count; ++i)
			{
				DATATYPE *data = m_pSDSMod->GetLocal(block->IDs[i]);
				if (data)
					m_Vars.Append(&data);
			}
		}
		BASICTYPE *Class = m_pSDSMod->GetGlobal(m_pFunc->ClassID);
		if (Class && m_Class != Class)
		{
			m_ClassFunctions.Clear();
			AddClassFunctions(Class);
			m_Class = Class;
		}
	}

	void CCurrentFunction::Init(CSDSModule *pSDSMod, FUNCTION *pFunc)
	{
		if (pFunc)
		{
			m_pSDSMod = pSDSMod;
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(pSDSMod->m_ExeFileName.operator const char*());
			if (pDbgMod)
				m_ImageBaseTRUE = pDbgMod->m_ImageBase;
			else	m_ImageBaseTRUE = 0;
			m_ImageBase = m_ImageBaseTRUE + pFunc->rva; //TODO DONE
			m_ImageHighBase = m_ImageBase + pFunc->length;
			m_Length = pFunc->length;
			m_pFunc = pFunc;
			Update();
		} else
		{
			m_ImageBaseTRUE = 0;
			m_ImageBase = 0;
			m_ImageHighBase = 0;
			m_Length = 0;
			m_pFunc = 0;
			m_Class = 0;
			m_Vars.Clear();	
			m_ClassFunctions.Clear();
			m_pSDSMod = pSDSMod;
		}
	}	
