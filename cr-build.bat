@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars32.bat" > nul

cl /c /GS- /Gr /O1 /Os /TC /Gy /arch:IA32 main.c

crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:8392 /noinitializers /truncatefloats:1 /unsafeimport /out:tiktak_safe.exe main.obj kernel32.lib
crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:8392 /noinitializers /truncatefloats:1 /tinyimport   /out:tiktak.exe      main.obj kernel32.lib

move tiktak*.exe Release\

del main.obj
pause
