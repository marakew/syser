
#ifndef _OSDEFINE_HPP_
#define _OSDEFINE_HPP_

#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0500

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

#endif