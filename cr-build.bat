@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars32.bat" > nul

cl /c /arch:IA32 /GS- /Gr /GA /O1 /fp:fast /TC /Gy /Zc:inline /Zp8 /MT /std:c11 /Os /Oy /cgthreads8 /permissive /W4 main.c


crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:4196 /noinitializers /truncatefloats:1 /unsafeimport /overridealignments:1 /out:tiktak_crinkler_normal.exe main.obj kernel32.lib user32.lib
crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:4196 /noinitializers /truncatefloats:1 /tinyimport /overridealignments:1 /out:tiktak_crinkler_tiny.exe main.obj kernel32.lib user32.lib
move tiktak_crinkler_*.exe Release\
move report_*.html Reports\

del main.obj
pause
