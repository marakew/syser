
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "syserdefine.hpp"
#include "othercmd.hpp"

int resize_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	int width;
	int height;
	if (argc == 1)
	{
		width = gpSyser->DesktopSize.cx;
		height = gpSyser->DesktopSize.cy;
	} else
	if (argc != 3 || !USDecStrToNum(argv[1], &width) || !USDecStrToNum(argv[1], &height))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : ws width height\n"));
		return 1;
	}

	if (width > 2048 || height > 1536)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : window is too larger !\n"));
		return 1;
	}

	if (width < 320 || height < 200)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : window is too smaller !\n"));
		return 1;
	}
	gpSyser->m_MainFrame.Resize(width, height, 1);
	return 0;
}

int lines_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc == 1)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : lines number\n"));
		return 0;
	}

	unsigned int lines;
	if (argc == 2 && USDecStrToNum(argv[1], &lines))
	{
		lines *= gpSyser->m_FontList[1].m_Height;
		if (lines >= gpSyser->DesktopSize.cy) //MAX_LIMIT
			lines = gpSyser->DesktopSize.cy;
		gpSyser->m_MainFrame.Resize(gpSyser->m_MainFrame.m_WindowRect.cx, lines, 1);
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : lines number\n"));
	}
	return 0;
}

int width_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc == 1)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : width number\n"));
		return 0;
	}

	unsigned int width;
	if (argc == 2 && USDecStrToNum(argv[1], &width))
	{
		width *= gpSyser->m_FontList[1].m_Width;
		if (width >= gpSyser->DesktopSize.cx) //MAX_LIMIT
			width = gpSyser->DesktopSize.cx;
		gpSyser->m_MainFrame.Resize(width, gpSyser->m_MainFrame.m_WindowRect.cy, 1);
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : width number\n"));
	}
	return 0;
}

int ioctl_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	WCHAR DeviceType[50];

	if (argc == 1)
	{
		for (unsigned long n = 0; stIoctlList[n].Name; ++n)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%08X "), stIoctlList[n].ControlCode);
			TStrCpy(DeviceType, stIoctlList[n].Name);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("FILE_DEVICE_"));
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s"), DeviceType);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
		}
	} else
	{
		const char *szFileDevices[0x5C+1];
		szFileDevices[0] = NULL;
		szFileDevices[1] = "BEEP";
		szFileDevices[2] = "CD_ROM";
		szFileDevices[3] = "CD_ROM_FILE_SYSTEM";
		szFileDevices[4] = "CONTROLLER";
		szFileDevices[5] = "DATALINK";
		szFileDevices[6] = "DFS";
		szFileDevices[7] = "DISK";
		szFileDevices[8] = "DISK_FILE_SYSTEM";
		szFileDevices[9] = "FILE_SYSTEM";
		szFileDevices[0xA] = "INPORT_PORT";
		szFileDevices[0xB] = "KEYBOARD";
		szFileDevices[0xC] = "MAILSLOT";
		szFileDevices[0xD] = "MIDI_IN";
		szFileDevices[0xE] = "MIDI_OUT";
		szFileDevices[0xF] = "MOUSE";
		szFileDevices[0x10] = "MULTI_UNC_PROVIDER";
		szFileDevices[0x11] = "NAMED_PIPE";
		szFileDevices[0x12] = "NETWORK";
		szFileDevices[0x13] = "NETWORK_BROWSER";
		szFileDevices[0x14] = "NETWORK_FILE_SYSTEM";
		szFileDevices[0x15] = "NULL";
		szFileDevices[0x16] = "PARALLEL_PORT";
		szFileDevices[0x17] = "PHYSICAL_NETCARD";
		szFileDevices[0x18] = "PRINTER";
		szFileDevices[0x19] = "SCANNER";
		szFileDevices[0x1A] = "SERIAL_MOUSE_PORT";
		szFileDevices[0x1B] = "SERIAL_PORT";
		szFileDevices[0x1C] = "SCREEN";
		szFileDevices[0x1D] = "SOUND";
		szFileDevices[0x1E] = "STREAMS";
		szFileDevices[0x1F] = "TAPE";
		szFileDevices[0x20] = "TAPE_FILE_SYSTEM";
		szFileDevices[0x21] = "TRANSPORT";
		szFileDevices[0x22] = "UNKNOWN";
		szFileDevices[0x23] = "VIDEO";
		szFileDevices[0x24] = "VIRTUAL_DISK";
		szFileDevices[0x25] = "WAVE_IN";
		szFileDevices[0x26] = "WAVE_OUT";
		szFileDevices[0x27] = "8042_PORT";
		szFileDevices[0x28] = "NETWORK_REDIRECTOR";
		szFileDevices[0x29] = "BATTERY";
		szFileDevices[0x2A] = "BUS_EXTENDER";
		szFileDevices[0x2B] = "MODEM";
		szFileDevices[0x2C] = "VDM";
		szFileDevices[0x2D] = "MASS_STORAGE";
		szFileDevices[0x2E] = "SMB";
		szFileDevices[0x2F] = "KS";
		szFileDevices[0x30] = "CHANGER";
		szFileDevices[0x31] = "SMARTCARD";
		szFileDevices[0x32] = "ACPI";
		szFileDevices[0x33] = "DVD";
		szFileDevices[0x34] = "FULLSCREEN_VIDEO";
		szFileDevices[0x35] = "DFS_FILE_SYSTEM";
		szFileDevices[0x36] = "DFS_VOLUME";
		szFileDevices[0x37] = "SERENUM";
		szFileDevices[0x38] = "TERMSRV";
		szFileDevices[0x39] = "KSEC";
		szFileDevices[0x3A] = "FIPS";
		szFileDevices[0x3B] = "INFINIBAND";

		szFileDevices[0x3C] = NULL;
		szFileDevices[0x3D] = NULL;
		szFileDevices[0x3E] = "VMBUS";
		szFileDevices[0x3F] = "CRYPT_PROVIDER";
		szFileDevices[0x40] = "WPD";
		szFileDevices[0x41] = "BLUETOOTH";
		szFileDevices[0x42] = "MT_COMPOSITE";
		szFileDevices[0x43] = "MT_TRANSPORT";
		szFileDevices[0x44] = "BIOMETRIC";
		szFileDevices[0x45] = "PMI";
		szFileDevices[0x46] = "EHSTOR";
		szFileDevices[0x47] = "DEVAPI";
		szFileDevices[0x48] = "GPIO";
		szFileDevices[0x49] = "USBEX";
		szFileDevices[0x4A] = NULL;
		szFileDevices[0x4B] = NULL;
		szFileDevices[0x4C] = NULL;
		szFileDevices[0x4D] = NULL;
		szFileDevices[0x4E] = NULL;
		szFileDevices[0x4F] = NULL;
		szFileDevices[0x50] = "CONSOLE";
		szFileDevices[0x51] = "NFP";
		szFileDevices[0x52] = "SYSENV";
		szFileDevices[0x53] = "VIRTUAL_BLOCK";
		szFileDevices[0x54] = "POINT_OF_SERVICE";
		szFileDevices[0x55] = "STORAGE_REPLICATION";
		szFileDevices[0x56] = "TRUST_ENV";
		szFileDevices[0x57] = "UCM";
		szFileDevices[0x58] = "UCMTCPCI";
		szFileDevices[0x59] = "PERSISTENT_MEMORY";
		szFileDevices[0x5A] = "NVDIMM";
		szFileDevices[0x5B] = "HOLOGRAPHIC";
		szFileDevices[0x5C] = "SDFXHCI";

		for (unsigned long i = 1; i < argc; ++i)
		{
			ULONG_PTR ControlCode = 0;
			if (!gpSyser->m_SyserUI.CalcExp(argv[i], &ControlCode))
			{
				char szFileDevice[125];
				memset(szFileDevice, 0, sizeof(szFileDevice));
				TStrCpy(szFileDevice, "FILE_DEVICE_");
				unsigned long n = 0;
				for ( ; stIoctlList[n].Name; ++n)
				{
					TStrCpy(&szFileDevice[sizeof("FILE_DEVICE_")-1], stIoctlList[n].Name);
					if (TStrIStr(szFileDevice, argv[i])) break;
				}
				if (!stIoctlList[n].Name) continue;
				ControlCode = stIoctlList[n].ControlCode;
			}

			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("    Control Code:  %08x "), ControlCode);
			for (unsigned long n = 0; stIoctlList[n].Name; ++n)
			{
				if (stIoctlList[n].ControlCode == ControlCode)
				{
					TStrCpy(DeviceType, stIoctlList[n].Name);
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s"), DeviceType);
					break;
				}
			}

			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
			if (ControlCode & 0x80000000)
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("    Device Type:   %x(vendor-assigned) "), (ControlCode | 0x8000FFFF) >> 16);
			} else
			{
				unsigned long d = (ControlCode >> 16) & 0x7FFF;
				if (d && d < (0x3B+1))//lenof(szFileDevices)) //<= 59 //TODO
				{
					TStrCpy(DeviceType, "FILE_DEVICE_");
					TStrCat(DeviceType, szFileDevices[d]);
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("    Device Type:   %s"), DeviceType);
				} else
				{
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("    Device Type:   %x(Microsoft reserved)"), d);
				}
			}
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n    Function:      "));
			if (ControlCode & 0x2000)
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0x%x (%d) "), (ControlCode & 0x1FFC | 0x2000) >> 2, (ControlCode & 0x1FFC | 0x2000) >> 2);
			else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0x%x (%d)(Microsoft Reserved) "), (ControlCode >> 2) & 0x7FF, (ControlCode >> 2) & 0x7FF);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n    Method:        "));
			switch (ControlCode & 3)
			{
			case 0:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("METHOD_BUFFERED "));
				break;
			case 1:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("METHOD_IN_DIRECT "));
				break;
			case 2:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("METHOD_OUT_DIRECT "));
				break;
			case 3:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("METHOD_NEITHER "));
				break;
			}
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n    Access:        "));
			switch ( (unsigned short)(ControlCode >> 14))
			{
			case 0:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("FILE_ANY_ACCESS "));
				break;
			case 1:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("FILE_READ_ACCESS "));
				break;
			case 2:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("FILE_WRITE_ACCESS "));
				break;
			case 3:
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("FILE_READ_ACCESS|FILE_WRITE_ACCESS "));
				break;
			}
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
		}
	}
	return 1;
}

int ntstatus_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	unsigned long Count = 0;
	if (argc == 1)
	{
		for (unsigned long n = 0; stNtStatusList[n].MsgName; ++n)
		{
			WCHAR MsgName[256];
			TStrCpy(MsgName, stNtStatusList[n].MsgName);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %s\n"), stNtStatusList[n].MsgNumber, MsgName);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	} else
	if (argc > 1)
	{
		--argc;
		++argv;
		while (argc>0)
		{
			ULONG_PTR Result;
			if (gpSyser->m_SyserUI.CalcExp(argv[0], &Result))
			{
		for (unsigned long n = 0; stNtStatusList[n].MsgName; ++n)
		{
			if (stNtStatusList[n].MsgNumber == Result)
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stNtStatusList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %<b>%s "), stNtStatusList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stNtStatusList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
				++Count;
			}
		}
			} else
			{
		for (unsigned long n = 0; stNtStatusList[n].MsgName; ++n)
		{
			if (!TStrNICmp(argv[0], stNtStatusList[n].MsgName, TStrLen(argv[0])))
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stNtStatusList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<b>0X%08x  %<1>%s "), stNtStatusList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stNtStatusList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
			}
		}
			}
			++argv;
			--argc;
		}
		if (Count)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	}
	return 0;
}

int winerror_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	unsigned long Count = 0;
	if (argc == 1)
	{
		for (unsigned long n = 0; stWinErrorList[n].MsgName; ++n)
		{
			WCHAR MsgName[256];
			TStrCpy(MsgName, stWinErrorList[n].MsgName);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %s\n"), stWinErrorList[n].MsgNumber, MsgName);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	} else
	if (argc > 1)
	{
		--argc;
		++argv;
		while (argc>0)
		{
			ULONG_PTR Result;
			if (gpSyser->m_SyserUI.CalcExp(argv[0], &Result))
			{
		for (unsigned long n = 0; stWinErrorList[n].MsgName; ++n)
		{
			if (stWinErrorList[n].MsgNumber == Result)
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stWinErrorList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %<b>%s "), stWinErrorList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stWinErrorList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
				++Count;
			}
		}
			} else
			{
		for (unsigned long n = 0; stWinErrorList[n].MsgName; ++n)
		{
			if (!TStrNICmp(argv[0], stWinErrorList[n].MsgName, TStrLen(argv[0])))
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stWinErrorList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<b>0X%08x  %<1>%s "), stWinErrorList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stWinErrorList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
			}
		}
			}
			++argv;
			--argc;
		}
		if (Count)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	}
	return 0;
}

int bugcheckcode_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	unsigned long Count = 0;
	if (argc == 1)
	{
		for (unsigned long n = 0; stBugCheckCodeList[n].MsgName; ++n)
		{
			WCHAR MsgName[256];
			TStrCpy(MsgName, stBugCheckCodeList[n].MsgName);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %s\n"), stBugCheckCodeList[n].MsgNumber, MsgName);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	} else
	if (argc > 1)
	{
		--argc;
		++argv;
		while (argc>0)
		{
			ULONG_PTR Result;
			if (gpSyser->m_SyserUI.CalcExp(argv[0], &Result))
			{
		for (unsigned long n = 0; stBugCheckCodeList[n].MsgName; ++n)
		{
			if (stBugCheckCodeList[n].MsgNumber == Result)
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stBugCheckCodeList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("0X%08x  %<b>%s "), stBugCheckCodeList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stBugCheckCodeList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
				++Count;
			}
		}
			} else
			{
		for (unsigned long n = 0; stBugCheckCodeList[n].MsgName; ++n)
		{
			if (!TStrNICmp(argv[0], stBugCheckCodeList[n].MsgName, TStrLen(argv[0])))
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stBugCheckCodeList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<b>0X%08x  %<1>%s "), stBugCheckCodeList[n].MsgNumber, MsgName);
				TStrCpy(MsgName, stBugCheckCodeList[n].MsgDesc);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
			}
		}
			}
			++argv;
			--argc;
		}
		if (Count)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	}
	return 0;
}

int wmsg_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	unsigned long Count = 0;
	if (argc == 1)
	{
		for (unsigned long n = 0; stMsgList[n].MsgName; ++n)
		{
			WCHAR MsgName[256];
			TStrCpy(MsgName, stMsgList[n].MsgName);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%04x  %s\n"), stMsgList[n].MsgNumber, MsgName);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	} else
	if (argc > 1)
	{
		--argc;
		++argv;
		while (argc>0)
		{
			ULONG_PTR Result;
			if (gpSyser->m_SyserUI.CalcExp(argv[0], &Result))
			{
		for (unsigned long n = 0; stMsgList[n].MsgName; ++n)
		{
			if (stMsgList[n].MsgNumber == Result)
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stMsgList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%04x  %<b>%s\n"), stMsgList[n].MsgNumber, MsgName);
				++Count;
			}
		}
			} else
			{
		for (unsigned long n = 0; stMsgList[n].MsgName; ++n)
		{
			if (!TStrNICmp(argv[0], stMsgList[n].MsgName, TStrLen(argv[0])))
			{
				WCHAR MsgName[256];
				TStrCpy(MsgName, stMsgList[n].MsgName);
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<b>%04x  %<1>%s\n"), stMsgList[n].MsgNumber, MsgName);
			}
		}
			}
			++argv;
			--argc;
		}
		if (Count)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("totals (%<e>%d%<1>)\n"), Count);
	}
	return 0;
}

int zap_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc != 1)
		return -1;

	X86_CPU_REG_PTR X86_REG;
	pSyser->m_pDebugger->GetX86RegPtr(&X86_REG);
	unsigned short word;
	pSyser->m_pDebugger->ReadMemory(*X86_REG.pCIP - 2, &word, 2);

	if (word == 0x1CD || word == 0x3CD)
	{
		word = 0x9090;
		pSyser->m_pDebugger->WriteMemory(*X86_REG.pCIP - 2, &word, 2);
	} else
	if (((word & 0xFF00) == 0xCC00) || ((word & 0xFF00) == 0xF100))
	{
		word = 0x9090;
		pSyser->m_pDebugger->WriteMemory(*X86_REG.pCIP - 1, &word, 1);
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("NO embedded INT 1 or INT 3\n"));
	}
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int file_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	CSDSModule *pSDSMod = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.m_pSDSMod;
	if (!pSDSMod)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : not found symbol module.\n"));
		return 1;
	}
	if (argc == 1)
	{
		unsigned long Len = 0;
		SrcFileNameID *SrcFileName = pSDSMod->GetSrcFiles(&Len);
		if (!SrcFileName)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : not found source file.\n"));
			return 1;
		}
		for (unsigned long n = 0; n < Len; ++n)
		{
			if (SrcFileName[n].FileSize)
			{
				char *pFileName = TStrRChr(SrcFileName[n].FileName, '\\');
				if (pFileName)
				{
					WCHAR FileName[512];
					TStrCpy(FileName, pFileName + 1);
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s\n"), FileName);
				}
			}
		}
	} else
	if (argc > 1)
	{
		char Buffer[512];
		int index = 1;
		while (index < argc)
		{
			TStrCpy(Buffer, argv[index]);
			SrcFileNameID *SrcFile = pSDSMod->GetSrcFileName(Buffer);
			if (SrcFile)
			{
				gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod, SrcFile->FileID, 0);
				gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
			}
			++index;
		}
	}
	return 0;
}

int watch_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc < 2)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : watch variablename [variablename2 ..].\n"));
		return 1;
	}

	if (!gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.m_pSDSMod)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : not found symbol module.\n"));
		return 1;
	}

	int index = 1;
	while (index < argc)
	{
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_TypeViewerWnd.AddWatch(argv[index], 0);
		gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
		++index;
	}
	return 0;
}

int table_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (gpSyser->m_SyserUI.m_SDSModulesMap.size() == 0 || argc > 2)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : not found symbol module.\n"));
		return 1;
	}
	WCHAR szModuleName[MAX_FN_LEN]; //512
	if (argc == 1)
	{
		for (map<FILESIG, CSDSModule>::IT it = gpSyser->m_SyserUI.m_SDSModulesMap.begin();
			it != gpSyser->m_SyserUI.m_SDSModulesMap.end(); ++it)
		{
			TStrCpy(szModuleName, it->second.GetModuleName());
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s\n"), szModuleName);
		}
	} else
	{
		for (map<FILESIG, CSDSModule>::IT it = gpSyser->m_SyserUI.m_SDSModulesMap.begin();
			it != gpSyser->m_SyserUI.m_SDSModulesMap.end(); ++it)
		{
			TStrCpy(szModuleName, it->second.GetModuleName());
			if (!TStrICmp(szModuleName, argv[1]))
			{
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.ExpandSDSModule(&it->second);
				gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
				break;
			}
		}
	}
	return 0;
}

const unsigned int gCRC32[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

const unsigned int gCRC16[256] = {
	0x0000,	0xc0c1,	0xc181,	0x0140,	0xc301,	0x03c0,	0x0280,	0xc241,
	0xc601,	0x06c0,	0x0780,	0xc741,	0x0500,	0xc5c1,	0xc481,	0x0440,
	0xcc01,	0x0cc0,	0x0d80,	0xcd41,	0x0f00,	0xcfc1,	0xce81,	0x0e40,
	0x0a00,	0xcac1,	0xcb81,	0x0b40,	0xc901,	0x09c0,	0x0880,	0xc841,
	0xd801,	0x18c0,	0x1980,	0xd941,	0x1b00,	0xdbc1,	0xda81,	0x1a40,
	0x1e00,	0xdec1,	0xdf81,	0x1f40,	0xdd01,	0x1dc0,	0x1c80,	0xdc41,
	0x1400,	0xd4c1,	0xd581,	0x1540,	0xd701,	0x17c0,	0x1680,	0xd641,
	0xd201,	0x12c0,	0x1380,	0xd341,	0x1100,	0xd1c1,	0xd081,	0x1040,
	0xf001,	0x30c0,	0x3180,	0xf141,	0x3300,	0xf3c1,	0xf281,	0x3240,
	0x3600,	0xf6c1,	0xf781,	0x3740,	0xf501,	0x35c0,	0x3480,	0xf441,
	0x3c00,	0xfcc1,	0xfd81,	0x3d40,	0xff01,	0x3fc0,	0x3e80,	0xfe41,
	0xfa01,	0x3ac0,	0x3b80,	0xfb41,	0x3900,	0xf9c1,	0xf881,	0x3840,
	0x2800,	0xe8c1,	0xe981,	0x2940,	0xeb01,	0x2bc0,	0x2a80,	0xea41,
	0xee01,	0x2ec0,	0x2f80,	0xef41,	0x2d00,	0xedc1,	0xec81,	0x2c40,
	0xe401,	0x24c0,	0x2580,	0xe541,	0x2700,	0xe7c1,	0xe681,	0x2640,
	0x2200,	0xe2c1,	0xe381,	0x2340,	0xe101,	0x21c0,	0x2080,	0xe041,
	0xa001,	0x60c0,	0x6180,	0xa141,	0x6300,	0xa3c1,	0xa281,	0x6240,
	0x6600,	0xa6c1,	0xa781,	0x6740,	0xa501,	0x65c0,	0x6480,	0xa441,
	0x6c00,	0xacc1,	0xad81,	0x6d40,	0xaf01,	0x6fc0,	0x6e80,	0xae41,
	0xaa01,	0x6ac0,	0x6b80,	0xab41,	0x6900,	0xa9c1,	0xa881,	0x6840,
	0x7800,	0xb8c1,	0xb981,	0x7940,	0xbb01,	0x7bc0,	0x7a80,	0xba41,
	0xbe01,	0x7ec0,	0x7f80,	0xbf41,	0x7d00,	0xbdc1,	0xbc81,	0x7c40,
	0xb401,	0x74c0,	0x7580,	0xb541,	0x7700,	0xb7c1,	0xb681,	0x7640,
	0x7200,	0xb2c1,	0xb381,	0x7340,	0xb101,	0x71c0,	0x7080,	0xb041,
	0x5000,	0x90c1,	0x9181,	0x5140,	0x9301,	0x53c0,	0x5280,	0x9241,
	0x9601,	0x56c0,	0x5780,	0x9741,	0x5500,	0x95c1,	0x9481,	0x5440,
	0x9c01,	0x5cc0,	0x5d80,	0x9d41,	0x5f00,	0x9fc1,	0x9e81,	0x5e40,
	0x5a00,	0x9ac1,	0x9b81,	0x5b40,	0x9901,	0x59c0,	0x5880,	0x9841,
	0x8801,	0x48c0,	0x4980,	0x8941,	0x4b00,	0x8bc1,	0x8a81,	0x4a40,
	0x4e00,	0x8ec1,	0x8f81,	0x4f40,	0x8d01,	0x4dc0,	0x4c80,	0x8c41,
	0x4400,	0x84c1,	0x8581,	0x4540,	0x8701,	0x47c0,	0x4680,	0x8641,
	0x8201,	0x42c0,	0x4380,	0x8341,	0x4100,	0x81c1,	0x8081,	0x4040
};

const unsigned short gCRC16CCITT[256] = {
    0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
    0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
    0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
    0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
    0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
    0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
    0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
    0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
    0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
    0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
    0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
    0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
    0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
    0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
    0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
    0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
    0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
    0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
    0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
    0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
    0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
    0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
    0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
    0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
    0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
    0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
    0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
    0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
    0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
    0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
    0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
    0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

int crc_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc != 3)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : %s address size\n"), argv[0]);
		return 1;
	}

	ULONG_PTR Address = 0;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : %s address size\n"), argv[0]);
		return 1;
	}

	ULONG_PTR Size = 0;
	if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Size))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : %s address size\n"), argv[0]);
		return 1;
	}

	if (!TStrICmp(argv[0], "crc16-ccitt"))
	{
		unsigned short crc16ccitt = -1;
		while (Size>0)
		{
			unsigned char byte = 0;
			if (!pSyser->m_pDebugger->ReadMemory(Address, &byte, 1))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Syser : read memory failed, Address is " F0ADDR "X!\n"), Address);
				break;
			}
			++Address;
			--Size;
			crc16ccitt = gCRC16CCITT[(unsigned char)(argc ^ (crc16ccitt>>8))] ^ (crc16ccitt << 8);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("CRC16/ccitt %04X\n"), crc16ccitt);
	} else
	if (!TStrICmp(argv[0], "crc16"))
	{
		unsigned short crc16 = 0;
		while (Size>0)
		{
			unsigned char byte = 0;
			if (!pSyser->m_pDebugger->ReadMemory(Address, &byte, 1))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Syser : read memory failed, Address is " F0ADDR "X!\n"), Address);
				break;
			}
			++Address;
			--Size;
			crc16 = gCRC16[(unsigned char)(crc16 ^ byte)] ^ (crc16 >> 8);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("CRC16 %04X\n"), crc16);
	} else
	if (!TStrICmp(argv[0], "crc32"))
	{
		unsigned int crc32 = -1;
		while (Size>0)
		{
			unsigned char byte = 0;
			if (!pSyser->m_pDebugger->ReadMemory(Address, &byte, 1))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Syser : read memory failed, Address is " F0ADDR "X!\n"), Address);
				break;
			}
			++Address;
			--Size;
			crc32 = gCRC32[(unsigned char)byte ^ (unsigned char)crc32] ^ (crc32 >> 8);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("CRC32 %08X\n"), ~crc32);
	}
	return 0;
}


SYSER_CMD_ENTRY OtherCmdTable[] =
{
	{ WSTR("resize"), WSTR("resize syser windows"), resize_command,
	0},
	{ WSTR("lines"), WSTR("Change the number of lines for the syser display."), lines_command,
	0},
	{ WSTR("width"), WSTR("Set the number of display columns in the syser window."), width_command,
	0},
	{ WSTR("ioctl"), WSTR("Decodes the IOCTL and FSCTL codes into readable English."), ioctl_command,
	0},
	{ WSTR("ntstatus"), WSTR("Display header-defined mnemonics for NTSTATUS error codes."), ntstatus_command,
	0},
	{ WSTR("winerror"), WSTR("Display header-defined mnemonics for Win32 error codes."), winerror_command,
	0},
	{ WSTR("bugcheckcode"), WSTR("Display Bug Check Code mnemonics for windows."), bugcheckcode_command,
	0},
	{ WSTR("wmsg"), WSTR("Display the names and message numbers of Windows messages."), wmsg_command,
	0},
	{ WSTR("zap"), WSTR("Replace an embedded INT 1 or INT 3 with a NOP instruction."), zap_command, 
	0},
	{ WSTR("file"), WSTR("List source code file"), file_command,
	0},
	{ WSTR("watch"), WSTR("Add source code variable watch"), watch_command,
	0},
	{ WSTR("table"), WSTR("List source code module"), table_command,
	0},
	{ WSTR("crc16"), WSTR("Calculate crc16 value with memory region."), crc_command,
	0},
	{ WSTR("crc32"), WSTR("Calculate crc32 value with memory region."), crc_command,
	0},
	{ WSTR("crc16-ccitt"), WSTR("Calculate crc-ccitt value with memory region."), crc_command,
	0},
	{0},
};
