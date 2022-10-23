cmake_minimum_required(VERSION 3.22)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/msvc-tools.cmake)

project(syser CXX C ASM_MASM RC)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/msvc-showcmd.cmake)

# copy elision break intrusive containers
set(CMAKE_CXX_STANDARD 98)

set(ADDITION_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../addition")
set(DIASDK_INCLUDE ${ADDITION_PATH}/DiaSDK/include)
if(PLATFORM STREQUAL "x32")
set(DIASDK_LIBRARY ${ADDITION_PATH}/DiaSDK/lib/diaguids.lib)
elseif(PLATFORM STREQUAL "x64")
set(DIASDK_LIBRARY ${ADDITION_PATH}/DiaSDK/lib/amd64/diaguids.lib)
endif()

add_library(DIASDK UNKNOWN IMPORTED)
set_target_properties(DIASDK PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${DIASDK_INCLUDE}")
set_target_properties(DIASDK PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES "C" IMPORTED_LOCATION "${DIASDK_LIBRARY}")
set_property(TARGET DIASDK APPEND PROPERTY INTERFACE_LINK_LIBRARIES ole32)
set_property(TARGET DIASDK APPEND PROPERTY INTERFACE_LINK_LIBRARIES oleaut32)

# debugger

file(GLOB_RECURSE SD_SOURCES
	Code/strmem.cpp
	Code/sysdep.cpp
	Code/fileio.cpp
	Code/fileiont.cpp
	EXEAnalyzer/symbol.cpp
	EXEAnalyzer/debugger.cpp
	EXEAnalyzer/x86debugger.cpp
	EXEAnalyzer/sdwin.cpp
)

file(GLOB_RECURSE SOURCES 

# syser
	Code/heap.cpp
	Code/strmem.cpp
	Code/time.cpp
	Code/modf.cpp
	Code/sysdep.cpp
	Code/cmdparser.cpp
	Code/options.cpp
	Code/handle.cpp
	Code/fileio.cpp
	Code/fileiont.cpp
	Code/txtfile.cpp
	Code/srctxtfile.cpp
	Code/utxtfile.cpp
	Code/imagefile.cpp
	Code/compress.cpp
	Code/pagemem.cpp
	Code/polyfs.cpp

	EXEAnalyzer/inteldis.cpp
	EXEAnalyzer/instrsym.cpp
	EXEAnalyzer/pefile.cpp
	EXEAnalyzer/symbol.cpp
	EXEAnalyzer/debugger.cpp

	EXEAnalyzer/sdsmodule.cpp 

	Wisp/utility.cpp
	Wisp/dibdata.cpp
	Wisp/wispdraw.cpp
	Wisp/wispfont.cpp
	Wisp/wispdib.cpp
	Wisp/wispdc.cpp
	Wisp/wispdrawobj.cpp
	Wisp/wispbase.cpp
	Wisp/wispapp.cpp
	Wisp/wispbasewnd.cpp
	Wisp/wispwnd.cpp
	Wisp/wisptipwnd.cpp
	Wisp/wispbutton.cpp
	Wisp/wispstatic.cpp
	Wisp/wispprogress.cpp
	Wisp/wispsoftkeyboard.cpp
	Wisp/wisptoolbar.cpp
	Wisp/wisptabwnd.cpp
	Wisp/wispmultitabview.cpp
	Wisp/wispform.cpp
	Wisp/wispprogressform.cpp
	Wisp/wispcolorstrwnd.cpp
	Wisp/wispconsolewnd.cpp
	Wisp/wisptestwnd.cpp
	Wisp/wispradiobox.cpp
	Wisp/wispcheckbox.cpp
	Wisp/wispsplitwnd.cpp
	Wisp/wispmenu.cpp
	Wisp/wispmsgbox.cpp
	Wisp/wisphexwnd.cpp
	Wisp/wisprgbselect.cpp
	Wisp/wispedit.cpp
	Wisp/wisplist.cpp
	Wisp/wispcombobox.cpp
	Wisp/wispoptionform.cpp
	Wisp/wispcalcwnd.cpp

	WispSyser/resource.cpp
	WispSyser/consolewnd.cpp
	WispSyser/terminalwnd.cpp
	WispSyser/syserui.cpp
	WispSyser/peexplorerform.cpp
	WispSyser/codeview.cpp
	WispSyser/multicodeview.cpp
	WispSyser/dataview.cpp
	WispSyser/dataviewform.cpp
	WispSyser/dataoperatordlg.cpp
	WispSyser/multidataview.cpp
	WispSyser/watchlist.cpp
	WispSyser/stackwatchlist.cpp
	WispSyser/findstringform.cpp
	WispSyser/findlistwnd.cpp
	WispSyser/breakpointform.cpp
	WispSyser/codebpedit.cpp
	WispSyser/databpedit.cpp
	WispSyser/processlist.cpp
	WispSyser/debuggerselectform.cpp
	WispSyser/pluginlistform.cpp
	WispSyser/syseroptionform.cpp
	WispSyser/systemexplorer.cpp
	WispSyser/searchdialog.cpp
	WispSyser/runtracewnd.cpp
	WispSyser/runtrace.cpp

	WispSyser/modulelist.cpp
	WispSyser/commentlist.cpp
	WispSyser/callstackwnd.cpp
	WispSyser/functionlistwnd.cpp
	WispSyser/genereglist.cpp
	WispSyser/fpuregisterlist.cpp
	WispSyser/sseregisterlist.cpp
	WispSyser/sdsmodulelist.cpp
	WispSyser/strinputwnd.cpp
	WispSyser/currentfunction.cpp
	WispSyser/sourcecodewnd.cpp
	WispSyser/lex.cpp
	WispSyser/multisourcecodeview.cpp
	WispSyser/typeviewerwnd.cpp
	WispSyser/typeviewerform.cpp
	WispSyser/sourcedebugframewnd.cpp
	WispSyser/aboutform.cpp
	WispSyser/keymappingpage.cpp
	WispSyser/crossreferenceform.cpp
	WispSyser/mainframe.cpp
	WispSyser/idtwnd.cpp
	WispSyser/gdtwnd.cpp
	WispSyser/reghelpwnd.cpp
	WispSyser/pagemapwnd.cpp
	WispSyser/syserconfig.cpp
	WispSyser/sysercmd.cpp
	WispSyser/sharecmd.cpp
	WispSyser/othercmd.cpp
	WispSyser/ioctl.cpp
	WispSyser/ntstatus.cpp
	WispSyser/winerror.cpp
	WispSyser/bugcheckcode.cpp
	WispSyser/windowsmsg.cpp
)

file(GLOB_RECURSE USERMODE_SOURCES
	EXEAnalyzer/sourcedebug.cpp

	WispSyser/winwisp.cpp
	WispSyser/SyserRing3.cpp
	resource.rc
	syser.manifest
)
list(APPEND SOURCES ${USERMODE_SOURCES})

if(PLATFORM STREQUAL "x32")
  list(APPEND SOURCES
	WispSyser/x87x86double.asm
	WispSyser/sehchainwnd.cpp
  )
elseif(PLATFORM STREQUAL "x64")
  list(APPEND SOURCES
	WispSyser/x87x64double.asm
  )
endif()

add_executable(syser${PLATFORM} WIN32 ${SOURCES})

target_link_libraries(syser${PLATFORM}
	DIASDK
)

target_link_libraries(syser${PLATFORM}
	psapi
)

if(PLATFORM STREQUAL "x64")
	target_compile_options(syser${PLATFORM} PUBLIC /GS- /D_X64_)
endif()
target_include_directories(syser${PLATFORM} PUBLIC .)

add_library(sd${PLATFORM} SHARED ${SD_SOURCES})
if(PLATFORM STREQUAL "x64")
	target_compile_options(sd${PLATFORM} PUBLIC /GS- /D_X64_)
endif()
target_include_directories(sd${PLATFORM} PUBLIC .)

add_executable(mfcsymbols${PLATFORM}
	Code/fileio.cpp
	Code/fileiont.cpp
	Code/sysdep.cpp
	Code/strmem.cpp
	mfcsymbols.cpp
)
if(PLATFORM STREQUAL "x64")
	target_compile_options(mfcsymbols${PLATFORM} PUBLIC /GS- /D_X64_)
endif()
target_include_directories(mfcsymbols${PLATFORM} PUBLIC .)

add_executable(pdb2sds${PLATFORM}
	EXEAnalyzer/sourcedebug.cpp
	EXEAnalyzer/sdsmodule.cpp
	EXEAnalyzer/pefile.cpp
	Code/strmem.cpp
	Code/fileio.cpp
	Code/fileiont.cpp
	Code/imagefile.cpp
	Code/compress.cpp
	pdb2sds.cpp
)
target_link_libraries(pdb2sds${PLATFORM}
	DIASDK
)
if(PLATFORM STREQUAL "x64")
	target_compile_options(pdb2sds${PLATFORM} PUBLIC /GS- /D_X64_)
endif()
target_include_directories(pdb2sds${PLATFORM} PUBLIC .)

add_executable(unpack${PLATFORM}
	Code/fileio.cpp
	Code/fileiont.cpp
	Code/polyfs.cpp
	Code/handle.cpp
	Code/pagemem.cpp
	Code/imagefile.cpp
	Code/compress.cpp
	unpack.cpp
)
if(PLATFORM STREQUAL "x64")
	target_compile_options(unpack${PLATFORM} PUBLIC /GS- /D_X64_)
endif()
target_include_directories(unpack${PLATFORM} PUBLIC .)
