# D:\Dev\cmake-3.22.1-windows-x86_64\bin\cmake.exe -G"NMake Makefiles" --debug-output --trace --trace-expand --debug-trycompile -DCMAKE_BUILD_TYPE=Release .. -v > log 2>&1
# D:\Dev\cmake-3.22.1-windows-x86_64\bin\cmake.exe --build . --target test -v
# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/msvc-tools.cmake)
# for enable all langs by file extension
# project(myproj CXX C ASM_MASM RC)

if(_TOOLCHAIN_INCLUDED)
  return()
endif(_TOOLCHAIN_INCLUDED)
set(_TOOLCHAIN_INCLUDED TRUE)

if(DRIVER)
set(CMAKE_C_COMPILER_WORKS TRUE)
endif()
#set(CMAKE_C_COMPILER_FORCED TRUE)
#set(CMAKE_C_COMPILER_ID_RUN TRUE)
if(DRIVER)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
endif()
#set(CMAKE_CXX_COMPILER_FORCED TRUE)
#set(CMAKE_CXX_COMPILER_ID_RUN TRUE)

function(get_highest_version the_dir the_ver)
  file(GLOB entries LIST_DIRECTORIES true RELATIVE "${the_dir}" "${the_dir}/[0-9.]*")
  foreach(entry ${entries})
    if(IS_DIRECTORY "${the_dir}/${entry}")
      set(${the_ver} "${entry}" PARENT_SCOPE)
    endif()
  endforeach()
endfunction()

set(NINJA_PATH "D:/Dev")

set(CLANG_PATH "D:")
set(CLANG_VER "15.0.3")

set(WINVCROOT "D:/Program Files (x86)/Microsoft Visual Studio/2019/EnterprisePreview")
set(WINSDKROOT "D:/Program Files (x86)/Windows Kits/10")

set(MSVC_VER "14.29.30133")
if (NOT MSVC_VER)
	get_highest_version("D:/Program Files (x86)/Microsoft Visual Studio/2019/EnterprisePreview/VC/Tools/MSVC" MSVC_VER)
endif()
set(VCPATH "D:/Program Files (x86)/Microsoft Visual Studio/2019/EnterprisePreview/VC/Tools/MSVC/${MSVC_VER}")

set(WINSDK_VER "10.0.18362.0")
if (NOT WINSDK_VER)
	get_highest_version("D:/Program Files (x86)/Windows Kits/10/Include" WINSDK_VER)
endif()
set(SDKPATH "D:/Program Files (x86)/Windows Kits/10")

if (NOT MSVC_VER OR NOT WINSDK_VER)
	message(SEND_ERROR "Must specify CMake variable MSVC_VER and WINSDK_VER")
endif()

set(ATLMFC_INCLUDE "${VCPATH}/atlmfc/include")
set(ATLMFC_LIB     "${VCPATH}/atlmfc/lib")
set(MSVC_INCLUDE   "${VCPATH}/include")
set(MSVC_LIB       "${VCPATH}/lib")
set(WINSDK_INCLUDE "${SDKPATH}/Include/${WINSDK_VER}")
set(WINSDK_LIB     "${SDKPATH}/Lib/${WINSDK_VER}")

#message("PLATFORM=${PLATFORM}")
if (NOT DEFINED ENV{PLATFORM})
    set(ENV{PLATFORM} ${PLATFORM})
endif()

if(DEFINED ENV{PLATFORM} AND $ENV{PLATFORM} STREQUAL "x32")

if(CMAKE_GENERATOR MATCHES "Ninja")
  set(CMAKE_MAKE_PROGRAM "${NINJA_PATH}/ninja.exe" CACHE FILEPATH "" FORCE)
else()
  set(CMAKE_MAKE_PROGRAM "${VCPATH}/bin/Hostx64/x86/nmake.exe")
endif()

if(USE_CLANGCL)
  set(CMAKE_C_COMPILER "${CLANG_PATH}/LLVM-${CLANG_VER}-win32/bin/clang-cl.exe")
  set(CMAKE_CXX_COMPILER "${CLANG_PATH}/LLVM-${CLANG_VER}-win32/bin/clang-cl.exe")
else()
  set(CMAKE_C_COMPILER "${VCPATH}/bin/Hostx64/x86/cl.exe")
  set(CMAKE_CXX_COMPILER "${VCPATH}/bin/Hostx64/x86/cl.exe")
endif()
  set(CMAKE_ASM_COMPILER "${VCPATH}/bin/Hostx64/x86/ml.exe")
  set(CMAKE_LINKER "${VCPATH}/bin/Hostx64/x86/link.exe")

  set(CMAKE_RC_COMPILER "${SDKPATH}/bin/${WINSDK_VER}/x86/rc.exe")
  set(CMAKE_MT "${SDKPATH}/bin/${WINSDK_VER}/x86/mt.exe")

elseif(DEFINED ENV{PLATFORM} AND $ENV{PLATFORM} STREQUAL "x64")

if(CMAKE_GENERATOR MATCHES "Ninja")
  set(CMAKE_MAKE_PROGRAM "${NINJA_PATH}/ninja.exe" CACHE FILEPATH "" FORCE)
else()
  set(CMAKE_MAKE_PROGRAM "${VCPATH}/bin/Hostx64/x64/nmake.exe")
endif()

if(USE_CLANGCL)
  set(CMAKE_C_COMPILER "${CLANG_PATH}/LLVM-${CLANG_VER}-win64/bin/clang-cl.exe")
  set(CMAKE_CXX_COMPILER "${CLANG_PATH}/LLVM-${CLANG_VER}-win64/bin/clang-cl.exe")
else()
  set(CMAKE_C_COMPILER "${VCPATH}/bin/Hostx64/x64/cl.exe")
  set(CMAKE_CXX_COMPILER "${VCPATH}/bin/Hostx64/x64/cl.exe")
endif()
  set(CMAKE_ASM_COMPILER "${VCPATH}/bin/Hostx64/x64/ml64.exe")
  set(CMAKE_LINKER "${VCPATH}/bin/Hostx64/x64/link.exe")

  set(CMAKE_RC_COMPILER "${SDKPATH}/bin/${WINSDK_VER}/x64/rc.exe")
  set(CMAKE_MT "${SDKPATH}/bin/${WINSDK_VER}/x64/mt.exe")

else()
  message(FATAL_ERROR "You can not do -DPLATFORM=${PLATFORM} at all, CMake will exit.")
endif()

if(DRIVER)
  include_directories(SYSTEM
    "${SDKPATH}/Include/${WINSDK_VER}/km/crt;"
    "${SDKPATH}/Include/${WINSDK_VER}/shared;"
    "${SDKPATH}/Include/${WINSDK_VER}/km;"
  )
else()
  include_directories(SYSTEM
    "${VCPATH}/include;"
    "${VCPATH}/atlmfc/include;"
    "${SDKPATH}/Include/${WINSDK_VER}/ucrt;"
    "${SDKPATH}/Include/${WINSDK_VER}/shared;"
    "${SDKPATH}/Include/${WINSDK_VER}/um;"
  )
endif()

if(DEFINED ENV{PLATFORM} AND $ENV{PLATFORM} STREQUAL "x32")
  if(DRIVER)
    link_directories(
      "${SDKPATH}/Lib/${WINSDK_VER}/km/x86;"
    )
#  set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES
#	"${SDKPATH}/Lib/${WINSDK_VER}/km/x86;"
#	CACHE STRING "" FORCE)
  else()
    link_directories(
      "${VCPATH}/lib/x86;"
      "${VCPATH}/atlmfc/lib/x86;"
      "${SDKPATH}/Lib/${WINSDK_VER}/ucrt/x86;"
      "${SDKPATH}/Lib/${WINSDK_VER}/um/x86;"
    )
#  set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES
#	"${VCPATH}/lib/x86;"
#	"${VCPATH}/atlmfc/lib/x86;"
#	"${SDKPATH}/Lib/${WINSDK_VER}/ucrt/x86;"
#	"${SDKPATH}/Lib/${WINSDK_VER}/um/x86;"
#	CACHE STRING "" FORCE)
  endif()
#  set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES} CACHE STRING "" FORCE)
elseif(DEFINED ENV{PLATFORM} AND $ENV{PLATFORM} STREQUAL "x64")
  if(DRIVER)
    link_directories(
      "${SDKPATH}/Lib/${WINSDK_VER}/km/x64;"
    )
#  set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES
#	"${SDKPATH}/Lib/${WINSDK_VER}/km/x64;"
#	CACHE STRING "" FORCE)
  else()
    link_directories(
      "${VCPATH}/lib/x64;"
      "${VCPATH}/atlmfc/lib/x64;"
      "${SDKPATH}/Lib/${WINSDK_VER}/ucrt/x64;"
      "${SDKPATH}/Lib/${WINSDK_VER}/um/x64;"
    )
#  set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES
#	"${VCPATH}/lib/x64;"
#	"${VCPATH}/atlmfc/lib/x64;"
#	"${SDKPATH}/Lib/${WINSDK_VER}/ucrt/x64;"
#	"${SDKPATH}/Lib/${WINSDK_VER}/um/x64;"
#	CACHE STRING "" FORCE)
  endif()
#  set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES} CACHE STRING "" FORCE)
endif()

if(DRIVER)
  set(CMAKE_C_STANDARD_LIBRARIES "libcntpr.lib ntstrsafe.lib BufferOverflowK.lib ntoskrnl.lib hal.lib wdm.lib" CACHE STRING "" FORCE)
  set(CMAKE_CXX_STANDARD_LIBRARIES "libcntpr.lib ntstrsafe.lib BufferOverflowK.lib ntoskrnl.lib hal.lib wdm.lib" CACHE STRING "" FORCE)
else()
  set(CMAKE_C_STANDARD_LIBRARIES "kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib ws2_32.lib" CACHE STRING "" FORCE)
  set(CMAKE_CXX_STANDARD_LIBRARIES "kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib ws2_32.lib" CACHE STRING "" FORCE)
endif()

# clang-cl
##string(APPEND _FLAGS_CXX " -fno-strict-aliasing")
##string(APPEND _FLAGS_CXX " -Wno-writable-strings")
##string(APPEND _FLAGS_CXX " -Wno-microsoft-template")
##string(APPEND _FLAGS_CXX " -Wno-deprecated-declarations")

#set(CMAKE_C_FLAGS "/DWIN32 -fms-extensions -fms-compatibility -D_WINDOWS${_Wall}" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS_DEBUG "${_FLAGS_DEBUG} -gline-tables-only -fno-inline -O0 ${_FLAGS_C}" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS_RELEASE "-O2 ${_FLAGS_C}" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "${_FLAGS_DEBUG} -gline-tables-only -O2 -fno-inline ${_FLAGS_C}" CACHE STRING "" FORCE)
#set(CMAKE_C_FLAGS_MINSIZEREL "${_FLAGS_C}" CACHE STRING "" FORCE)

#set(CMAKE_CXX_FLAGS "/DWIN32 -fms-extensions -fms-compatibility -D_WINDOWS${_Wall}" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_DEBUG "${_FLAGS_DEBUG} -gline-tables-only -fno-inline -O0 ${_FLAGS_CXX}" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${_FLAGS_CXX}" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${_FLAGS_DEBUG} -gline-tables-only -O2 -fno-inline ${_FLAGS_CXX}" CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_MINSIZEREL "${_FLAGS_CXX}" CACHE STRING "" FORCE)

set(_FLAGS_DEBUG "/Zi")
#set(_FLAGS_DEBUG "/Z7")
set(_GR "/GR-") # RTTI - disable
set(_GS "/GS-") # Buffer Security Check - disable
#set(_FLAGS_ARCH "/arch:IA32")
#set(_FLAGS_ARCH "/arch:SSE")
#set(_FLAGS_ARCH "/arch:SSE2")
#set(_FLAGS_ARCH "/arch:AVX")
#set(_FLAGS_ARCH "/arch:AVX2")
#set(_FLAGS_ARCH "/arch:AVX512")
set(_FLAGS_CXX "${_GR} /EHsc")
set(_FLAGS_C "")
#set(_W3 " /W3")
set(_W3 "")
string(APPEND _W3 " /W4") # Baseline reasonable warnings
if(NOT USE_CLANGCL)
string(APPEND _W3 " /w14242") # 'identifier': conversion from 'type1' to 'type1', possible loss of data
string(APPEND _W3 " /w14254") # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
string(APPEND _W3 " /w14263") # 'function': member function does not override any base class virtual member function
string(APPEND _W3 " /w14265") # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
				# be destructed correctly
string(APPEND _W3 " /w14287") # 'operator': unsigned/negative constant mismatch
string(APPEND _W3 " /we4289") # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
				# the for-loop scope
string(APPEND _W3 " /w14296") # 'operator': expression is always 'boolean_value'
string(APPEND _W3 " /w14311") # 'variable': pointer truncation from 'type1' to 'type2'
string(APPEND _W3 " /w14545") # expression before comma evaluates to a function which is missing an argument list
string(APPEND _W3 " /w14546") # function call before comma missing argument list
string(APPEND _W3 " /w14547") # 'operator': operator before comma has no effect; expected operator with side-effect
string(APPEND _W3 " /w14549") # 'operator': operator before comma has no effect; did you intend 'operator'?
string(APPEND _W3 " /w14555") # expression has no effect; expected expression with side- effect
string(APPEND _W3 " /w14619") # pragma warning: there is no warning number 'number'
string(APPEND _W3 " /w14640") # Enable warning on thread un-safe static member initialization
string(APPEND _W3 " /w14826") # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
string(APPEND _W3 " /w14905") # wide string literal cast to 'LPSTR'
string(APPEND _W3 " /w14906") # string literal cast to 'LPWSTR'
string(APPEND _W3 " /w14928") # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
endif()
string(APPEND _W3 " /permissive-") # standards conformance mode for MSVC compiler.

if(DRIVER)
  # specifies the __stdcall calling convention for all functions except C++ member functions
  string(APPEND _FLAGS_CXX " /Gz /KERNEL") #/Zc:threadSafeInit- 
  string(APPEND _FLAGS_C " /Gz /KERNEL") #/Zc:threadSafeInit- 
endif()

set(PLATFORM_COMPAT)
if(USE_CLANGCL)
string(APPEND PLATFORM_COMPAT " -fms-extensions -fms-compatibility")
endif()
set(CMAKE_C_FLAGS "/DWIN32 /D_WINDOWS${PLATFORM_COMPAT}${_W3}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG "${_FLAGS_DEBUG} /Ob0 /Od ${_FLAGS_C}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob2 ${_FLAGS_C}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${_FLAGS_DEBUG} /O2 /Ob1 ${_FLAGS_C}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL "/O1 /Ob1 ${_FLAGS_C}" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS${PLATFORM_COMPAT}${_W3}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${_FLAGS_DEBUG} /Ob0 /Od ${_FLAGS_CXX}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 ${_FLAGS_CXX}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${_FLAGS_DEBUG} /O2 /Ob1 ${_FLAGS_CXX}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /Ob1 ${_FLAGS_CXX}" CACHE STRING "" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "")

if(DRIVER)
#  set(CMAKE_C_CREATE_CONSOLE_EXE "/DRIVER /ENTRY:\"DriverEntry\" /SUBSYSTEM:CONSOLE /NODEFAULTLIB" CACHE STRING "" FORCE)
#  set(CMAKE_CXX_CREATE_CONSOLE_EXE "/DRIVER /ENTRY:\"DriverEntry\" /SUBSYSTEM:CONSOLE /NODEFAULTLIB" CACHE STRING "" FORCE)
#  set(CMAKE_C_CREATE_WIN32_EXE "/DRIVER /ENTRY:\"DriverEntry\" /SUBSYSTEM:CONSOLE /NODEFAULTLIB" CACHE STRING "" FORCE)
#  set(CMAKE_CXX_CREATE_WIN32_EXE "/DRIVER /ENTRY:\"DriverEntry\" /SUBSYSTEM:CONSOLE /NODEFAULTLIB" CACHE STRING "" FORCE)

  foreach(t EXE SHARED MODULE)
    set(CMAKE_${t}_LINKER_FLAGS_INIT "/DRIVER /ENTRY:\"DriverEntry\" /SUBSYSTEM:CONSOLE /NODEFAULTLIB" CACHE STRING "" FORCE)
  endforeach()
   set(CMAKE_EXECUTABLE_SUFFIX ".sys" CACHE STRING "" FORCE)
   set_property(GLOBAL PROPERTY SUFFIX ".sys")
   set_property(TARGET PROPERTY SUFFIX ".sys")

   set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
   set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif()

# Modules\Platform\Windows.cmake
#set(CMAKE_START_TEMP_FILE "" CACHE STRING "" FORCE)
#set(CMAKE_END_TEMP_FILE "" CACHE STRING "" FORCE)
#set(CMAKE_VERBOSE_MAKEFILE 1 CACHE STRING "" FORCE)

#set(CMAKE_C_COMPILER_ID_FLAGS " /link XXX")
#set(CMAKE_CXX_COMPILER_ID_FLAGS " /link XXX")

#set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "marakew_c_imp_inc_dirs" CACHE STRING "" FORCE)
#set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "marakew_cxx_imp_inc_dirs" CACHE STRING "" FORCE)
#set(_CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES_INIT "marakew_c_imp_inc_dirs1" CACHE STRING "" FORCE)
#set(_CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES_INIT "marakew_cxx_imp_inc_dirs1" CACHE STRING "" FORCE)

#libs

#set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "marakew_c_imp_link_dirs" CACHE STRING "" FORCE)
#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "marakew_cxx_imp_link_dirs" CACHE STRING "" FORCE)

#set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "marakew_c_imp_link_libs" CACHE STRING "" FORCE)
#set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "marakew_cxx_imp_link_libs" CACHE STRING "" FORCE)

set(CMAKE_C_COMPILER_ID_LINK_FLAGS_ALWAYS " /link " CACHE STRING "" FORCE)
foreach(dir ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES})
        string(REPLACE "/" "\\" dir ${dir})
	string(APPEND CMAKE_C_COMPILER_ID_LINK_FLAGS_ALWAYS " -LIBPATH:\"${dir}\"")# CACHE STRING "" FORCE)
endforeach()
#string(APPEND CMAKE_C_COMPILER_ID_FLAGS_ALWAYS " /Tc")
#string(PREPEND CMAKE_C_COMPILER_ID_LINK_FLAGS_ALWAYS "/link")

set(CMAKE_CXX_COMPILER_ID_LINK_FLAGS_ALWAYS " /link " CACHE STRING "" FORCE)
foreach(dir ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
        string(REPLACE "/" "\\" dir ${dir})
	string(APPEND CMAKE_CXX_COMPILER_ID_LINK_FLAGS_ALWAYS " -LIBPATH:\"${dir}\"")# CACHE STRING "" FORCE)
endforeach()
#string(APPEND CMAKE_CXX_COMPILER_ID_FLAGS_ALWAYS " /Tp")
#string(PREPEND CMAKE_CXX_COMPILER_ID_LINK_FLAGS_ALWAYS "/link")
