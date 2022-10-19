
#ifndef _DEFINE_H_
#define _DEFINE_H_

#ifdef _X64_
typedef unsigned long long ADDR;
typedef signed long long SADDR;
#define ADDR_SIZE 16
#define ADDR_BYTES 8
#define F0ADDR "%016I64"
#define FADDR "%I64"
//#define ADDRFMT "%016I64X"
//#define HEXADDRFMT "0x%016I64X"
#define HEXFMT "%I64X"
#define DECFMT "%I64d"
#define UNREADSTR "????????????????"
#define CIPSTR "RIP"
#define CSPSTR "RSP"
#define CBPSTR "RBP"
#define Eip Rip
#define STACKREG 4 //ESP_IDX
#define CODE_MODE CODE_MODE_64
//  "0%I64X"
//   "%I64X"
#else
typedef unsigned long ADDR;
typedef signed long SADDR;
#define ADDR_SIZE 8
#define ADDR_BYTES 4
#define F0ADDR "%08"
#define FADDR "%"
//#define ADDRFMT "%08X"
//#define HEXADDRFMT "0x%08X"
#define HEXFMT "%X"
#define DECFMT "%d"
#define UNREADSTR "????????"
#define CIPSTR "EIP"
#define CSPSTR "ESP"
#define CBPSTR "EBP"
#define STACKREG 5 //EBP_IDX
#define CODE_MODE CODE_MODE_32
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

//#ifdef WIN32
//#ifndef CODE_OS_NT_DRV
//#define CODE_OS_WIN

#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0501

//#define WIN32_LEAN_AND_MEAN

//#include <winresrc.h>
//#include <winuser.h>

#include <windows.h>
#include <windowsx.h>
#include <winnt.h>

//suxx
#if 1
#undef CreateFile
#undef DeleteFile
#undef FindFirstFile
#undef FindNextFile
#undef GetOpenFileName
#undef SendMessage
#undef PostMessage
#undef DispatchMessage
#undef GetCharWidth
#undef SetWindowText
#undef AppendMenu
#undef MoveMemory
#endif

//#endif
//#else   //WIN32
//#ifndef CODE_OS_CATOS
//#define CODE_OS_UNIX
//#endif
//#endif

#ifndef NULL
#define NULL			0
#endif

#ifndef TRUE
#define TRUE			1
#endif

#ifndef FALSE
#define FALSE			0
#endif

#define ERROR_USCODE		-1
#define PI			3.141592653549
#define MAX_FN_LEN		260
#define MAX_SHORT_NAME_LEN	32
#define MAX_NAME_LEN		64
#define MAX_LONG_NAME_LEN	128

#ifndef MAX_PATH
#define MAX_PATH		260
#endif

#define MAX_BYTE		0xFF
#define MAX_WORD		0xFFFF
#define MAX_DWORD		0xFFFFFFFF

#define IN
#define OUT
#define INOUT
#define OPTIONAL

#ifdef	CODE_OS_UNIX
#define PATH_SEPARATOR_CHAR	'/'
#define PATH_SEPARATOR_STR	"/"
#define PATH_SEPARATOR_WCHAR	L'/'
#define PATH_SEPARATOR_WSTR	L"/"
#else
#define PATH_SEPARATOR_CHAR	'\\'
#define PATH_SEPARATOR_STR	"\\"
#define PATH_SEPARATOR_WCHAR	L'\\'
#define PATH_SEPARATOR_WSTR	L"\\"
#endif

#define WSTR(s)			(L##s)

#ifdef _MSC_VER

typedef __int64			LONGLONG;
typedef unsigned __int64	ULONGLONG;
typedef unsigned __int64	NUM_PTR;

#ifndef _WCHAR_T_DEFINED
typedef unsigned short		wchar_t;
#define _WCHAR_T_DEFINED
#endif

#else //_MSC_VER

typedef long long		LONGLONG;
typedef unsigned long long	ULONGLONG;
typedef unsigned long long	NUM_PTR;

#endif //_MSC_VER

typedef ULONGLONG		QWORD;
typedef unsigned long		DWORD;
typedef unsigned short		WORD;
typedef unsigned char		BYTE;

typedef long			LONG;
typedef int			INT;
typedef short			SHORT;
typedef char			CHAR;
typedef unsigned long		ULONG;
typedef unsigned int		UINT;
typedef unsigned short		USHORT;
typedef unsigned char		UCHAR;
typedef int			BOOL;
typedef wchar_t			WCHAR;
typedef void*			HANDLE;

typedef DWORD			COLOR;
#define GetRColor(x)		(BYTE)((x)>>16)
#define GetGColor(x)		(BYTE)((x)>> 8)
#define GetBColor(x)		(BYTE)((x)>> 0)
#define RGBColor(r,g,b)		(((((DWORD)(r))<<16) | (((DWORD)(g))<<8) | ((DWORD)(b))<<0))

//
#define MAKE_WORD(HB,LB)		((WORD)(  (WORD)(LB) | ((WORD)(HB)<<8)   )) 
#define MAKE_DWORD(HW,LW)		((DWORD)(  (DWORD)(LW) | ((DWORD)(HW)<<16)  ))
#define MAKE_QDWORD(HD,LD)		((ULONGLONG)( (ULONGLONG)(LD) | ((ULONGLONG)(HD)<<32) ))

#define GET_BIT(Ptr,BitOff)		((RT_BYTE(Ptr,((BitOff)>>3)) >>((BitOff)&7)) & 1)
#define SET_BIT(Ptr,BitOff)		RT_BYTE(Ptr,((BitOff)>>3))|=1<<((BitOff)&7)
#define CLR_BIT(Ptr,BitOff)		RT_BYTE(Ptr,((BitOff)>>3))&=~(1<<((BitOff)&7))

#define PTR_TO_NUM(Ptr)			((unsigned int)(NUM_PTR)(Ptr))
#define NUM_TO_PTR(Num)			((void*)(NUM_PTR)(Num))

#define PTR_DELTA(__Ptr1,__Ptr2)	((UINT)(__Ptr2-__Ptr1))

#define LOW_BYTE(w)			((BYTE)(w))
#define HIGH_BYTE(w)			((BYTE)((WORD)(w)>>8))

#define LOW_WORD(l)			((WORD)(l))
#define HIGH_WORD(l)			((WORD)((DWORD)(l)>>16))

#define LOW_DWORD(l)			((DWORD)(l))
#define HIGH_DWORD(l)			((DWORD)((QWORD)(l)>>32))

#define RT_PVOID(ptr,off)		((void*)( (BYTE*)(ptr) + (off) ))
#define RT_LPVOID(ptr,off)		((void*)( (BYTE*)(ptr) + (off) ))
#define RT_PDWORD(ptr,off)		((DWORD*)( (BYTE*)(ptr) + (off) ))
#define RT_PWORD(ptr,off)		((WORD *)( (BYTE*)(ptr) + (off) ))
#define RT_PBYTE(ptr,off)		((BYTE *)( (BYTE*)(ptr) + (off) ))
#define RT_PCHAR(ptr,off)		((CHAR *)( (BYTE*)(ptr) + (off) ))

#define RT_BYTE(ptr,off)		(*( (BYTE*)(ptr) + (off) ))

#define _RT_PDWORD(ptr)			((DWORD*)(ptr) )
#define _RT_PWORD(ptr)			((WORD *)(ptr) )
#define _RT_PBYTE(ptr)			((BYTE *)(ptr) )
#define _RT_PCHAR(ptr)			((CHAR *)(ptr) )
#define _RT_PVOID(ptr)			((void *)(ptr) )

#define GET_CHAR(ptr,off)		(*(CHAR *)( (BYTE*)(ptr) + (off) ) )
#define _GET_CHAR(ptr)			(*(CHAR *)(ptr) )
#define _GET_CCHAR(ptr)			(*(const CHAR *)(ptr) )

#define PUT_CHAR(ptr,off,val)		do{(*(CHAR *)( (BYTE*)(ptr) + (off) ) ) = val;}while(0)
#define _PUT_CHAR(ptr,val)		do{(*(CHAR *)(ptr) ) = val;}while(0)

#define GET_BYTE(ptr,off)		(*(BYTE *)( (BYTE*)(ptr) + (off) ) )
#define _GET_BYTE(ptr)			(*(BYTE *)(ptr) )

#define PUT_BYTE(ptr,off,val)		do{(*(BYTE *)( (BYTE*)(ptr) + (off) ) ) = val;}while(0)
#define _PUT_BYTE(ptr,val)		do{(*(BYTE *)(ptr) ) = val;}while(0)

#ifdef CODE_BIG_ENDIAN

#define GET_WORD(ptr,off)		(*RT_PBYTE(ptr,off)+(*RT_PBYTE(ptr,(off)+1)<<8))
#define _GET_WORD(ptr)			(*_RT_PBYTE(ptr)+(*RT_PBYTE(ptr,1)<<8))

#define PUT_WORD(ptr,off,val)		do{PUT_BYTE(ptr,off,LOW_BYTE(val));PUT_BYTE(ptr,(off)+1,HIGH_BYTE(val));}while(0)
#define _PUT_WORD(ptr,val)		PUT_WORD(ptr,0,val)
#define GET_3BYTES(ptr,off)		(*RT_PBYTE(ptr,off)+(*RT_PBYTE(ptr,(off)+1)<<8)+(*RT_PBYTE(ptr,(off)+2)<<16))
#define _GET_3BYTES(ptr)		(*_RT_PBYTE(ptr)+(*RT_PBYTE(ptr,1)<<8)+(*RT_PBYTE(ptr,2)<<16))
#define PUT_3BYTES(ptr,off,val)		do{PUT_WORD(ptr,(off)+1,(WORD)((val)>>8));PUT_BYTE(ptr,off,LOW_BYTE(val));}while(0)
#define _PUT_3BYTES(ptr,val)		PUT_3BYTES(ptr,0,val)
#define GET_DWORD(ptr,off)		(*RT_PBYTE(ptr,off)+(*RT_PBYTE(ptr,(off)+1)<<8)+(*RT_PBYTE(ptr,(off)+2)<<16)+(*RT_PBYTE(ptr,(off)+3)<<24))
#define _GET_DWORD(ptr)			(*_RT_PBYTE(ptr)+(*RT_PBYTE(ptr,1)<<8)+(*RT_PBYTE(ptr,2)<<16)+(*RT_PBYTE(ptr,3)<<24))
#define PUT_DWORD(ptr,off,val)		do{PUT_WORD(ptr,off,LOW_WORD(val));PUT_WORD(ptr,(off)+2,HIGH_WORD(val));}while(0)
#define _PUT_DWORD(ptr,val)		PUT_DWORD(ptr,0,val)

#define BIG_ENDIAN_DWORD(x)		(x = _GET_DWORD(&(x)))
#define BIG_ENDIAN_WORD(x)		(x = _GET_WORD(&(x)))

#else//CODE_BIG_ENDIAN

#define GET_WORD(ptr,off)		(*(WORD*)( (BYTE*)(ptr) + (off) ) )
#define _GET_WORD(ptr)			(*(WORD*)( (BYTE*)(ptr)) )
#define PUT_WORD(ptr,off,val)		do{(*(WORD*)( (BYTE*)(ptr) + (off) ) )=val;}while(0)
#define _PUT_WORD(ptr,val)		do{(*(WORD*)( (BYTE*)(ptr)) )=val;}while(0)

#define GET_3BYTES(ptr,off)		MAKE_DWORD((BYTE)GET_WORD(ptr,(off)+2),GET_BYTE(ptr,off))
#define _GET_3BYTES(ptr)		MAKE_DWORD((BYTE)GET_WORD(ptr,2),_GET_BYTE(ptr))

#define PUT_3BYTES(ptr,off,val)		do{PUT_WORD(ptr,off,LOW_WORD(val));PUT_BYTE(ptr,(off)+2,(BYTE)HIGH_WORD(val));}while(0)
#define _PUT_3BYTES(ptr,val)		PUT_3BYTES(ptr,0,val)

#define GET_DWORD(ptr,off)		(*(DWORD*)( (BYTE*)(ptr) + (off) ) )
#define _GET_DWORD(ptr)			(*(DWORD*)( (BYTE*)(ptr) ) )

#define PUT_DWORD(ptr,off,val)		do{(*(DWORD*)( (BYTE*)(ptr) + (off) ) )=(DWORD)(val);}while(0)
#define _PUT_DWORD(ptr,val)		do{(*(DWORD*)( (BYTE*)(ptr) ) )=(DWORD)(val);}while(0)

#define BIG_ENDIAN_DWORD
#define BIG_ENDIAN_WORD

#endif//CODE_BIG_ENDIAN

//
#ifndef ABS
#define ABS(a)		((a)>=0?(a):-(a))
#endif

#ifndef MIN
#define MIN(a,b)	((a)>(b)?(b):(a))
#endif

#ifndef MAX
#define MAX(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef CLAMP
//#define CLAMP(x,l,h)	MIN((h), MAX((l),(x)))
#define CLAMP(x,l,h)	(((x)<(l)) ? (l): (((h)<(x)) ? (h):(x)))
#endif

//
template<class T>
bool is_digit(T c);//		{ return c>='0'&&c<='9'; }
template<class T>
bool is_lower(T c);//		{ return c>='a'&&c<='z'; }
template<class T>
bool is_upper(T c);//		{ return c>='A'&&c<='Z'; }
template<class T>
bool is_letter(T c);//		{ return is_lower(c) || is_upper(c); }
template<class T>
bool is_hex_letter(T c);//	{ return (c>='a'&&c<='f') || (c>='A'&&c<='F'); }
template<class T>
bool is_hex_digit(T c);//	{ return is_digit(c) || is_hex_letter(c); }

template<>
inline bool is_digit<char>(char c)		{ return c>='0'&&c<='9'; }
template<>
inline bool is_lower<char>(char c)		{ return c>='a'&&c<='z'; }
template<>
inline bool is_upper<char>(char c)		{ return c>='A'&&c<='Z'; }
template<>
inline bool is_letter<char>(char c)		{ return is_lower(c) || is_upper(c); }
template<>
inline bool is_hex_letter<char>(char c)		{ return (c>='a'&&c<='f') || (c>='A'&&c<='F'); }
template<>
inline bool is_hex_digit<char>(char c)		{ return is_digit(c) || is_hex_letter(c); }
//
template<>
inline bool is_digit<WCHAR>(WCHAR c)		{ return c>=L'0'&&c<=L'9'; }
template<>
inline bool is_lower<WCHAR>(WCHAR c)		{ return c>=L'a'&&c<=L'z'; }
template<>
inline bool is_upper<WCHAR>(WCHAR c)		{ return c>=L'A'&&c<=L'Z'; }
template<>
inline bool is_letter<WCHAR>(WCHAR c)		{ return is_lower(c) || is_upper(c); }
template<>
inline bool is_hex_letter<WCHAR>(WCHAR c)	{ return (c>=L'a'&&c<=L'f') || (c>=L'A'&&c<=L'F'); }
template<>
inline bool is_hex_digit<WCHAR>(WCHAR c)	{ return is_digit(c) || is_hex_letter(c); }

#ifndef IS_DIGIT
#define IS_DIGIT(c)		((c)>='0'&&(c)<='9')
#endif

#ifndef IS_LOWER
#define IS_LOWER(c)		((c)>='a'&&(c)<='z')
#endif

#ifndef IS_UPPER
#define IS_UPPER(c)		((c)>='A'&&(c)<='Z')
#endif

#ifndef IS_LETTER
#define IS_LETTER(c)		(IS_LOWER(c) || IS_UPPER(c))
#endif

#ifndef IS_HEX_LETTER
#define IS_HEX_LETTER(c)	( ((c)>='a'&&(c)<='f') || ((c)>='A'&&(c)<='F') )
#endif

#ifndef IS_HEX_DIGIT
#define IS_HEX_DIGIT(c)		(IS_DIGIT(c) || IS_HEX_LETTER(c))
#endif

#ifndef SWAP
#define SWAP(a,b)	{a=a^b;b=a^b;a=a^b;}
#endif

#define PAGE_COUNT(__size,__page)	((__size)/(__page)+(((__size)%(__page))?1:0))
#define ALIGN_SIZE(__size,__page)	(PAGE_COUNT((__size),(__page))*(__page))
#define CENTER_ALGN(__size,__total)	(((__total)-(__size))/2)

#define SafeDelete(__Ptr)		if(__Ptr){delete __Ptr;__Ptr=NULL;}
#define SafeDeleteAry(__Ptr)		if(__Ptr){delete[] __Ptr;__Ptr=NULL;}
#define SafeRelease(__Ptr)		if(__Ptr){__Ptr->Release();__Ptr=NULL;}

template<class T>
void make_char_upper(T & __c);//	{ if(__c>='a' && __c <='z')__c +='A'-'a'; }
template<class T>
void make_char_lower(T & __c);//	{ if(__c>='a' && __c <='z')__c +='a'-'A'; }
template<class T>
T upper_char(T __c);//			{ return ((__c>='a' && __c<='z')?__c+'A'-'a':__c); }
template<class T>
T lower_char(T __c);//			{ return ((__c>='A' && __c<='Z')?__c+'a'-'A':__c); }

template<>
inline void make_char_upper<char>(char & __c)	{ if(__c>='a' && __c <='z')__c +='A'-'a'; }
template<>
inline void make_char_lower<char>(char & __c)	{ if(__c>='a' && __c <='z')__c +='a'-'A'; }
template<>
inline char upper_char<char>(char __c)		{ return ((__c>='a' && __c<='z')?__c+'A'-'a':__c); }
template<>
inline char lower_char<char>(char __c)		{ return ((__c>='A' && __c<='Z')?__c+'a'-'A':__c); }

template<>
inline void make_char_upper<WCHAR>(WCHAR & __c)	{ if(__c>=L'a' && __c <=L'z')__c +=L'A'-L'a'; }
template<>
inline void make_char_lower<WCHAR>(WCHAR & __c)	{ if(__c>=L'a' && __c <=L'z')__c +=L'a'-L'A'; }
template<>
inline WCHAR upper_char<WCHAR>(WCHAR __c)	{ return ((__c>=L'a' && __c<=L'z')?__c+L'A'-L'a':__c); }
template<>
inline WCHAR lower_char<WCHAR>(WCHAR __c)	{ return ((__c>=L'A' && __c<=L'Z')?__c+L'a'-L'A':__c); }

#define MAKE_CHAR_UPPER(__c)	if(__c>='a' && __c <='z')__c +='A'-'a'
#define MAKE_CHAR_LOWER(__c)	if(__c>='A' && __c <='Z')__c +='a'-'A'
#define UPPER_CHAR(__c)		((__c>='a' && __c<='z')?__c+'A'-'a':__c)
#define LOWER_CHAR(__c)		((__c>='A' && __c<='Z')?__c+'a'-'A':__c)

#define MAX_LIMIT(__L,__M)	if((__L)>(__M))__L=__M
#define MIN_LIMIT(__L,__M)	if((__L)<(__M))__L=__M

//#include <winnt.h>
//#define CONTAINING_RECORD(address, type, field) ((type *)( \
//                                                  (PCHAR)(address) - \
//                                                  (ULONG_PTR)(&((type *)0)->field)))

#define CONTAINING_RECORD_MY1(address, type, field) (\
    (type *)((char*)(address) -(LONG_PTR)(&((type *)0)->field)))

#define CONTAINING_RECORD_MY(address, type, field) (\
    (type *)( (char*)(address) -(LONG_PTR)(field) ) )

#define ALIGNUP(__M,__N)	(((__M)+(__N)-1)/(__N))
#define ALIGNDOWN(__M,__N)	((__M)/(__N))

#define ROUNDNEXT(__M,__N)		((__M)+(__N)-((__M)%(__N)))
#define ROUNDPREV(__M,__N)		((__M)      -((__M)%(__N)))

#define ROUNDREVERSE(__M,__N)		((__N)      -((__M)%(__N)))

#define lenof(x) sizeof(x)/sizeof(x[0])

struct FILESIG
{
	DWORD TimeStamp;
	DWORD CheckSum;

	FILESIG(DWORD TimeStamp, DWORD CheckSum):TimeStamp(TimeStamp), CheckSum(CheckSum) {}

	bool operator<(const FILESIG & other) const
	{
		if (TimeStamp < other.TimeStamp) return true;
		if (TimeStamp > other.TimeStamp) return false;
		if (CheckSum < other.CheckSum) return true;
		return false;
	}
};

#endif
