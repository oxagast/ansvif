@echo off
FOR /F "tokens=* USEBACKQ" %%F IN (`dir "C:\Program Files\mingw-w64\*posix-seh*" /b /S`) DO ( 
SET mingw=%%F
)
set PATH=%mingw%\mingw64\bin;%PATH%
rem echo %PATH%
@echo on
"windres.exe" bin\win\metadata.rc -O coff -o metadata.res
"g++.exe" src/common.cpp src/bin2hex.cpp src/popen2.cpp src/main.cpp src/help.cpp src/match_fault.cpp src/sys_string.cpp src/man_read.cpp src/randomizer.cpp src/trash.cpp src/log.cpp metadata.res src/version.h -I./ -I./include -std=c++11 -lstdc++ -lpthread -O2 -o ansvif.exe -static -static-libgcc -static-libstdc++
"gcc.exe" src/win/printf.c -o printf.exe
