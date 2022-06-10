
reverse engineering old syser win32 debugger<br>
improved handle x64 arch and fix some bugs<br>

struct viewer
convert any .pdb or .exe by util pdb2sds.exe
load .sds file from syser menu
Menu "File"-">Load Symbol File"
check from console by command
>sdsmod
File BP(s) Size(k) CheckSum TimeStamp ModuleName
0          0k 00000001 00000001  testx64.pdb
1 Module(s)

start debug .exe
open popup menu on data window by right mouse button
choose "Attach type to Addr"
choose "Lock Type"
enter at "Address" address of struct

[screenshot of sources](screenshot/readme.md)<br>

[list of sources](list_of_sources)<br>

to buy sources or make it opensource contact to email from syser about<br>

<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/struct_viewer.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/attach_struct.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64src1.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64src.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64.png">
