
# solution to show full command and disable names collision while compilation

# https://stackoverflow.com/questions/57358039/how-to-see-the-underlying-compiler-linker-command-line-with-cmake-nmake

	# another solution to disable 8.3 names generation while compilation
	# http://tips.defun.work/windows-perf.html
	#Run cmd with admin privileges:
	#fsutil behavior set disable8dot3 1 OR  fsutil behavior set disable8dot3 3
	#(1 = all volumes / 3 = all but system colume)  - then reboot.
	#Run cmd again with admin credentials and type:
	#fsutil 8dot3name strip /f /s C:
	#Repeat strip for every drive/partition.

# append after project()

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0 FORCE)
foreach(lang IN ITEMS C CXX)
    foreach(cmd IN ITEMS COMPILE_OBJECT CREATE_SHARED_LIBRARY CREATE_PREPROCESSED_SOURCE CREATE_ASSEMBLY_SOURCE LINK_EXECUTABLE)
        string(REPLACE "${CMAKE_START_TEMP_FILE}" "" CMAKE_${lang}_${cmd} "${CMAKE_${lang}_${cmd}}")
        string(REPLACE "${CMAKE_END_TEMP_FILE}" "" CMAKE_${lang}_${cmd} "${CMAKE_${lang}_${cmd}}")
    endforeach()
endforeach()
