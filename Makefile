all: ansvif

default: ansvif

ansvif:
	g++ src/common.cpp src/win/bin2hex.cpp src/win/bin2hex_pc.cpp src/win/popen2.cpp src/win/main.cpp src/win/help.cpp src/match_seg.cpp src/win/sys_string.cpp src/win/sys_string_pc.cpp src/win/man_read.cpp -std=gnu++11 -lstdc++ -lpthread -O3 -o ansvif.exe
	g++ src/win/printf.c -O2 -o printf.exe
	gcc -o faulty.exe ./src/faulty.c

clean:
	rm Makefile ansvif.exe faulty.exe printf.exe src/*.o
