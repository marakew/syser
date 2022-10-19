
#include "resource.hpp"

WISP_MENU_GROUP_ITEM SystemExplorerCharacterSetTablesMenu[] =
{
	{WSTR("IBM ASCII"), EVENT_ID_IBM, 0x67, 0, 0},
	{WSTR("ANSI ASCII"), EVENT_ID_ANSI, 0x67, 0, 0},
	{WSTR("EBCDIC"), EVENT_ID_EBCDIC, 0x67, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerRecentFilesMenu[] =
{
	{WSTR("Empty"), EVENT_ID_FILE0, 0x84, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerFileMenu[] =
{
	{WSTR("Open"), EVENT_ID_OPEN, 8, 0x100000, 0},
	{WSTR("Close"), EVENT_ID_CLOSE, 9, 0x100000, 0},
	{WSTR("Load Symbol Files"), EVENT_ID_LOADSYM, 0x0A, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Recent Files"), 0, 0x24, 1, SystemExplorerRecentFilesMenu},
	{0, 0, 0, 4, 0},
	{WSTR("Save Comments"), EVENT_ID_SAVECMT, 0x93, 0x100000, 0},
	{WSTR("Load Comments"), EVENT_ID_LOADCMT, 0x93, 0x100000, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerEditMenu[] =
{
	{WSTR("Undo"), EVENT_ID_EDIT_UNDO, 0xC8, 0, 0},
	{WSTR("Redo"), EVENT_ID_EDIT_REDO, 0xC9, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Cut"), EVENT_ID_EDIT_CUT, 0xC5, 0, 0},
	{WSTR("Copy"), EVENT_ID_EDIT_COPY, 0xC6, 0, 0},
	{WSTR("Paste"), EVENT_ID_EDIT_PASTE, 0xC7, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Find"), EVENT_ID_EDIT_FIND, 0xC, 0, 0},
	{WSTR("Relpace"), EVENT_ID_EDIT_REPLACE, 0x10E, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Go to...    Ctrl+G"), EVENT_ID_EDIT_GOTO, 0xE8, 0, 0},
	{0}
};

WISP_MENU_GROUP_ITEM SystemExplorerDebugMenu[] =
{
#ifndef _RING0_
	{WSTR("Reset"), EVENT_ID_DBG_RESET, 0x10, 0, 0},
#endif
	{WSTR("Continue      F5"), EVENT_ID_GO, 3, 0, 0},
	{WSTR("Return        F6"), EVENT_ID_DBG_RETURN, 2, 0, 0},
	{WSTR("Step          F8"), EVENT_ID_DBG_STEP, 0, 0, 0},
	{WSTR("Proceed       F10"), EVENT_ID_DBG_PROCEED, 1, 0, 0},
	{WSTR("Step Branch   F11"), EVENT_ID_DBG_STEPB, 0, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Break Point   Ctrl+F9"), EVENT_ID_BREAK_POINT, 4, 0, 0},
	{WSTR("Enable All Breakpoints"), EVENT_ID_BP_ENABLE_ALL, 0xF3, 0, 0},
	{WSTR("Disable All Breakpoints"), EVENT_ID_BP_DISABLE_ALL, 0xF4, 0, 0},
	{WSTR("Remove All Breakpoints"), EVENT_ID_BP_REMOVE_ALL, 0xF2, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerMonitorMenu[] =
{
	{WSTR("Register View                 Alt+G"), EVENT_ID_REGVIEW, 0x34, 0, 0},
	{WSTR("Data View                     Alt+D"), EVENT_ID_DATAVIEW, 0x59, 0, 0},
	{WSTR("Call Stack                    Alt+C"), EVENT_ID_CALLSTACK, 0x12, 0, 0},
	{WSTR("Run Trace List"), EVENT_ID_RUNTRACE, 0xB5, 0, 0},
	{WSTR("Comment List"), EVENT_ID_CMTLIST, 0x93, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerViewMenu[] =
{
	{WSTR("Save Position                 Shift+Enter"), EVENT_ID_SAVEPOS, 0x18, 0, 0},
	{WSTR("Jump To Previous Position     Backspace"), EVENT_ID_PREVPOS, 0x15, 0, 0},
	{WSTR("Jump To Next Position         Ctrl+Enter"), EVENT_ID_NEXTPOS, 0x14, 0, 0},
	{WSTR("Empty Position List"), EVENT_ID_CLRPOS, 0x26, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Toggle   BookMark             Ctrl+K"), EVENT_ID_TOGGLE_BM, 0xD0, 0, 0},
	{WSTR("Previous BookMark             Ctrl+P"), EVENT_ID_PREV_BM, 0xD2, 0, 0},
	{WSTR("Next     BookMark             Ctrl+N"), EVENT_ID_NEXT_BM, 0xD1, 0, 0},
	{WSTR("Clear    BookMark             Ctrl+L"), EVENT_ID_CLEAR_BM, 0xD3, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerPlugins[] =
{
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerOptionMenu[] =
{
	{WSTR("Option"), EVENT_ID_CONFIG_OPTION, 0x33, 0, 0},
	{WSTR("Debugger Select"), EVENT_ID_DBG_LIST, 0x35, 0, 0},
	{WSTR("Plugin List"), EVENT_ID_PLUGINS, 0x1D, 0, 0},
	{WSTR("Plugins"), EVENT_ID_PLUGINS, 0x1D, 1, SystemExplorerPlugins}, //++ ??? TODO
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerToolsMenu[] =
{
	{WSTR("Memory Search"), EVENT_ID_MEMSEARCH, 0xDF, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Calculator"), EVENT_ID_CALC, 6, 0, 0},
	{WSTR("Soft Keyboard"), EVENT_ID_SOFTKB, 7, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("PE Explorer"), EVENT_ID_PE_EXPLORER, 0x0E, 0, 0},
	{WSTR("Character Set Tables"), 0, 0x24, 1, SystemExplorerCharacterSetTablesMenu},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerHelpMenu[] =
{
	{WSTR("Register Help"), EVENT_ID_REG_HELP, 0x34, 0, 0},
#ifndef _RING0_
	{WSTR("Check New Version"), EVENT_ID_CHECK_NEW_VER, 0x68, 0, 0},
#endif
	{WSTR("About Syser"), EVENT_ID_ABOUT, 0x3E, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerInformationMenu[] =
{
//#ifdef _RING0_
	{WSTR("IDT"), EVENT_ID_IDT, 0x1A, 0, 0},
	{WSTR("GDT"), EVENT_ID_GDT, 0x1A, 0, 0},
	{WSTR("LDT"), EVENT_ID_LDT, 0x1A, 0, 0},
	{WSTR("Page Map"), EVENT_ID_PAGEMAP, 0x33, 0, 0},
	{0, 0, 0, 4, 0},
//#endif
	{WSTR("String References"), EVENT_ID_STRREF, 0x0F, 0, 0},
	{WSTR("Processes"), EVENT_ID_PROCESS, 0x0F, 0, 0},
	{WSTR("Threads"), EVENT_ID_THREADS, 0xD4, 0, 0},
	{WSTR("Modules"), EVENT_ID_MODULES, 0xDC, 0, 0},
	{WSTR("Functions"), EVENT_ID_FUNCTIONS, 0xDC, 0, 0},
//	{WSTR("SEH chain"), EVENT_ID_SEH, 0xDC, 0, 0}, //???
#ifdef _RING0_
	{WSTR("Object Directory"), EVENT_ID_OBJDIR, 0xDC, 0, 0}, //???
	{WSTR("HWND"), EVENT_ID_HWND, 0xDC, 0, 0},
#endif
	{0},
};

WISP_MENU_GROUP_ITEM SystemExplorerMainMenu[] =
{
#ifndef _RING0_
	{WSTR("File"), 0, 0x3D, 1, SystemExplorerFileMenu},
#endif
//TODO	{WSTR("Edit"), 0, 0x3D, 1, SystemExplorerEditMenu},
	{WSTR("Debug"), 0, 0x39, 1, SystemExplorerDebugMenu},
	{WSTR("Monitor"), 0, 0x35, 1, SystemExplorerMonitorMenu},
	{WSTR("View"), 0, 0x7E, 1, SystemExplorerViewMenu},
	{WSTR("Information"), 0, 0x5F, 1, SystemExplorerInformationMenu},
#ifndef _RING0_
	{WSTR("Option"), 0, 0x33, 1, SystemExplorerOptionMenu},
#endif
	{WSTR("Tools"), 0, 0x76, 1, SystemExplorerToolsMenu},
	{WSTR("Help"), 0, 0x1E, 1, SystemExplorerHelpMenu},
	{0},
};

WISP_TOOLBAR_RES_ITEM SystemExplorerToolbar[] =
{
#ifndef _RING0_
	{EVENT_ID_OPEN, 8, 0, WSTR("File -> Open"), 0, 0x200},
	{EVENT_ID_CLOSE, 9, 0, WSTR("File -> Close"), 0, 0x200},
	{EVENT_ID_DBG_RESET, 0x10, 0, WSTR("Debug -> Reset"), 0, 0x200},
	{0, 0, 0x10000, 0, 0, 0},
#endif
	{EVENT_ID_DBG_STEP, 0, 0, WSTR("Debug -> Step F8"), 0, 0},
	{EVENT_ID_DBG_PROCEED, 1, 0, WSTR("Debug -> Proceed F10"), 0, 0},
	{EVENT_ID_DBG_RETURN, 2, 0, WSTR("Debug -> Return F6"), 0, 0},
	{EVENT_ID_GO, 3, 0, WSTR("Debug -> Continue F5"), 0, 0},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_SAVEPOS, 0x18, 0, WSTR("View -> Save Position     Shift+Enter"), 0, 0x200},
	{EVENT_ID_PREVPOS, 0x15, 0, WSTR("View -> Jump To Previous Position    Backspace<-"), 0, 0x200},
	{EVENT_ID_NEXTPOS, 0x14, 0, WSTR("View -> Jump To Next Position    Ctrl+Enter"), 0, 0x200},
	{EVENT_ID_CLRPOS, 0x26, 0, WSTR("View -> Empty Position List"), 0, 0x200},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_BREAK_POINT, 4, 0, WSTR("Debug -> Break point Ctrl+F9"), 0, 0},
	{EVENT_ID_CALLSTACK, 0x12, 0, WSTR("Monitor -> Call Stack ALT+C"), 0, 0},
	{EVENT_ID_RUNTRACE, 0xB5, 0, WSTR("Monitor -> Run Trace List"), 0, 0},
	{EVENT_ID_CMTLIST, 0x93, 0, WSTR("Monitor -> Comment List"), 0, 0},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_TOGGLE_BM, 0xD0, 0, WSTR("View -> Toggle BookMark Ctrl+K"), 0, 0x200},
	{EVENT_ID_PREV_BM, 0xD2, 0, WSTR("View -> Previous BookMark Ctrl+P"), 0, 0x200},
	{EVENT_ID_NEXT_BM, 0xD1, 0, WSTR("View -> Next BookMark Ctrl+N"), 0, 0x200},
	{EVENT_ID_CLEAR_BM, 0xD3, 0, WSTR("View -> Clear BookMark Ctrl+L"), 0, 0x200},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_MEMSEARCH, 0xDF, 0, WSTR("Tools -> Memory Search"), 0, 0},
	{EVENT_ID_SOFTKB, 7, 0, WSTR("Tools -> Soft keyboard"), 0, 0},
	{EVENT_ID_INSTRINFO, 0, 0x20001, WSTR("Instruction Information"), 0x12C, 0x4040242},
	{WISP_ID_STATIC_DIB, 0x1E, 0x20002, 0, 0, 0},
	{0},
};

//SourceDebug

WISP_MENU_GROUP_ITEM SourceDebugDebugMenu[] =
{
	{WSTR("Continue      F5"), EVENT_ID_GO, 3, 0x100000, 0},
	{WSTR("Return        F6"), EVENT_ID_DBG_RETURN, 2, 0x100000, 0},
	{WSTR("Step          F8"), EVENT_ID_DBG_STEP, 0, 0x100000, 0},
	{WSTR("Proceed       F10"), EVENT_ID_DBG_PROCEED, 1, 0x100000, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Break point   Ctrl+F9"), EVENT_ID_BREAK_POINT, 4, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SourceDebugMonitorMenu[] =
{
	{WSTR("Register View                 Alt+G"), EVENT_ID_REGVIEW, 0x34, 0, 0},
	{WSTR("Data View                     Alt+D"), EVENT_ID_DATAVIEW, 0x59, 0, 0},
	{WSTR("Call Stack                    Alt+C"), EVENT_ID_CALLSTACK, 0x12, 0, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SourceDebugViewMenu[] =
{
	{WSTR("Toggle   BookMark    Ctrl+K"), EVENT_ID_TOGGLE_BM, 0xD0, 0x100000, 0},
	{WSTR("Next     BookMark    Ctrl+N"), EVENT_ID_NEXT_BM, 0xD1, 0x100000, 0},
	{WSTR("Previous BookMark    Ctrl+P"), EVENT_ID_PREV_BM, 0xD2, 0x100000, 0},
	{WSTR("Clear    BookMark    Ctrl+L"), EVENT_ID_CLEAR_BM, 0xD3, 0x100000, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Go To Line           Ctrl+G"), EVENT_ID_SOURCECODEWND_GOTOLINE, 0x10B, 0x100000, 0},
	{0},
};

WISP_MENU_GROUP_ITEM SourceDebugMainMenu[] =
{
	{WSTR("Debug"), 0, 0x39, 1, SourceDebugDebugMenu},
	{WSTR("Monitor"), 0, 0x35, 1, SourceDebugMonitorMenu},
	{WSTR("View"), 0, 0x7E, 1, SourceDebugViewMenu},
	{0},
};

WISP_TOOLBAR_RES_ITEM SourceDebugToolbar[] =
{
	{EVENT_ID_DBG_STEP, 0, 0, WSTR("Step"), 0, 0x200},
	{EVENT_ID_DBG_PROCEED, 1, 0, WSTR("Proceed"), 0, 0x200},
	{EVENT_ID_DBG_RETURN, 2, 0, WSTR("Return"), 0, 0x200},
	{EVENT_ID_GO, 3, 0, WSTR("Continue"), 0, 0x200},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_BREAK_POINT, 4, 0, WSTR("Break point"), 0, 0},
	{EVENT_ID_SOURCECODEWND_GOTOLINE, 0x10B, 0, WSTR("Go To Line Ctrl+G"), 0, 0x200},
	{0, 0, 0x10000, 0, 0, 0},
	{EVENT_ID_TOGGLE_BM, 0xD0, 0, WSTR("Toggle BookMark Ctrl+K"), 0, 0x200},
	{EVENT_ID_NEXT_BM, 0xD1, 0, WSTR("Next BookMark Ctrl+N"), 0, 0x200},
	{EVENT_ID_PREV_BM, 0xD2, 0, WSTR("Previous BookMark Ctrl+P"), 0, 0x200},
	{EVENT_ID_CLEAR_BM, 0xD3, 0, WSTR("Clear BookMark Ctrl+L"), 0, 0x200},
	{0},
};

//