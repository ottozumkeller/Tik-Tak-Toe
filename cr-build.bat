@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars32.bat" > nul

cl /c /arch:IA32 /GS- /Gr /GA /FC /O1 /fp:strict /TC /Gy /Zc:inline /Zp8 /Zc:ternary /fp:except- /GF /MT /std:c11 /Os /Oy /cgthreads8 /permissive main.c

crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:32768 /noinitializers /truncatefloats:8 /unsafeimport /out:tiktak_crinkler_normal.exe /report:report_normal.html main.obj kernel32.lib user32.lib
crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:32768 /noinitializers /truncatefloats:8 /tinyimport /out:tiktak_crinkler_tiny.exe /report:report_tiny.html main.obj kernel32.lib user32.lib
move tiktak_crinkler_*.exe Release\
move report_*.html Reports\

del main.obj
pause
