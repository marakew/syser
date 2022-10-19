
#include <stdlib.h>
#include <stdio.h>

#include "Code/define.h"
#include "Code/tstrmem.hpp"
#include "Code/sysdep.hpp"
#include "Code/fileiont.hpp"

int Find(CFileIO *This, const char *FullFileName, int FileLength, void *pData)
{
	char *Txt = (char*)pData;

	char szModuleName[MAX_FN_LEN];
	//::GetSystemDirectoryA(szModuleName, MAX_FN_LEN);
	GetModulePath(szModuleName, true); //+++

	HANDLE Handle;
	if (!gpFileIO->OpenFile(FullFileName, &Handle, 0x10000))
		return 0;

	unsigned long long Size = gpFileIO->GetFileLength(Handle);
	unsigned long *Sym = (unsigned long *)new char[Size];
	gpFileIO->ReadFile(Handle, Sym, Size, 0);
	gpFileIO->CloseHandle(Handle);

	FullFileName = TGetFileName(FullFileName);
	//TStrCat(szModuleName, "\\drivers\\plugin\\");
	TStrCat(szModuleName, FullFileName);

	char *pStr = TStrIStr(szModuleName, ".sym");
	if (pStr) *pStr = 0;

	FILE *f = fopen(szModuleName, "w");
	if (!f)
	{
		delete [](char*)Sym;
		return 0;
	}
	unsigned long Count = Sym[0];
	unsigned long FuncOrd = Sym[1];

	for (unsigned long n = 0; n < Count; ++n)
	{
		if (Txt[Sym[n+2]])
		{
			fprintf(f,"%d %s\n",FuncOrd+n, &Txt[Sym[n+2]]);
			fflush(f);
		}
	}
	
	fclose(f);

	delete [](char*)Sym;
	return 0;
}

int main()
{
	::FreeConsole();

	char szMfcSymFile[MAX_FN_LEN];
	GetModulePath(szMfcSymFile, true);
	TStrCat(szMfcSymFile, "mfcsym.txt");

	HANDLE Handle;
	if (!gpFileIO->OpenFile(szMfcSymFile, &Handle, 0x10000))
		return 1;

	char *pStr = TStrRChr(szMfcSymFile, '\\');
	if (pStr && *pStr == '\\')
		*pStr = 0;
	//char *pStr = TGetFileName(szMfcSymFile);
	//if (pStr)
	//	*pStr = 0;
	else	::GetCurrentDirectoryA(MAX_FN_LEN, szMfcSymFile);

	TStrCat(szMfcSymFile, "\\*.sym");

	unsigned long long Size = gpFileIO->GetFileLength(Handle);
	char *Txt = new char[Size];

	gpFileIO->ReadFile(Handle, Txt, Size, 0);
	gpFileIO->CloseHandle(Handle);

	gpFileIO->XFind(szMfcSymFile, Find, Txt, false, nullptr); //???

	delete []Txt;
	return 0;
}
