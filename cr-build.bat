@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

cl /c /GS- /Gr /FC /O1 /fp:strict /MP /TC /Gy /Ob0 /Zc:inline /Zp8 /fp:except- /GF /MT /std:c++14 /Os main.c

crinkler /tinyheader /priority:idle /subsystem:console /entry:main /ordertries:30000 /unaligncode /noinitializers /unsafeimport /out:tiktak_crinkler.exe /report:report.html main.obj kernel32.lib user32.lib

move tiktak_crinkler.exe Release\
move report.html Reports\

del main.obj
pause
