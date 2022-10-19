#ifndef _CURRENTFUNCTION_HPP_
#define _CURRENTFUNCTION_HPP_

#include "../Code/alt/altlist.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

struct CCurrentFunction
{
	ULONG_PTR m_ImageBaseTRUE;
	ULONG_PTR m_ImageBase;
	ULONG_PTR m_ImageHighBase;
	unsigned long m_Length;
	CSDSModule *m_pSDSMod;
	FUNCTION *m_pFunc;
	TList<DATATYPE*> m_Vars;
	TList<FUNCTION*> m_ClassFunctions;
	BASICTYPE *m_Class;

	CCurrentFunction();
	virtual ~CCurrentFunction();

	DATATYPE *GetLocal(char *pStr, bool bNoCase);
	FUNCTION *GetFunction(const char *FuncName, bool X);
	const char *GetSymbol(unsigned long displacement, unsigned int base, DATATYPE **Type) const;
	void AddClassFunctions(BASICTYPE *Type);
	void Update();
	void Init(CSDSModule *pSDSMod, FUNCTION *pFunc);
};

#endif
