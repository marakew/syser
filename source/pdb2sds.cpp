#include <stdlib.h>
#include <stdio.h>
#include "Code/alt/altlist.hpp"
#include "Code/alt/altmap.hpp"
#include "Code/sysdep.hpp"

#include "Code/define.h"
#include "Code/tstrmem.hpp"

#include "EXEAnalyzer/sourcedebug.hpp"
#include "EXEAnalyzer/sdsmodule.hpp"


void CreateProgress(unsigned long MaxPos)
{
	printf("\n%d ", MaxPos);
}

void UpdateProgress(unsigned long Pos)
{
	printf("*");
}

void UpdateCaption(const WCHAR *Text)
{
	char szText[256];
	TStrCpy(szText, Text);
	printf("%s ", szText);
}

void DestroyProgress()
{
	printf("\n");
}

	CSourceDebug SourceDebug;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("error: incorrect args!\n%s file.pdb\nor\n%s file.exe", TGetFileName(argv[0]),TGetFileName(argv[0]));
		return 1;
	}
	SourceDebug.m_CreateProgress = CreateProgress;
	SourceDebug.m_UpdateProgress = UpdateProgress;
	SourceDebug.m_UpdateCaption = UpdateCaption;
	SourceDebug.m_DestroyProgress = DestroyProgress;

	char szModule[MAX_FN_LEN];
	//TStrCpy(szModule, "L:\\flash2G_my\\syser\\Syser\\source\\x32_\\syserx32.exe"); //SYSER
	TStrCpy(szModule, "L:\\flash2G_my\\syser\\Syser\\source\\x32_\\testx32.exe"); //TEST
	TStrCpy(szModule, argv[1]);
	//TStrCpy(szModule, "L:\\flash2G_my\\syser\\Syser\\source\\x32\\testx32.exe"); //WORK
	char SDSFullFileName[MAX_FN_LEN];
	char *pSDSFullFileName = SDSFullFileName;
	SDSFullFileName[0] = 0;
	//TStrCpy(SDSFullFileName, "L:\\flash2G_my\\syser\\Syser\\source\\x32\\testx32.exe.sds"); //MY LAST
//	TStrCpy(SDSFullFileName, "L:\\flash2G_my\\syser\\Syser\\source\\x32_\\testx32.exe.sds"); //MY LAST
	//TStrCpy(SDSFullFileName, "L:\\flash2G_my\\syser\\Syser\\source\\x32_\\testx32.exe.sds_MY"); //ERROR
	//TStrCpy(SDSFullFileName, "L:\\flash2G_my\\syser\\Syser\\source\\x32_\\testx32.exe.sds_SYS");
//#define LOAD_SDS
//#ifndef LOAD_SDS
	if (pSDSFullFileName[0] == 0)
{
	printf("OPEN Module %s\n", szModule);
	if (!SourceDebug.CheckSDS(szModule, pSDSFullFileName))
	{
		if (!SourceDebug.Load(szModule, pSDSFullFileName, eUseSourcesNone))
		{
			printf("OPEN ERROR\n");
			return 1;
		} else
		{
			printf("Convert OK\n");
		}
	} else
	{
		printf("Check SDS OK\n");
	}
}
//#endif
	printf("SDSFile %s\n", pSDSFullFileName);
	//printf("SIZE %d\n", sizeof(SDSFILE));

	CSDSModule sdsmodule;
	if (!sdsmodule.Load(pSDSFullFileName))
	{
		printf("Check Load SDS: ERROR\n");
		return 1;
	}

	FILESIG FileSig = CSDSModule::GetFILESIG(pSDSFullFileName);
	printf("Sig: Timestamp 0x%08x CheckSum 0x%08x\n", FileSig.TimeStamp, FileSig.CheckSum);
	if (FileSig.TimeStamp == 0 && FileSig.CheckSum == 0)
	{
		printf("Sig - ERROR\n", FileSig.TimeStamp, FileSig.CheckSum);
		return 1;
	}
	return 0;
}
