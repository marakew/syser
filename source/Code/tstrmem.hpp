
#ifndef _TSTRMEM_HPP_
#define _TSTRMEM_HPP_

#include <stdarg.h>
#include "define.h"
#include "alt/altfile.hpp"

int UnicodeToAnsi(const WCHAR *WszBuf, char *szBuf, int MaxLen);
int AnsiToUnicode(const char *szBuf, WCHAR *WszBuf, int MaxLen);

bool UNormalize(char *InBuffer, unsigned long InLen, char *OutBuffer, unsigned long MaxOutLen, unsigned long *Len);
bool ANormalize(char *InBuffer, unsigned long InLen, char *OutBuffer, unsigned long MaxOutLen, unsigned long *Len);

	template<class T>
	inline T *TMemCpy(T *dst, const T *src, int count)
	{
		T *ret = dst;
		//if (dst && src) //???
		while (count) --count,*dst++ = *src++;
		return ret;
	}

	template<class T, class R>
	inline int TMemICmp(const T *dst, const R *src, int count)
	{
		char l = 0;//T l; //???
		char r = 0;//R r; //f;

		//if (dst && src) //???
		while (count && l == r)
		{
			--count;
			l = *dst++; MAKE_CHAR_LOWER(l);
			r = *src++; MAKE_CHAR_LOWER(r);
		}
		return l - r;
#if 0
		while (count && *dst && ((*dst|32) == (*src|32))) { --count; ++dst; ++src; }
		return *dst - *src;
#endif
	}

	//TAB

	template<class T>
	inline T *TMemSet(T *dst, T val, int count)
	{
		T *start = dst;
		//if (dst) //???
		while (count--) *start++ = val;
		return dst;
	}

	template<class T, class S>
	inline int TStrCpy(T *dst, const S *src)
	{
		int len = 0;
		T *cp;
		if (dst && src)	while ((*dst++=*src++) != (T)0) ++len; //copy \0
		return len;
	}

	template<class T>
	inline T *TStrNCpy(T *dst, const T *src, int count)
	{
		T *start = dst;
		if (dst && src && count > 0)
		{
			while ((*dst++=*src++) != (T)0 && count--); //copy \0
			while (count>0) *dst++ = 0,count--;
		}
		return start;
	}

	template<class T>
	inline int TStrLen(const T *str)
	{
		if (!str) return 0;
		const T *eos = str;
		while (*eos++);
		return eos-str-1;
	}

	template<class T, class S>
	inline T *TStrCat(T *dst, const S *src)
	{
		if (dst && src)
		{
			T *cp = dst; while (*cp) ++cp;
			while ((*cp++ = *src++) != (T)0);
		}
		return dst;
	}

	template<class T, class R>
	inline T *TStrChr(const T *string, R ch) //class R char
	{
		if (!string) return NULL;

	while (*string && *string != ch) string++;
//	return (T*)string;
	return *string == ch ? (T*)string : (T*)0;

//		while (*string) { if (*string == ch) return string; else ++string; }
//		return ch ? 0 : string;
	}

	template<class T>
	inline T *TStrIChr(const T *string, T ch)
	{
		if (!IS_LETTER(ch)) return TStrChr(string, ch);

		if (!string) return NULL;
		T LChar = LOWER_CHAR(ch);
		T UChar = UPPER_CHAR(ch);
	while (*string && (*string != LChar && *string != UChar)) string++;
//	while (*string && (*string != ch && *string != LChar)) string++;
//	return (T*)string;
	return *string == ch ? (T*)string : (T*)0;

//		while (*string) { if (*string == ch) return string; else ++string; }
//		return ch ? 0 : string;
	}

	template<class T>
	inline T *TStrRChr(const T *string, char ch)
	{
		T *start = (T*)string;
		while (*start++);
		while (--start != string && *start != ch);
		return *start == ch ? (T*)start : (T*)0;
	}

	template<class T>
	inline T *TStrRChr(const T *string, const T *start, char ch)
	{
	//	T *start = (T*)string;
	//	while (*start++);
		while (--start != string && *start != ch);
		return *start == ch ? (T*)start : (T*)0;
	}

	template<class T, class R>
	inline int TStrCmp(const T *dst, const R *src)
	{
	#if 1
		if (!dst || !src)
		{
			if (!dst && src) return -(R)*src;
			if (dst && !src) return (T)*dst;
			return 0;
		}
	#else
		if (!dst)
		{
			if (src) return -(R)*src;
			return 0;
		}
		if (!src) return *dst;
	#endif
		int ret = 0;
		while (*dst && (*dst == *src)) { ++dst; ++src; } //== |
		ret = *dst - *src;
		//return ret < 0 ? -1 : (ret > 0 ? 1 : 0);
		if (ret < 0) return -1;
		if (ret > 0) return 1;
		return 0;
	}

	template<class T>
	inline int TStrNCmp(const T *dst, const T *src, int count)
	{
		if (!count) return 0;
	#if 1
		if (!dst || !src)
		{
			if (!dst && src) return -(T)*src;
			if (dst && !src) return (T)*dst;
			return 0;
		}
	#else
		if (!dst)
		{
			if (src) return -(T)*src;
			return 0;
		}
		if (!src) return *dst;
	#endif
		int ret = 0;
		while (--count && *dst && (*dst == *src)) { ++dst; ++src; } //== |
		ret = *dst - *src;
		//return ret < 0 ? -1 : (ret > 0 ? 1 : 0);
		if (ret < 0) return -1;
		if (ret > 0) return 1;
		return 0;
	}

	template<class T, class R>
	inline int TStrICmp(const T *dst, const R *src)
	{
		char l;//T l;
		char r;//R r; //f;
	#if 1
		if (!dst || !src)
		{
			if (!dst && src) return -(R)*src;
			if (dst && !src) return (T)*dst;
			return 0;
		}
	#else
		if (!dst)
		{
			if (src) return -(T)*src;
			return 0;
		}
		if (!src) return *dst;
	#endif
#if 0
		while (*dst && ((*dst|32) == (*src|32))) { ++dst; ++src; }
		return *dst - *src;
#else
		do {
			l = *dst++; MAKE_CHAR_LOWER(l);
			r = *src++; MAKE_CHAR_LOWER(r);
		} while (l && l == r);
		return l - r;
#endif
	}

	template<class T, class R>
	inline int TStrNICmp(const T *dst, const R *src, int count) //TODO check cmp
	{
		char l;//T l; //???
		char r;//R r; //f;

		if (!count) return 0;
	#if 1
		if (!dst || !src)
		{
			if (!dst && src) return -(R)*src;
			if (dst && !src) return (T)*dst;
			return 0;
		}
	#else
		if (!dst)
		{
			if (src) return -(R)*src;
			return 0;
		}
		if (!src) return *dst;
	#endif
#if 0
		while (count && *dst && ((*dst|32) == (*src|32))) { --count; ++dst; ++src; }
		return *dst - *src;
#else
		do {
			l = *dst++; MAKE_CHAR_LOWER(l);
			r = *src++; MAKE_CHAR_LOWER(r);
		} while (--count && l && l == r);
		return l - r;
#endif
	}

	template<class T>
	inline T *TStrUpr(T *string)
	{
		if (!string) return NULL;
		for (T *cp = string; *cp; ++cp) MAKE_CHAR_UPPER(*cp); //TODO cast letter
		return string;
	}

	template<class T>
	inline T *TStrLwr(T *string)
	{
		if (!string) return NULL;
		for (T *cp = string; *cp; ++cp) MAKE_CHAR_LOWER(*cp); //TODO cast letter
		return string;
	}

	template<class T, class S>
	inline int TStrToStr(const T *Src, S *Des)
	{
		int Count = 0;
		if (Src && Des)
		{
			while (Src[Count]) Des[Count]=Src[Count],++Count;
			Des[Count] = 0;
		}
		return Count;
	}

	template<class T, class S>
	inline int TStrToPrint(const T *Src, S *Des, int Len)
	{
		int Count = 0;
		for (Count=0;Count<Len;++Count)
		{
			Des[Count]=Src[Count]<32?46:Src[Count];
		}
		Des[Count] = 0;
		return Count;
	}

	template<class T>
	inline int TStrFillTail(T *Str, int MaxLen, T FillChar)
	{
		int StrLen = TStrLen(Str);
		if (StrLen < MaxLen)
		{
			TMemSet(&Str[StrLen], FillChar, MaxLen - StrLen);
			Str[MaxLen] = 0;
			StrLen = MaxLen;
		}
		return StrLen;
	}

	template<class T, class S>
	inline int TStrCpyLimit(T *Des, const S *Src, int Limit)
	{
		int Len = 0;
		while (*Src && (Len < Limit-1)) *Des++ = *Src++, ++Len;
		*Des = 0;
		return Len;
	}

	inline int TStrOmit(const WCHAR *Src, WCHAR *Des, int MaxLen)
	{
		int i = MaxLen - 1;
		if (i <= 0 || !Src || !Des)
			return 0;

		int StrLen = TStrLen(Src);
		if (StrLen > i)
		{
			TStrCpyLimit(Des, Src, MaxLen);
			Des[i - 1] = '.';
			Des[i - 2] = '.';
			Des[i - 3] = '.';
			return i;
		}
		TStrCpy(Des, Src);
		return StrLen;
	}

	template<class T>
	bool ConvertFileNameByMacro(T *FileName, const T *szMacro, const T *szMacroStr)
	{
		int Len = TStrLen(szMacro);
		if (TStrNICmp(FileName, szMacro, Len)==0)
		{
			T szBuffer[MAX_FN_LEN];
			TStrCpy(szBuffer, &FileName[Len]);
			TStrCpy(FileName, szMacroStr);
			TStrCat(FileName, szBuffer);
			return true;
		}
		return false;
	}

	//TAB

	template<class T>
	inline T *TGetFileName(const T *szFullName)
	{
		if (!*szFullName) return NULL;
		T *pStr = TStrRChr(szFullName, '\\');
		if (!pStr) return (T*)szFullName;
		return pStr+1;
	}

	template<class T>
	inline T *TGetFileExt(const T *szFullName)
	{
		T *pFullName = TGetFileName(szFullName);
		if (!pFullName) return NULL;
		T *pStr = TStrRChr(pFullName, '.');
		if (!pStr) return NULL;
		++pStr;
		if (!*pStr) return NULL;
		return pStr;
	}

	//TAB

	template<class T>
	inline int TGetFileTitle(const T *szFullName, T *szTitle)
	{
		if (!szTitle) return 0;
		T *pStr = TGetFileName(szFullName);
		if (!pStr) { *szTitle = 0; return 0; }
		T *pDot = TStrRChr(pStr, '.');
		if (!pDot) return TStrCpy(szTitle, pStr);
		int Length = pDot - pStr;
		TMemCpy(szTitle, pStr, Length);
		szTitle[Length] = 0;
		return Length;
	}

	template<class T>
	inline int TGetFilePath(const T *szFullName, T *szPath)
	{
		T *pStr = TGetFileName(szFullName);
		if (!pStr) return TStrCpy(szPath, szFullName);
		int Length = pStr - szFullName;
		if (Length > 0) Length--;
		TMemCpy(szPath, szFullName, Length);
		szPath[Length] = 0;
		return Length;
	}

	template<class T, class S>
	inline bool TCmpModuleName(const T *FileName, const S *ModuleName)
	{
		T FileCmpName[512];
		S ModuleCmpName[512];

		if (TStrRChr(ModuleName, '\\'))
			return TStrICmp(FileName, ModuleName) == 0;
		T *pStr = TStrRChr(ModuleName, '.');
		if (pStr && !TStrRChr(FileName, '.'))
			pStr = NULL;
		if (pStr)
		{
			TStrCpy(FileCmpName, TGetFileName(FileName));
			TStrCpy(ModuleCmpName, TGetFileName(ModuleName));
		} else
		{
			TGetFileTitle(FileName, FileCmpName);
			TGetFileTitle(ModuleName, ModuleCmpName);
		}
		return TStrICmp(FileCmpName, ModuleCmpName) == 0;
	}

	template<class T, class S>
	T *uNumToStr(S Num, T *String, int Radix);

	template<class T, class S>
	T *NumToStr(S Num, T *String, int Radix);

	int TVSPrintf(char *buffer, const char *format, va_list ParamList);
	int TVSPrintf(WCHAR *buffer, const WCHAR *format, va_list ParamList);

	template<class T>
	inline int TSPrintf(T *buffer, const T *format, ...)
	{
		va_list next;
		va_start(next, format);
		int ret = TVSPrintf(buffer, format, next);
		va_end(next);
		return ret;
	}

#define ZEROPAD		(1<<0)	/* Pad with zero */
#define SIGN		(1<<1)	/* Unsigned/signed long */
#define PLUS		(1<<2)	/* Show plus */
#define SPACE		(1<<3)	/* Spacer */
#define LEFT		(1<<4)	/* Left justified */
#define HEX_PREP	(1<<5)	/* 0x */
#define UPPERCASE	(1<<6)	/* 'ABCDEF' */

#define HAS_FLOAT
#ifdef HAS_FLOAT
#if 1
	extern "C" double __cdecl modf(double,double*);
#else
	extern "C" {
	#include <math.h>
	}

	#ifdef _RING0_
	extern "C" double __cdecl _modf_default(double,double*);
	#define modf _modf_default
	#endif
#endif
	#include <float.h>
	//http://www.jbox.dk/sanos/source/lib/fcvt.c.html
	#define CVTBUFSIZE  2 * DBL_MAX_10_EXP + 10

	template<class T>
	T *cvt(double arg, int ndigits, int *decpt, int *sign, T *buf, int eflag)
	{
		int r2;
		double fi, fj;
		T *p, *p1;
 
		if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;
		r2 = 0;
		*sign = 0;
		p = &buf[0];
		if (arg < 0)
		{
			*sign = 1;
			arg = -arg;
		}
		arg = modf(arg, &fi);
		p1 = &buf[CVTBUFSIZE];
 
		if (fi != 0)
		{
			p1 = &buf[CVTBUFSIZE];
			while (fi != 0)
			{
				fj = modf(fi / 10, &fi);
				*--p1 = (int)((fj + .03) * 10) + '0';
				r2++;
			} while (p1 < &buf[CVTBUFSIZE]) *p++ = *p1++;
		} else
		if (arg > 0)
		{
			while ((fj = arg * 10) < 1)
			{
				arg = fj;
				r2--;
			}
		}
		p1 = &buf[ndigits];
		if (eflag == 0) p1 += r2;
		*decpt = r2;
		if (p1 < &buf[0])
		{
			buf[0] = '\0';
			return buf;
		}
		while (p <= p1 && p < &buf[CVTBUFSIZE])
		{
			arg *= 10;
			arg = modf(arg, &fj);
			*p++ = (int) fj + '0';
		}
		if (p1 >= &buf[CVTBUFSIZE])
		{
			buf[CVTBUFSIZE - 1] = '\0';
			return buf;
		}
		p = p1;
		*p1 += 5;
		while (*p1 > '9')
		{
			*p1 = '0';
			if (p1 > buf)
				++*--p1;
			else
			{
				*p1 = '1';
				(*decpt)++;
				if (eflag == 0)
				{
					if (p > buf) *p = '0';
					p++;
				}
			}
		}
		*p = '\0';
		return buf;
	}
 
	template<class T>
	T *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, T *buf)
	{
		return cvt(arg, ndigits, decpt, sign, buf, 1);
	}

	template<class T>
	T *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, T *buf)
	{
		return cvt(arg, ndigits, decpt, sign, buf, 0);
	}

	//TMemCpy
 	template<class T>
	void ee_bufcpy(T *pd, T *ps, int count) {
		T *pe=ps+count;
		while (ps!=pe)
			*pd++=*ps++;
	}

	template<class T>
	void parse_float(double value, T *buffer, T fmt, int precision)
	{
		int decpt, sign, exp, pos;
		T *fdigits = NULL;
		T cvtbuf[80];
		int capexp = 0;
		int magnitude;

		if (fmt == 'G' || fmt == 'E')
		{
			capexp = 1;
			fmt += (T)'a' - 'A';
		}

		if (fmt == 'g')
		{
			fdigits = ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
			magnitude = decpt - 1;
			if (magnitude < -4  ||  magnitude > precision - 1)
			{
				fmt = 'e';
				precision -= 1;
			} else
			{
				fmt = 'f';
				precision -= decpt;
			}
		}

		if (fmt == 'e')
		{
			fdigits = ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

			if (sign) *buffer++ = '-';
			*buffer++ = *fdigits;
			if (precision > 0) *buffer++ = '.';
			ee_bufcpy(buffer, fdigits + 1, precision); //TMemCpy(buffer, fdigits + 1, precision);
			buffer += precision;
			*buffer++ = capexp ? 'E' : 'e';

			if (decpt == 0)
			{
				if (value == 0.0)
					exp = 0;
				else
					exp = -1;
			} else
				exp = decpt - 1;

			if (exp < 0)
			{
				*buffer++ = '-';
				exp = -exp;
			} else
				*buffer++ = '+';

			buffer[2] = (exp % 10) + '0';
			exp = exp / 10;
			buffer[1] = (exp % 10) + '0';
			exp = exp / 10;
			buffer[0] = (exp % 10) + '0';
			buffer += 3;
		} else
		if (fmt == 'f')
		{
			fdigits = fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
			if (sign) *buffer++ = '-';
			if (*fdigits)
			{
				if (decpt <= 0)
				{
					*buffer++ = '0';
					*buffer++ = '.';
					for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
					while (*fdigits) *buffer++ = *fdigits++;
				} else
				{
					pos = 0;
					while (*fdigits)
					{
						if (pos++ == decpt) *buffer++ = '.';
						*buffer++ = *fdigits++;
					}
				}
			} else
			{
				*buffer++ = '0';
				if (precision > 0)
				{
					*buffer++ = '.';
					for (pos = 0; pos < precision; pos++) *buffer++ = '0';
				}
			}
		}

		*buffer = '\0';
	}

	template<class T>
	void decimal_point(T *buffer)
	{
		while (*buffer)
		{
			if (*buffer == '.') return;
			if (*buffer == 'e' || *buffer == 'E') break;
			buffer++;
		}

		if (*buffer)
		{
			int n = TStrLen(buffer);
			while (n > 0) 
			{
				buffer[n + 1] = buffer[n];
				n--;
			}

			*buffer = '.';
		} else
		{
			*buffer++ = '.';
			*buffer = '\0';
		}
	}

	template<class T>
	void cropzeros(T *buffer)
	{
		while (*buffer && *buffer != '.') buffer++;
		if (*buffer++)
		{
			while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
			T *stop = buffer--;
			while (*buffer == '0') buffer--;
			if (*buffer == '.') buffer--;
			//while (buffer!=stop)
			//	*++buffer=0;
			TStrCpy(stop, buffer);
		}
	}

	template<class T>
	T *flt(T *str, double num, int size, int precision, T fmt, int flags)
	{
		T tmp[80];
		//T c, sign;
		//int n, i;

		// Left align means no zero padding
		if (flags & LEFT) flags &= ~ZEROPAD;

		// Determine padding and sign char
		T c = (flags & ZEROPAD) ? '0' : ' ';
		T sign = 0;
		if (flags & SIGN)
		{
			if (num < 0.0)
			{
				sign = '-';
				num = -num;
				size--;
			} else
			if (flags & PLUS)
			{
				sign = '+';
				size--;
			} else
			if (flags & SPACE)
			{
				sign = ' ';
				size--;
			}
		}

		// Compute the precision value
		if (precision < 0)
			precision = 6; // Default precision: 6

		// Convert floating point number to text
		parse_float(num, tmp, fmt, precision);

		if ((flags & HEX_PREP) && precision == 0) decimal_point(tmp);
		if (fmt == 'g' && !(flags & HEX_PREP)) cropzeros(tmp);

		int n = TStrLen(tmp);

		// Output number with alignment and padding
		size -= n;
		if (!(flags & (ZEROPAD | LEFT)))
			while (size-- > 0) *str++ = ' '; //if (size>0) { TMemSet(str, ' ', size); str += size*2; }
		if (sign) *str++ = sign;
		if (!(flags & LEFT))
			while (size-- > 0) *str++ = c; //if (size>0) { TMemSet(str, c, size); str += size*2; }
		for (int i = 0; i < n; i++) *str++ = tmp[i];
		while (size-- > 0) *str++ = ' '; //if (size>0) { TMemSet(str, ' ', size); str += size*2; }

		return str;
	}
#endif

	//https://github.com/jpbonn/coremark_lm32/blob/master/ee_printf.c
	#include <stdarg.h>

	#define LITERAL(T,x) CString_traits<T>::choose(x, L##x)

	template<typename T>
	struct CString_traits
	{
		typedef char char_type;
		static const char *choose(const char *narrow, const WCHAR *wide) { return narrow; }
		//static char choose(char narrow, wchar_t wide) { return narrow; }
	};

	template<>
	struct CString_traits<WCHAR>
	{
		typedef WCHAR char_type;
		static const WCHAR *choose(const char *narrow, const WCHAR *wide) { return wide; }
		//static WCHAR choose(char narrow, wchar_t wide) { return wide; }
	};


	template<class T>
	inline int TVSPrintf_(T *buffer, const T *format, va_list Next)//ParamList)
	{
		//EmitNumber
		//EmitString
		//Done

		const T *str;
		bool bIsShort;
		static const T *strSub = LITERAL(T, "-");//[2];
		static const T *strSpace = LITERAL(T, " ");//[2];
		static const T *strZero = LITERAL(T, "0");//[2];
		int Precision;
		bool bLeftJustify;
		bool bIsLong;
		bool bMore;
		bool bPlusSign;
		T * savestr;
		T strbuf[1025]; //??? increase 36
		bool bZeroPrefix;
	//	va_list Next;
		static const T *str0X = LITERAL(T, "0X");//[3];
		unsigned long val;
		static const T *strPlus = LITERAL(T, "+");//[2];
		int Flags;
		T cPad;
		static const T *strNULL = LITERAL(T, "");//[1];
		int len;
		__int64 val64;
		bool bBlankSign;
		bool bIsLonglong;
		int Length;
		int Width;
		int nPad;
		static const T *strError = LITERAL(T, "<Error>");//[8];
		int nLeadingZeros;
		const T * sPrefix;
		static const T *str0x = LITERAL(T, "0x");//[3];

	#define OutPut(x,y)	(if (x) *x++ = *y;)

		//Next = ParamList;

		Length = 0;
		while (*format)
		{
			if (*format != '%')
			{
				if (buffer) *buffer++ = *format;
				++Length;
				++format;
				continue;
			}

			Width = 0;
			Precision = -1;
			bLeftJustify = false;
			bPlusSign = false;
			bBlankSign = false;
			bZeroPrefix = false;
			bIsLonglong = false;
			Flags = 0;
			cPad = 0x20;
			sPrefix = strNULL;

			format++;

			bool bMore = true;
			while (bMore)
			{
				switch (*format)
				{
				case ' ': /*Width = 0;*/ bBlankSign = true; format++; Flags |= SPACE; break;
				case '#': bZeroPrefix = true; format++; Flags |= HEX_PREP; break;
				case '+': bPlusSign = true; format++; Flags |= PLUS; break;
				case '-': /*Width = 0;*/ bLeftJustify = true; format++; Flags |= LEFT; break;
				case '0': cPad = '0'; format++; Flags |= ZEROPAD; break;
				default: bMore = false; break;
				}
			}

			if (*format == '*')
			{
				Width = va_arg(Next, int);
				format++;
				if (Width < 0)
				{
					Width = 0;
					bLeftJustify = true;
					Flags |= LEFT;
				}
			} else
			if (IS_DIGIT(*format))
			{
				while (IS_DIGIT(*format))
					Width = 10 * Width + *format++ - '0';
			}

			//switch TODO
			if (*format == '.')
			{
				format++;
				Precision = 0;
				if (*format == '*')
				{
					Precision = va_arg(Next, int);
					format++;
				} else
				{
					while (IS_DIGIT(*format))
						Precision = 10 * Precision + *format++ - '0';
				}
				if (Precision < 0) Precision = 0;
			}

			if (format[0] == 'I')
			{
				if (format[1] == '6' && format[2] == '4')
				{
					format += 3;
					bIsLonglong = true;
					
				}
			} else
			if (*format == 'h') ++format;
			else
			if (*format == 'l') ++format;

			switch (*format++)
			{
			case '%':
				strbuf[0] = '%';
				str = strbuf;
				len = 1;
				goto EmitString;
		#ifdef HAS_FLOAT
			case 'E':
			case 'G':
			case 'e':
			case 'f':
			case 'g': {
				double valDouble = va_arg(Next, double);

//unsigned long NaN[2] = {0xffffffff, 0x7fffffff}
//A quiet NaN is represented by any bit pattern between X'7FF80000 00000000' and X'7FFFFFFF FFFFFFFF' 
//or between X'FFF80000 00000000' and X'FFFFFFFF FFFFFFFF'.
//https://stackoverflow.com/questions/2249110/how-do-i-make-a-portable-isnan-isinf-function/2249173

				if (_fpclass(valDouble) == _FPCLASS_QNAN || _fpclass(valDouble) == _FPCLASS_SNAN)
				{
					TStrCpy(strbuf, ("Nan"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				} else
				if (_fpclass(valDouble) == _FPCLASS_NINF)
				{
					TStrCpy(strbuf, ("-Inf"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				} else
				if (_fpclass(valDouble) == _FPCLASS_PINF)
				{
					TStrCpy(strbuf, ("+Inf"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				}
				//_FPCLASS_NN
				//_FPCLASS_ND
				//_FPCLASS_NZ
				//_FPCLASS_PZ
				//_FPCLASS_PD
				//_FPCLASS_PN
				else {

				T *fstr = flt(strbuf, valDouble, Width, Precision, *(format-1), Flags | SIGN);
				*fstr = (T)0;
				str = strbuf;
				len = TStrLen(strbuf);
				goto EmitNumber; }}
		#endif
			case 'X':
				if (bZeroPrefix) sPrefix = str0X;
				if (bIsLonglong)
				{
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 16);
					if (!val64) sPrefix = strNULL;
				} else
				{
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 16);
					if (!val) sPrefix = strNULL;
				}
				TStrUpr(strbuf);
				goto EmitNumber;
			case 'c':
				strbuf[0] = (T)va_arg(Next, int); //??? char/WCHAR
				str = strbuf;
				len = 1;
				goto EmitString;
			case 'd':
			case 'i':
				Flags |= SIGN;
				if (bIsLonglong)
				{
					long long val64 = va_arg(Next, long long);
					NumToStr(val64, strbuf, 10);
				} else
				{
					long val = va_arg(Next, long);
					NumToStr(val, strbuf, 10);
				}
				if (strbuf[0] == '-') sPrefix = strSub;
				else
				if (bPlusSign) sPrefix = strPlus;
				else
				if (bBlankSign) sPrefix = strSpace;
				goto EmitNumber;
			case 'n':
				goto EmitNumber;
			case 'o':
				if (bIsLonglong)
				{
					//goto EmitLongLongNumber;
					unsigned long long val64 = va_arg(Next, unsigned long long);	
					NumToStr(val64, strbuf, 8); //??? 8/was 10
				} else
				{
					if (bZeroPrefix) sPrefix = strZero;
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 8);
					if (!val) sPrefix = strNULL;
				}
				goto EmitNumber;
			case 'p':
			case 'x':
				if (bZeroPrefix) sPrefix = str0x;
				if (bIsLonglong)
				{
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 16);
					if (!val64) sPrefix = strNULL;
				} else
				{
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 16);
					if (!val) sPrefix = strNULL;
				}
				TStrLwr(strbuf);
				goto EmitNumber;
			case 's':
				str = va_arg(Next, T*);
				len = TStrLen(str);
				if (Precision != -1 && Precision < len)
					len = Precision;
				goto EmitString;
			case 'u':
				if (bIsLonglong)
				{
//EmitLongLongNumber:
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 10);
				} else
				{	
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 10);		
				}
EmitNumber:
				if (Precision == -1) Precision = 1;
				savestr = strbuf;
				if (strbuf[0] == '-')
					savestr = &strbuf[1];
				len = TStrLen(savestr);
				nLeadingZeros = Precision - len;
				if (nLeadingZeros < 0)
					nLeadingZeros = 0;
				nPad = Width - (TStrLen(sPrefix) + nLeadingZeros + len);
				if (nPad < 0) nPad = 0;
				if (cPad == '0')
				{
					while (*sPrefix)
					{
						if (buffer) *buffer++ = *sPrefix++;
						++Length;
					}
				}
				if (nPad && !bLeftJustify)
				{
					while (nPad--)
					{
						if (buffer) *buffer++ = cPad;
						++Length;
					}
					nPad = 0;
				}
				while (*sPrefix)
				{
					if (buffer) *buffer++ = *sPrefix++;
					++Length;
				}
				while (nLeadingZeros--)
				{
					if (buffer) *buffer++ = 0x30;
					++Length;
				}
				while (len--)
				{
					if (buffer) *buffer++ = *savestr++;
					++Length;
				}
				if (nPad)
				{
					while (nPad--)
					{
						if (buffer) *buffer++ = 0x20;
						++Length;
					}
				}
				break;
			default:
				str = strError;
				len = TStrLen(str);
			EmitString:
				if (Width <= len)
					nPad = 0;
				else	nPad = Width - len;
				if (nPad && !bLeftJustify)
				{
					while (nPad--)
					{
						if (buffer) *buffer++ = cPad;
						++Length;
					}
				} 
				while (len--)
				{
					if (buffer) *buffer++ = *str++;
					++Length;
				}
				if (nPad && bLeftJustify)
				{
					while (nPad--)
					{
						if (buffer) *buffer++ = 0x20;
						++Length;
					}
				}
				break;
			}
		}

		if (buffer) *buffer = 0;
	#undef OutPut
		return Length;
	}

	//////////////////


//=====
	template<class T, class S>
	inline T *TStrStr(T *Str, const S *Sub)
	{
		if (!Str || !Sub) return NULL;
		if (!*Sub) return Str;
		while (*Str)
		{
			const S *Sub_ = Sub;
			const T *Str_ = Str;
			while (*Str_ && *Sub_)
			{
				if (*Str_ != *Sub_) break;
				++Sub_;
				++Str_;
			}
			if (!*Sub_) return Str;
			++Str;
		}
		return NULL;
	}

	template<class T, class S>
	inline T *TStrIStr(T *Str, const S *Sub)
	{
		if (!Str || !Sub) return NULL;
		if (!*Sub) return Str;
		while (*Str)
		{
			const S *Sub_ = Sub;
			const T *Str_ = Str;
			while (*Str_ && *Sub_)
			{
				if (LOWER_CHAR(*Str_) != LOWER_CHAR(*Sub_)) break;
				++Sub_;
				++Str_;
			}
			if (!*Sub_) return Str;
			++Str;
		}
		return NULL;
	}

	template<class T, class S>
	inline T *TStrIStrX(T *Str, const S *Sub, const S *NewSub)
	{
		if (!Str || !Sub) return NULL;
		if (!*Sub) return Str;

		if (!TStrNICmp(Str, NewSub, TStrLen(NewSub)))
		{
			const S *Sub_ = Sub;
			while (*Sub_)
			{
				if (!Str[TStrLen(NewSub)]) return Str;
				if (*Sub_ == Str[TStrLen(NewSub)]) return Str;
				++Sub_;
			}
		}

		while (*Str)
		{
			const S *Sub_ = Sub;
			while (*Sub_)
			{
				if (*Str == *Sub_ && !TStrNICmp(Str+1, NewSub, TStrLen(NewSub)) )
				{
					const S *Sub2 = Sub; //Sub_ ???
					while (*Sub2)
					{
						if (!Str[TStrLen(NewSub)+1]) return Str+1;
						if (*Sub2 == Str[TStrLen(NewSub)+1]) return Str+1;
						++Sub2;
					}
				}
				++Sub_;
			}
			++Str;
		}
		return NULL;
	}

	inline char *TStrFunc(char *pStr)
	{
		int Level = 0;
		char *pClose = TStrRChr(pStr, ')');
		int pCloseLen = TStrLen(pClose);
		int i = TStrLen(pStr) - pCloseLen; //WTF ??? just pCloseLen ? pCloseLen : TStrLen(pStr)
		while (i >= 0)
		{
			if (pStr[i] == ')')
			{
				++Level;
			} else
			if (pStr[i] == '(' )
			{
				--Level;
			}
			if (!Level)
				break;
			--i;
		}
		//if (Level) return nullptr; //+++ ???
		return &pStr[i];
	}

template<class T, int NoCaseCmp, int N>
struct TString
{
	T *m_pData;
	int m_nLength;
	int m_nMaxLength;
public:
	void Init() { m_pData = NULL; m_nLength = 0; m_nMaxLength = 0; }

	TString(const TString & String)
	{
		Init();
		if (String.m_nLength) Preallocate(String.m_nLength+1, String.m_pData);
	}

	TString(const T *pString)
	{
		Init();
		m_pData = (T*)pString; //TODO const cast
		m_nLength = TStrLen(pString);
	}

	TString() { Init(); }
	virtual ~TString() { Empty(); }

	virtual void Preallocate(int nLength, const T *pNewString)
	{
		T *pNewData = m_pData;

		int OldLen = m_nMaxLength;
		if (nLength > m_nMaxLength)
		{
			m_nMaxLength = ROUNDNEXT(nLength, N);
			pNewData = new T[m_nMaxLength]; //???
			*pNewData = 0;
		}
		if (pNewString && pNewString != pNewData)
		{
			m_nLength = TStrLen(pNewString);
			TStrCpyLimit(pNewData, pNewString, m_nMaxLength);
		}
		if (m_pData && pNewData != m_pData)
		{
			if (OldLen)
				delete []m_pData;
			m_pData = NULL;
		}
		m_pData = pNewData;
	}

	int GetLength() const { return m_nLength; }

	virtual void Empty()
	{
		if (m_pData) { if (m_nMaxLength) delete []m_pData; m_pData = NULL; } 
		m_nLength = 0;
		m_nMaxLength = 0;
	}
	bool IsEmpty() const { return !m_pData || !m_nLength || !m_pData[0]; }
	int Compare(const TString StrObj) const { return TStrCmp(m_pData, StrObj.m_pData); }
	int CompareNoCase(const TString StrObj) const { return TStrICmp(m_pData, StrObj.m_pData); }
	T GetAt(int nIndex) const { return m_pData[nIndex]; }
	T SetAt(int nIndex, T Char)
	{
		if (!m_pData) Preallocate(m_nLength+1, NULL);
		m_pData[nIndex] = Char;
		if (!Char) m_nLength = TStrLen(m_pData);
		return Char;
	}

	void Truncate(int nLength)
	{
		if (!m_pData) Preallocate(nLength+1, NULL);
		m_pData[nLength] = 0;
		if (m_pData) m_nLength = TStrLen(m_pData);
		else m_nLength = 0;
		//return m_nLength;
	}

	void ReverseCpy(T *dst, const T *src, int count)
	{
		dst += count;
		src += count;
		while(count) dst--,src--,*dst=*src,count--;
	}

	TString & Fill(T Char, int nCount)
	{
		Preallocate(nCount+1, NULL);
		for (int i = 0; i < nCount; ++i)
		{
			m_pData[i] = Char;
			++m_nLength;
		}
		m_pData[m_nLength] = 0;
		return *this;
	}

	int Insert(int nIndex, T Char)
	{
		if (!m_pData) Preallocate(nIndex+1, NULL);
		int v4 = m_nLength + 1;
		if (v4 >= m_nMaxLength) Preallocate(v4+1, m_pData);
		ReverseCpy(&m_pData[nIndex + 1], &m_pData[nIndex], m_nLength - nIndex);
		m_pData[nIndex] = Char;
		m_nLength += 1;
		m_pData[m_nLength] = 0;
		return m_nLength;
	}

	int Insert(int nIndex, const T *pString)
	{
		if (!m_pData) Preallocate(nIndex+1, NULL);
		int newLength = TStrLen(pString);

		int v7 = m_nLength + newLength;
		if (v7 >= m_nMaxLength)	Preallocate(v7+1, m_pData);
		ReverseCpy(&m_pData[nIndex + newLength], &m_pData[nIndex], m_nLength - nIndex);
		ReverseCpy(&m_pData[nIndex], pString, newLength);
		m_nLength += newLength;
		m_pData[m_nLength] = 0;
		return m_nLength;
	}

	int Delete(int nIndex, int nCount)
	{
		if (nIndex < 0 || nIndex >= m_nLength)
			return m_nLength;
		
		int v5 = MIN(nCount, m_nLength-nIndex);
		TMemCpy(&m_pData[nIndex], &m_pData[nIndex + v5], m_nLength - nIndex - v5);
		m_nLength -= v5;
		m_pData[m_nLength] = 0;
		return m_nLength;
	}

	TString & operator=(const T *pString)
	{
		if (pString) Preallocate(TStrLen(pString)+1, pString);
		else Empty();
		return *this;
	}

	TString & operator=(const TString & String)
	{
		if (String.m_nLength) Preallocate(String.m_nLength+1, String.m_pData);
		else Empty();
		return *this;
	}

	TString & operator+=(T Char)
	{
		Preallocate(m_nLength+2, m_pData); //??? 2
		m_pData[m_nLength++] = Char;
		m_pData[m_nLength] = 0;
		return *this;
	}

	TString & operator+=(const TString & String)
	{
		Preallocate(m_nLength+String.m_nLength+1, m_pData);
		TStrCat(m_pData, String.m_pData);
		m_nLength += String.m_nLength;
		return *this;
	}

	TString & operator+=(const T *pString)
	{
		int Length = TStrLen(pString);
		Preallocate(m_nLength+Length+1, m_pData);
		TStrCat(m_pData, pString);
		m_nLength += Length;
		return *this;
	}

	const T *cstr() const
	{
		static const T NullChar = 0;
		if (m_pData) return m_pData;
		return &NullChar;
	}

	operator const T *() const
	{
		static const T NullChar = 0;
		if (m_pData) return m_pData;
		return &NullChar;
	}

	T *data()// const
	{
		static T NullChar = 0;
		if (m_pData) return m_pData;
		return &NullChar;
	}

	TString operator+(const T *pNewString) const
	{
		TString tmp = *this;
		tmp += pNewString;
		return tmp;
	}

	TString operator+(const TString & String) const
	{
		TString tmp = *this;
		tmp += String;
		return tmp;
	}

	T & operator[](int nIndex) { return &m_pData[nIndex]; }

	bool operator==(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)==0;
		return TStrCmp(m_pData, pString)==0;
	}
	bool operator==(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)==0;
		return TStrCmp(m_pData, StrObj.m_pData)==0;
	}

	bool operator!=(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)!=0;
		return TStrCmp(m_pData, pString)!=0;
	}
	bool operator!=(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)!=0;
		return TStrCmp(m_pData, StrObj.m_pData)!=0;
	}

	bool operator<(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)<0;
		return TStrCmp(m_pData, pString)<0;
	}
	bool operator<(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)<0;
		return TStrCmp(m_pData, StrObj.m_pData)<0;
	}

	bool operator>(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)>0;
		return TStrCmp(m_pData, pString)>0;
	}
	bool operator>(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)>0;
		return TStrCmp(m_pData, StrObj.m_pData)>0;
	}

	bool operator<=(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)<=0;
		return TStrCmp(m_pData, pString)<=0;
	}
	bool operator<=(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)<=0;
		return TStrCmp(m_pData, StrObj.m_pData)<=0;
	}

	bool operator>=(const T *pString) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, pString)>=0;
		return TStrCmp(m_pData, pString)>=0;
	}
	bool operator>=(const TString & StrObj) const
	{
		if (NoCaseCmp)
			return TStrICmp(m_pData, StrObj.m_pData)>=0;
		return TStrCmp(m_pData, StrObj.m_pData)>=0;
	}

	void FormatV(const T *format, va_list Next)
	{
		int Len = TVSPrintf((T*)NULL, format, Next);
		Preallocate(Len+1, NULL);
		TVSPrintf(m_pData, format, Next);
		m_nLength = Len;
	}

	void Format(const T *format, ...)
	{
		va_list Next;
		va_start(Next, format);
		FormatV(format, Next);
		va_end(Next);
	}

	TString & MakeUpper() { if (m_pData) TStrUpr(m_pData); return *this; }
	TString & MakeLower() { if (m_pData) TStrLwr(m_pData); return *this; }

	T * GetFileName() const { return TGetFileName(operator T*()); }
	T * GetFileExt() const { return TGetFileExt(operator T*()); }
	int GetFileTitle(T *pStr) const { return TGetFileTitle(operator T*(), pStr); }
	int GetFilePath(T *pStr) const { return TGetFilePath(operator T*(), pStr); }

	int Write(ALT::ALTFileStream & Stream) const
	{
		int nRet = Stream.Write(&m_nLength, sizeof(m_nLength));
		if  (m_nLength>0)
			nRet += Stream.Write(m_pData, m_nLength);
		return nRet;
	}

	int Read(ALT::ALTFileStream & Stream)
	{
		if (Stream.Read(&m_nLength, sizeof(m_nLength)) != sizeof(m_nLength))
			return 0;
		if (m_nLength==0)
			return 4;
		Preallocate(m_nLength+1, NULL);
		int nRet = Stream.Read(m_pData, m_nLength);
		if (nRet != m_nLength)
			return 0;
		m_pData[m_nLength] = 0;
		return nRet + sizeof(m_nLength);
	}
};

	typedef TString<unsigned long,0,8> TColorString;
	typedef TString<WCHAR,0,8> TWideString;
	typedef TString<char,0,8> TAnsiString;

	typedef TString<WCHAR,1,8> TWideNoCaseString;
	typedef TString<char,1,8> TAnsiNoCaseString;

	struct MY_char_compare
	{
		bool operator()(const char *s1, const char *s2) const
		{
			return TStrCmp(s1, s2) < 0;
		}
	};

	template<typename S, typename T>
	T *THexBytesToStr(S *Hex, T *String, int nHex, int Length, const T *szSpc)
	{
		//S Data;
		//int LeftLen;
		//T *pStr;
		//S tmp;
		//int Turn;
		//int StrLen;
		int n;

		int Turn = 2 * Length;
		int StrLen = 0;
		if (szSpc)
		{
			StrLen = TStrLen(szSpc);
			Turn += StrLen;
		}
		for (n = 0; n < nHex / Length; ++n)
		{
			T *pStr = &String[(Turn * n + 2 * Length)];
			if (szSpc)
				TStrCpy(pStr, szSpc);
			for (int m = 0; m < Length; ++m)
			{
				S Data = Hex[m + Length * n];
				S tmp = Data & 0xf; // % 16
				pStr--,*pStr = tmp > 9 ? tmp+'7':tmp+'0';
				tmp = Data >> 4; // /16
				pStr--,*pStr = tmp > 9 ? tmp+'7':tmp+'0';
			}
			
		}
		int LeftLen = nHex % Length;
		if (LeftLen)
		{
			T *pStr = &String[(StrLen * n + 2 * LeftLen)];

			for (int m = 0; m < LeftLen; ++m)
			{
				S Data = Hex[m + Length * n];
				S tmp = Data & 0xf; // % 16
				pStr--,*pStr = tmp > 9 ? tmp+'7':tmp+'0';
				tmp = Data >> 4; // /16
				pStr--,*pStr = tmp > 9 ? tmp+'7':tmp+'0';
			}
			StrLen = 0;
		}
		String[(Turn * n -StrLen + 2 * LeftLen)] = 0;
		return &String[(Turn * n -StrLen + 2 * LeftLen)];
	}

	//ushort,char
	template<typename S, typename T>
	int THexBytesToASCII(S *Hex, T *String, int nHex)
	{
		T LOW,HI;
		int n = 0;
		while (Hex[0] && n < nHex)
		{
			if (Hex[1]>= 0x30 && Hex[1]<= 0x39) LOW = Hex[1]-0x30;
			else
			if (Hex[1]>= 0x41 && Hex[1]<= 0x46) LOW = Hex[1]-0x37;
			else
			if (Hex[1]>= 0x61 && Hex[1]<= 0x66) LOW = Hex[1]-0x57;
			else return 0;
			if (Hex[0]>= 0x30 && Hex[0]<= 0x39) HI = Hex[0]-0x30;
			else
			if (Hex[0]>= 0x41 && Hex[0]<= 0x46) HI = Hex[0]-0x37;
			else
			if (Hex[0]>= 0x61 && Hex[0]<= 0x66) HI = Hex[0]-0x57;
			else return 0;
			Hex += 2;
			String[n] = (HI<<4) + LOW;
			if (Hex[0] == 0x20 || Hex[0] == 0x2c) ++Hex;
			++n;
		}
		return n;
	}

	//ushort,char
	template<typename S, typename T>
	int THexBytesToPrint(S *Hex, T *String, int Count)
	{
		int i;
		for (i = 0; i < Count; ++i)
		{
			String[i] = Hex[i];
			if (String[i] < 32) //' '
				String[i] = 46; //','
		}
		String[i] = 0;
		return i;
	}

	//TAB
	inline unsigned long TGetCodeID(const unsigned char *Buffer, int Len)
	{
		unsigned short HighWord = 0;
		unsigned short LowWord = 0;
		for (int n = 0; n < Len; ++n)
		{
			LowWord += Buffer[n];
			if (n+2 <= Len)
				HighWord += *(unsigned short*)&(Buffer[n]);
		}
		return MAKE_DWORD(HighWord, LowWord);
	}


	inline unsigned long TGetNameCode(const char *Name, int Len) //??? unsigned
	{
		unsigned short Word = 0;
		unsigned char Byte = 0;
		for (int n = 0; n < Len; ++n)
		{
			Byte += Name[n];
			Word += Name[n];
		}
		return MAKE_DWORD(Word, MAKE_WORD(Byte, Len));
	//	return ((unsigned long)Word<<16) | (((unsigned short)Byte << 8) | Len);
	}

	inline unsigned long TGetNameCode(const char *Name) //??? unsigned
	{
		return TGetNameCode(Name, TStrLen(Name));
	}

	//TAB
	template<class T>
	inline bool TIMatchWithPattern(const T *Pattern, const T *Name)
	{
		if (Pattern[0] == 0) return true;
		if (Pattern[0] == '*')
		{
			++Pattern;
			if (Pattern[0] == 0) return true;
			while (Name[0])
			{
				if ((LOWER_CHAR(Pattern[0]) == LOWER_CHAR(Name[0]) || Pattern[0] == '?') &&
					TIMatchWithPattern(&Pattern[1], &Name[1]))
					return true;
				++Name;
			}
		} else
		{
			while (Name[0] && Pattern[0] != '*')
			{
				if (LOWER_CHAR(Pattern[0]) != LOWER_CHAR(Name[0]) && Pattern[0] != '?')
					return false;
				++Name;
				++Pattern;
			}
			if (Name[0]) return TIMatchWithPattern(Pattern, Name);
		}
		return Pattern[0] == 0 || Pattern[0] == '*';
	}

	template<class T>
	inline bool TIMultiMatchWithPattern(const T *Pattern, const T *Name)
	{
		T buf[256];
		//bool Result;
		//T *pEnd;
		//T *pBegin;

		if (Pattern[0] == 0) return true;
		TStrCpy(buf, Pattern);

		T *pEnd = &buf[TStrLen(buf)-1];
		if (*pEnd == ';') *pEnd = 0;

		T *pBegin = buf;
		while ((pEnd = TStrChr(pBegin, ';')))
		{
			*pEnd = 0;
			if (TIMatchWithPattern(pBegin, Name))
				return true;
			pBegin++;
		}
		return TIMatchWithPattern(pBegin, Name);
	}

	//TAB

	template<class T, class S>
	inline T *uNumToStr(S Num, T *String, int Radix)
	{
		if (Num == 0)
		{
			String[0] = '0';
			String[1] = 0;
			return String;
		}

		if (Radix < 2 || Radix > 36)
		{
			String[0] = 0;
			return String;
		}

		S PrevPow = 0;
		S Pow = 1;
		while (Num >= Pow && (!(Pow%Radix) || Pow == 1) && Pow > PrevPow)
		{
				PrevPow = Pow;
				Pow *= Radix;
		}

		S Digit = PrevPow;
		while (Digit)
		{
			*String++ = (Num/Digit)>9?(Num/Digit)+'7':(Num/Digit)+'0';
			Num -= (Num/Digit) * Digit;
			Digit /= Radix;
		}
		*String = 0;
		return String;
	}

	template<class T, class S>
	inline T *NumToStr(S Num, T *String, int Radix)
	{
		if (Num == 0)
		{
			String[0] = '0';
			String[1] = 0;
			return String;
		}

		if (Radix < 2 || Radix > 36)
		{
			String[0] = 0;
			return String;
		}

		if (Num < 0)
		{
			*String++ = '-';
			Num = -Num;
		}

		S PrevPow = 0;
		S Pow = 1;
		while (Num >= Pow && (!(Pow%Radix) || Pow == 1) && Pow > PrevPow)
		{
				PrevPow = Pow;
				Pow *= Radix;
		}

		S Digit = PrevPow;
		while (Digit)
		{
			*String++ = (Num/Digit)>9?(Num/Digit)+'7':(Num/Digit)+'0';
			Num -= (Num/Digit) * Digit;
			Digit /= Radix;
		}
		*String = 0;
		return String;
	}

	template<class T, class S>
	inline bool USHexStrToNum(const T *String, S *USHex)
	{
		S Result = 0;
		S Len = 0; //TStrLen(String);
		while(String[Len]) ++Len;
		if (Len == 0) return false;
		if (Len > sizeof(S)*2) return false;
		for (S n = 0; n < Len; ++n)
		{
			S Num = 0;
			T Char = String[Len-n-1];
			if (Char >= (T)'0' && Char <= (T)'9') Num = (S)Char-(S)'0';
			else
			if (Char >= (T)'A' && Char <= (T)'F') Num = (S)Char-(S)'7'; //-'A'+10
			else
			if (Char >= (T)'a' && Char <= (T)'f') Num = (S)Char-(S)'W'; //-'a'+10
			else return false;
			//printf("%s:%d C %c Num %ld n %d Len %d\n",__func__,__LINE__, Char,Num, n,Len);
			Result |= Num<<(n*4);
		}
		if (USHex) *USHex = Result;
		return true;
	}

	template<class T, class S>
	inline bool SHexStrToNum(const T *String, S *SHex)
	{
		S tmp;

		if (*String == '-')
		{
			tmp = -1;
			++String;
		} else
		{
			if (*String == '+')
			{
				++String;
			}
			tmp = 1;  
		}

		S Result = 0;
		S Len = 0; //TStrLen(String);
		while(String[Len]) ++Len;
		if (Len == 0) return false;
		if (Len > sizeof(S)*2) return false;
		for (S n = 0; n < Len; ++n)
		{
			S Num = 0;
			T Char = String[Len-n-1];
			if (Char >= (T)'0' && Char <= (T)'9') Num = (S)Char-(S)'0';
			else
			if (Char >= (T)'A' && Char <= (T)'F') Num = (S)Char-(S)'7'; //-'A'+10
			else
			if (Char >= (T)'a' && Char <= (T)'f') Num = (S)Char-(S)'W'; //-'a'+10
			else return false;
			//printf("%s:%d C %c Num %ld n %d Len %d\n",__func__,__LINE__, Char,Num, n,Len);
			Result |= Num<<(n*4);
		}
		if (SHex) *SHex = tmp * Result;
		return true;
	}

	template<class T, class S>
	inline bool USDecStrToNum(const T *String, S *USDec)
	{
		S Result = 0;
		S Len = 0; //TStrLen(String);
		while (String[Len]) ++Len;
		if (Len == 0) return false;
		S X10Base = 1;
		for (S n = 0; n < Len; ++n)
		{
			T c = String[Len-n-1];
			if (!IS_DIGIT(c)) return false;
			Result += X10Base * (c-'0');
			X10Base *= 10;
		}
		if (USDec) *USDec = Result;
		return true;
	}

	template<class T, class S>
	inline bool SDecStrToNum(const T *String, S *SDec)
	{
		S tmp;

		if (*String == '-')
		{
			tmp = -1;
			++String;
		} else
		{
			if (*String == '+')
			{
				++String;
			}
			tmp = 1;  
		}

		S Result = 0;
		S Len = 0;//TStrLen(String);
		while (String[Len]) ++Len;
		if (Len == 0) return false;
		S X10Base = 1;
		for (S n = 0; n < Len; ++n)
		{
			T c = String[Len-n-1];
			if (!IS_DIGIT(c)) return false;
			Result += X10Base * (c-'0');
			X10Base *= 10;
		}
		if (SDec) *SDec = tmp * Result;
		return true;
	}

	template <typename T, int N = 256>
	struct TStaticString
	{
		T Buffer[N];
		T *pBuffer;

		TStaticString()
		{
			pBuffer = Buffer;
			
		}
		TStaticString(int Len)
		{
			pBuffer = Buffer;
			if (Len >= N)
				pBuffer = new T[Len];
			*pBuffer = 0; //+++
		}
		template <typename I>
		TStaticString(const I *Str)
		{
			pBuffer = Buffer;
			int Len = TStrLen(Str)+1;
			if (Len >= N)
				pBuffer = new T[Len];
			*pBuffer = 0; //+++
			TStrCpy(pBuffer, Str);
		}
		~TStaticString()
		{
			if (pBuffer != Buffer) delete []pBuffer;
		}
		T *data() const { return pBuffer; }
		operator T *() const { return pBuffer; }
	};

	typedef TStaticString<WCHAR,256> TStaticWideString;
	typedef TStaticString<char,256> TStaticAnsiString;
#endif
