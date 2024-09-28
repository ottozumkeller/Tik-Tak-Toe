@echo off

title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars32.bat" > nul

cl /c /GS- /Gr /O1 /arch:IA32 /cgthreads8 main.c

crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:4196 /truncatefloats /unaligncode /noinitializers /tinyimport /out:tiktak.exe main.obj kernel32.lib

move tiktak*.exe Release\

del main.obj
pause
