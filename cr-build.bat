@echo off
title Crinkler
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

cl /c /GS- /Gr /FC /O1 /fp:strict /MP /FA /TC /Gy /Ob0 /Zc:inline /Zp8 /fp:except- /GF /MT /std:c++14 /Os main.c

crinkler /tinyheader /priority:idle /subsystem:console /entry:main /ordertries:30000 /unaligncode /noinitializers /unsafeimport /out:tiktak_crinkler.exe /report:report.html main.obj kernel32.lib user32.lib

move tiktak_crinkler.exe Release\
move report.html Reports\

crinkler /tinyheader /tinyimport /priority:idle /subsystem:console /entry:main /ordertries:30000 /unaligncode /noinitializers /out:tiktak_crinkler_tinyimport.exe /report:report_tinyimport.html main.obj kernel32.lib user32.lib

move tiktak_crinkler_tinyimport.exe Release\
move report_tinyimport.html Reports\

del main.obj
pause
