@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" > nul

cl /c /GS- /Gr /FC /O1 /fp:strict /MP /TC /Gy /Ob0 /Zc:inline /Zp8 /fp:except- /GF /MT /std:c++14 /Os main.c

crinkler /tinyheader /priority:normal /subsystem:console /entry:main /ordertries:32768 /compmode:veryslow /noinitializers /unsafeimport /out:tiktak_crinkler_normal.exe /report:report_normal.html main.obj kernel32.lib user32.lib
crinkler /tinyheader /tinyimport /priority:normal /subsystem:console /entry:main /ordertries:32768 /compmode:veryslow /noinitializers /unsafeimport /out:tiktak_crinkler_tiny.exe /report:report_tiny.html main.obj kernel32.lib user32.lib
move tiktak_crinkler_*.exe Release\
move report_*.html Reports\

del main.obj
pause
