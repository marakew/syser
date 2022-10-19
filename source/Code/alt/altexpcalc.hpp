
#ifndef _ALT_CALCEXP_HPP_
#define _ALT_CALCEXP_HPP_

#include "../define.h"
#include "../tstrmem.hpp"
#include "altlist.hpp"
#include "altmap.hpp"

//TExpCalc<char, unsigned long, 1>
//TEST altexpcalc.exe 4*(2+1)/(12/6)
//OK 4

//TEST altexpcalc.exe ffffffff+1
//OK 0

template<typename T, typename R, int NoCaseCmp>
struct TExpCalc
{
	TExpCalc()
	{
		AddFunc("&&", 5, LogicAnd, this);
		AddFunc("||", 5, LogicOr, this);
		AddFunc("^^", 5, LogicXor, this);

		AddFunc(">=", 4, NotBelow, this);
		AddFunc("<=", 4, NotAbove, this);
		AddFunc("==", 4, Equal, this);
		AddFunc("!=", 4, NotEqual, this);

		AddFunc(">>", 1, ShiftRight, this);
		AddFunc("<<", 1, ShiftLeft, this);

		AddFunc(">", 4, Above, this);
		AddFunc("<", 4, Below, this);

		AddFunc("+", 3, Add, this);
		AddFunc("-", 3, Sub, this);

		AddFunc("*", 2, Mul, this);
		AddFunc("/", 2, Div, this);
		AddFunc("%", 2, Mod, this);

		AddFunc("@", 1, Addr, this);
		AddFunc("^", 1, Xor, this);
		AddFunc("!", 1, Not, this);
		AddFunc("&", 1, And, this);
		AddFunc("|", 1, Or, this);

		AddFunc("(", 0, LeftBracket, this);
		AddFunc(")", 0, RightBracket, this);

		AddFunc("\"", 0, ExpVar, this);
		AddFunc("'", 0, ExpVar, this);
	}

	~TExpCalc()
	{
	}

	enum
	{
		SYM_VAL = 0,
		SYM_MEM = 1,
		SYM_FUNC = 2,
	};

	struct SYM_EXP_ITEM
	{
		union
		{
			R Val;
			void *Mem;
			R (*Func)(const char *Str, unsigned long Len);
		};
		int Length;
		int Type;
	};

	map<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM> m_SymExpItem;

	void Reset()
	{
		m_SymExpItem.clear();
	}

	enum
	{
		EXP_RESULT	= 0,
		EXP_NEEDCALC	= 1,
		EXP_SYM		= 2,
		EXP_OPT		= 3,
	};

	struct EXPITEM
	{
		union
		{
			R Value;
			const char *StrValue;
			void *OptSymFunc;

		};
		int Type;
		int Length;
		const char *Str;
	};

	struct OPTSYMFUNC
	{
		TAnsiString OptStr;
		int Level;
		EXPITEM *(*OptFunc)(TListIter<EXPITEM> It, TList<EXPITEM> & Expressions, TExpCalc *This);
		TExpCalc *This;
	};

	TList<OPTSYMFUNC> m_OptFunc;

	void AddFunc(const char *OptStr, int Level, EXPITEM *(*OptFunc)(TListIter<EXPITEM> It, TList<EXPITEM> & Expressions, TExpCalc *This), TExpCalc *This)
	{
		int Len = TStrLen(OptStr);
		TListIter<OPTSYMFUNC> It = m_OptFunc.Begin();
		while (It != m_OptFunc.End())
		{
			if (It->Level > Level || (It->Level == Level && Len >= It->OptStr.m_nLength))
				break;
			++It;
		}
		It = m_OptFunc.InsertBefore(It);
		It->OptStr = OptStr;
		It->Level = Level;
		It->OptFunc = OptFunc;
		It->This = This;
	}

	OPTSYMFUNC * GetOptFunc(const char *OptStr) const
	{
		if (!TStrChr("`~!@#$%^&*()-+=|{}:;\"'<>,.?/\\'\"", OptStr[0]))
			return nullptr;

		for (int Len = 2; Len > 0; Len--)
		{
			TListIter<OPTSYMFUNC> It = m_OptFunc.Begin();
			while (It != m_OptFunc.End())
			{
				if (Len == It->OptStr.m_nLength &&
					!TStrNCmp(OptStr, It->OptStr.operator const char*(), It->OptStr.m_nLength))
				{
					//printf("%s:%d OP %s level %d\n",__func__,__LINE__, It->OptStr.operator const char*(), It->Level);
					return &*It;
				}
				++It;
			}
		}
		return nullptr;
	}

//
	bool InsertSym(const char *szSym, R Value)
	{
		typename map<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>::IT it = m_SymExpItem.insert(_Tmap_pair<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>(szSym,SYM_EXP_ITEM()));
		if (it == m_SymExpItem.end())
			return false;
		it->second.Val = Value;
		it->second.Type = SYM_VAL;
		return true;
	}
//
	bool InsertSym(const char *szSym, R *Mem, int Length)
	{
		typename map<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>::IT it = m_SymExpItem.insert(_Tmap_pair<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>(szSym,SYM_EXP_ITEM()));
		if (it == m_SymExpItem.end())
			return false;
		it->second.Mem = Mem;
		it->second.Length = Length;
		it->second.Type = SYM_MEM;
		return true;
	}

	bool UpdateSym(const char *szSym, R Mem)
	{
		typename map<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>::IT it = m_SymExpItem.find(szSym);
		if (it == m_SymExpItem.end())
			return false;
		if (it->second.Type != SYM_MEM)
			return false;
		memcpy(it->second.Mem, &Mem, it->second.Length);
		return true;
	}

	virtual bool GetValue(const char *szItem, R *pValue) const
	{
		if (USHexStrToNum(szItem, pValue))
		{
			//printf("%s:%d %s %d\n",__func__,__LINE__, szItem, *pValue);
			return true;
		}

		//printf("%s:%d CONTINUE\n",__func__,__LINE__);
		typename map<TString<T,NoCaseCmp,8>, SYM_EXP_ITEM>::const_iterator It = m_SymExpItem.find(szItem);

		if (It == m_SymExpItem.end())
			return false;
		
		if (It->second.Type == SYM_VAL)
		{
			*pValue = It->second.Val;
		} else
		if (It->second.Type == SYM_MEM)
		{
			*pValue = 0;
			TMemCpy((unsigned char*)pValue, (unsigned char*)It->second.Mem, It->second.Length);
		} else
		if (It->second.Type == SYM_FUNC)
		{
			*pValue = It->second.Func(It->first.operator const T*(), It->second.Length);
		}
		return true;
	}

	virtual R ReadValue(R Address)
	{
		R Value = 0;
		return Value;
	}
//
	virtual int ReadString(unsigned long Address, char *Buffer, int Length)
	{
		int nRet = 0;
		while (nRet<Length)
		{
			char Byte = ReadValue(Address+nRet*sizeof(char));
			if (Byte==0) break;
			Buffer[nRet++]=Byte;
		}
		Buffer[nRet]=0;
		return nRet;
	}
//
	virtual int ReadString(unsigned long Address, short *Buffer, int Length)
	{
		int nRet = 0;
		while (nRet<Length)
		{
			short Word = ReadValue(Address+nRet*sizeof(short));
			if (Word==0) break;
			Buffer[nRet++]=Word;
		}
		Buffer[nRet]=0;
		return nRet;
	}
//
	virtual void ReadMemory(R Address, void *Buffer, int Length)
	{
		int nRet = 0;
		while (nRet<Length)
		{
			((unsigned char*)Buffer)[nRet] = ReadValue(Address+nRet*sizeof(unsigned char));
			++nRet;
		}
	}
//
	bool ReadVar(EXPITEM *ExpItem) const
	{
		char Buffer[128];
		TStrCpyLimit(Buffer, ExpItem->Str, ExpItem->Length+1);
		if (GetValue(Buffer, &ExpItem->Value))
		{
			ExpItem->Type = EXP_RESULT;
			return true;
		}
		ExpItem->Type = EXP_NEEDCALC;
		return false;
	}

	bool BuildExpression(const char *ExpStr, TList<EXPITEM> & Expressions) const
	{
		EXPITEM *ExpItem = nullptr;
		int Len = TStrLen(ExpStr);

		int start = 0;
		int end = 0;

		while (start < Len)
		{
			const char *pCur = (const char *)&ExpStr[start];
			if (TStrChr("\r\n\t ", pCur[0]))
			{
				if (ExpItem)
				{
					ExpItem->Length = start - end;
					ReadVar(ExpItem);
					ExpItem = nullptr;
				}
			} else
			{
				OPTSYMFUNC *OptSymFunc = GetOptFunc(pCur);
				if (OptSymFunc)
				{
					if (ExpItem)
					{
						ExpItem->Length = start - end;
						ReadVar(ExpItem);
						ExpItem = nullptr;
					}

					if (pCur[0] == '"' || pCur[0] == '\'')
					{
						TListIter<EXPITEM> It = Expressions.Append();
						++start;
						It->Str = (char *)&ExpStr[start];
						It->StrValue = (char *)&ExpStr[start];
						end = start;
						while (ExpStr[start] &&
							ExpStr[start] != '"' &&
							ExpStr[start] != '\'')
							++start;
						It->Length = start - end;
						It->Type = EXP_SYM;
						//printf("%s:%d ADD SYM\n",__func__,__LINE__);
					} else
					{
						TListIter<EXPITEM> It = Expressions.Append();
						It->Str = pCur;
						It->Length = OptSymFunc->OptStr.m_nLength;
						It->OptSymFunc = OptSymFunc;
						It->Type = EXP_OPT;
						start += It->Length - 1;
						//printf("%s:%d ADD OPT\n",__func__,__LINE__);
					}
					end = start;
				} else
				if (ExpItem == nullptr)
				{
					TListIter<EXPITEM> It = Expressions.Append();
					ExpItem = &*It;
					ExpItem->Value = 0;
					ExpItem->Str = pCur;
					//ExpItem->Type = 0; //EXP_RESULT ???
					end = start;
					//printf("%s:%d ADD\n",__func__,__LINE__);
				}
				
			}
			++start;
		}

		if (ExpItem)
		{
			ExpItem->Length = start - end;
			ReadVar(ExpItem);
		}
		return true;
	}

	bool SolveExpression(TListIter<EXPITEM> Iter, TList<EXPITEM> & Expressions)// const
	{
		for (int level = 0; level < 6; ++level)
		{
			TListIter<EXPITEM> It = Iter;
			while (It != Expressions.End())
			{
				if (It->Type == EXP_OPT)
				{
					OPTSYMFUNC *OptSymFunc = (OPTSYMFUNC *)It->OptSymFunc;
					if (OptSymFunc->Level == level)
					{
						//printf("%s:%d %s level %d >>\n",__func__,__LINE__, OptSymFunc->OptStr.operator const char*(), level);
						EXPITEM *ExpItem = OptSymFunc->OptFunc(It, Expressions, this);
						if (ExpItem == nullptr) return false;
						//printf("%s:%d %s level %d Value %ld <<\n",__func__,__LINE__, OptSymFunc->OptStr.operator const char*(), level, ExpItem->Value);
						ExpItem->Type = EXP_RESULT;
						It = ExpItem; //???
					}
				}
				++It;
			}
		}
		return Expressions.Size() == 1 && Expressions.Begin()->Type == EXP_RESULT;
	}

	bool CalcExpression(const char *ExpStr, R *Result) //const
	{
		if (ExpStr == nullptr || ExpStr[0] == 0)
		{
			*Result = 0;
			return true;
		}
		
		TList<EXPITEM> Expressions;

		if (!BuildExpression(ExpStr, Expressions))
		{
			//printf("%s:%d BUILD FAIL\n",__func__,__LINE__);
			return false;
		}

		if (!SolveExpression(Expressions.Begin(), Expressions))
			return false;

		if (Result) *Result =  Expressions.Begin()->Value;
		return true;
	}

	static bool CheckResult(int LLevel, TListIter<EXPITEM> It, int RLevel, TList<EXPITEM> & Expressions)
	{
	#if 0
		TListIter<EXPITEM> Iter = It-1;
		int Index = 0;
		while (Index < LLevel)
		{
			if (Iter == Expressions.End() || Iter->Type!=EXP_RESULT)
				return false;
			--Iter;
			++Index;
		}

		//TListIter<EXPITEM>
		Iter = It+1;
		//int
		Index = 0;
		while (Index < LLevel)
		{
			if (Iter == Expressions.End() || Iter->Type!=EXP_RESULT)
				return false;
			++Iter;
			++Index;
		}
		return true;
	#else
		int PrevIndex = 0;
		if (LLevel <= 0)
		{
	AGAIN:
			TListIter<EXPITEM> NextIter = It+1;
			int NextIndex = 0;
			if (LLevel <= 0)
				return true;
			while (NextIter != Expressions.End() && NextIter->Type==EXP_RESULT)
			{
				++NextIter;
				++NextIndex;
				if (NextIndex>=LLevel)
					return true;
			}
		} else
		{
			TListIter<EXPITEM> PrevIter = It-1;
			while (PrevIter != Expressions.End() && PrevIter->Type==EXP_RESULT)
			{
				--PrevIter;
				++PrevIndex;
				if (PrevIndex>=LLevel)
				{
					goto AGAIN;
				}
			}
			
		}
		return false;
	#endif
	}

	static bool RemoveExpr(TListIter<EXPITEM> It, int Level, TList<EXPITEM> & Expressions)
	{
	#if 0
		TListIter<EXPITEM> Iter = It;
		int Index = 0;
		while (Index<Level)
		{
			if (Iter==Expressions.End()) return false;
			++Iter;
			++Index;
		}
		Expressions.Remove(It, Iter);
		return true;
	#else
		TListIter<EXPITEM> First = It;
		if (Level <= 0)
		{
			Expressions.Remove(First, First);//+1
			return true;
		}
		int Index = 0;
		TListIter<EXPITEM> Iter = It;
		while (Iter != Expressions.End())
		{
			++Iter;
			++Index;
			if (Level<=Index)
			{
				Expressions.Remove(First, Iter);
				return true;
			}
		}
		return false;
	#endif
	}

	static EXPITEM * LogicAnd(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value && It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * LogicOr(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value || It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * LogicXor(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			if (ExpItem->Value == 0)
			{
				if (It.Next()->Value == 1)
					ExpItem->Value = 1;
				else	ExpItem->Value = 0;
			} else
			if (ExpItem->Value == 1)
			{
				if (It.Next()->Value == 1)
					ExpItem->Value = 0;
				else	ExpItem->Value = 1;
			}
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}

	static EXPITEM * NotBelow(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value >= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * NotAbove(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value <= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Below(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value < It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Above(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value > It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}

	static EXPITEM * Equal(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
			{
				if (It-1 != Expression.End() && It+1 != Expression.End())
				{
					char Buffer[64];
					if (It.Prev()->Type == EXP_SYM && It.Next()->Type == EXP_RESULT)
					{
						pThis->ReadMemory(It.Next()->Value, Buffer, MIN(It.Prev()->Length*2,64));
						int r = !TStrNICmp((const char*)Buffer, (const char*)It.Prev()->StrValue, MIN(It.Prev()->Length,64)) ||
							!TStrNICmp((const short*)Buffer, (const short*)It.Prev()->StrValue, MIN(It.Prev()->Length,32));
						It.Prev()->Type = EXP_RESULT;
						It.Prev()->Value = r;
						RemoveExpr(It, 2, Expression);
						return &*It;
					} else
					if (It.Prev()->Type == EXP_RESULT && It.Next()->Type == EXP_SYM)
					{
						pThis->ReadMemory(It.Prev()->Value, Buffer, MIN(It.Next()->Length*2,64));
						int r = !TStrNICmp((const char*)Buffer, (const char*)It.Next()->StrValue, MIN(It.Next()->Length,64)) ||
							!TStrNICmp((const short*)Buffer, (const short*)It.Next()->StrValue, MIN(It.Next()->Length,32));
						It.Prev()->Type = EXP_RESULT;
						It.Prev()->Value = r;
						RemoveExpr(It, 2, Expression);
						return &*It;
					}
				}
				return nullptr;
			}
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value == It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * NotEqual(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value = ExpItem->Value != It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}

	static EXPITEM * ShiftRight(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value >>= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * ShiftLeft(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value <<= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}

	static EXPITEM * Add(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			//printf("%s:%d\n",__func__,__LINE__);
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			//printf("%s:%d %ld %ld Size %d\n",__func__,__LINE__, It.Prev()->Value, It.Next()->Value, Expression.Size());
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value += It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			//printf("%s:%d Size %d\n",__func__,__LINE__, Expression.Size());
			return ExpItem;
		}
	static EXPITEM * Sub(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (CheckResult(1, It, 1, Expression))
			{
				EXPITEM *ExpItem = It.Prev();
				ExpItem->Value = ExpItem->Value - It.Next()->Value;
				RemoveExpr(It, 2, Expression);
				return ExpItem;
			} else
			if (CheckResult(0, It, 1, Expression))
			{
				EXPITEM *ExpItem = &*It;
				ExpItem->Value = -It.Next()->Value;
				RemoveExpr(It+1, 1, Expression);
				return ExpItem;
			}
			return nullptr;
		}

	static EXPITEM * Mul(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (CheckResult(1, It, 1, Expression))
			{
				EXPITEM *ExpItem = It.Prev();
				ExpItem->Value = ExpItem->Value * It.Next()->Value;
				RemoveExpr(It, 2, Expression);
				return ExpItem;
			} else
			if (CheckResult(0, It, 1, Expression))
			{
				EXPITEM *ExpItem = &*It;
				ExpItem->Value = pThis->ReadValue(It.Next()->Value);
				RemoveExpr(It+1, 1, Expression);
				return ExpItem;
			}
			return nullptr;
		}
	static EXPITEM * Div(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			if (It.Next() == 0)
				return nullptr;
			ExpItem->Value /= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Mod(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value %= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}

	static EXPITEM * Xor(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value ^= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Not(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(0, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = &*It;
			ExpItem->Value = It.Next()->Value == 0;
			RemoveExpr(It+1, 1, Expression);
			return ExpItem;
		}
	static EXPITEM * And(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value &= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Or(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(1, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = It.Prev();
			ExpItem->Value |= It.Next()->Value;
			RemoveExpr(It, 2, Expression);
			return ExpItem;
		}
	static EXPITEM * Addr(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			if (!CheckResult(0, It, 1, Expression))
				return nullptr;
			EXPITEM *ExpItem = &*It;
			ExpItem->Value = pThis->ReadValue(It.Next()->Value);
			RemoveExpr(It+1, 1, Expression);
			return ExpItem;
		}

	static EXPITEM * LeftBracket(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{
			int level = 0;
			TListIter<EXPITEM> Iter = It+1;
			TListIter<EXPITEM> First = Iter;
			TListIter<EXPITEM> Last = First;
			while (Iter != Expression.End())
			{
				if (!TStrNCmp(Iter->Str, "(", Iter->Length))
					++level;
				if (!TStrNCmp(Iter->Str, ")", Iter->Length))
				{
					if (level==0) break;
					--level;
				}
				++Iter;
				Last = Iter;
			}
			if (Iter == Expression.End())
				return nullptr;

			TList<EXPITEM> SubExpressions;
			TListIter<EXPITEM> End = SubExpressions.End();
			Expression.Splice(First, Last, SubExpressions, End);
			if (!pThis->SolveExpression(SubExpressions.Begin(), SubExpressions))
				return nullptr;
			It->Value = SubExpressions.Begin()->Value;
			RemoveExpr(It+1, 1, Expression);
			return &*It;
		}
	static EXPITEM * RightBracket(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{ return nullptr; }

	static EXPITEM * ExpVar(TListIter<EXPITEM> It, TList<EXPITEM> & Expression, TExpCalc *pThis)
		{ return nullptr; }
};

#endif

//#define TEST
#ifdef TEST
#include "../../define.h"
//#include "altexpcalc.hpp"

#include <stdio.h>
#include <stdlib.h>

typedef TExpCalc<char, unsigned long, 1> ExpCalc;

int main(int argc, char **argv)
{
	ExpCalc calc;
	unsigned long Result = 0;
	printf("Calc %s\n", argv[1]);
	if (calc.CalcExpression(argv[1], &Result))
	{
		printf("Calc %s\n", argv[1]);
		printf("OK %ld %08x\n", Result, Result);
	} else
	{
		printf("Calc %s\n", argv[1]);
		printf("FAIL\n");
	}
return 0;
}
#endif
