
reverse engineering old syser win32 debugger<br>
improved handle x64 arch and fix some bugs<br>

struct viewer<br>
note: pdb2sds.exe .pdb .sds must be same arch!<br>
for source level debugging don't convert .exe to .sds<br>
syser debugger will automatic detect .pdb for .exe<br>
pdb2sds.exe while convert doest include source level debug info<br>
only structs and variables<br>

convert any .pdb or .exe by util pdb2sds.exe<br>
load .sds file from syser menu<br>
Menu "File"->"Load Symbol File"<br>
check from console by command<br>
>sdsmod<br>
File BP(s) Size(k) CheckSum TimeStamp ModuleName<br>
0          0k 00000001 00000001  testx64.pdb<br>
1 Module(s)<br>

start debug .exe<br>
open popup menu on data window by right mouse button<br>
choose a struct by select it<br>
choose "Attach type to Addr"<br>
choose "Lock Type"<br>
enter at "Address" address of struct<br>

[sources](source)<br>

<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/struct_viewer.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/attach_struct.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64src1.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64src.png">
<img width="100%" src="https://raw.githubusercontent.com/marakew/syser/master/images/SyserDebuggerx64.png">
