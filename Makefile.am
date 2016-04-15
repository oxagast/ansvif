AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
bin_PROGRAMS = ansvif find_suid faulty
noinst_LIBRARIES = libgzstream.a
libgzstream_a_SOURCES = include/gzstream/gzstream.h include/gzstream/gzstream.C
ansvif_SOURCES = src/common.cpp src/linux/bin2hex.cpp src/linux/bin2hex_pc.cpp src/linux/popen2.cpp src/linux/main.cpp src/linux/help.cpp src/match_seg.cpp src/linux/sys_string.cpp src/linux/sys_string_pc.cpp src/linux/man_read.cpp include/gzstream/gzstream.h
ansvif_LDADD = -lgzstream -lstdc++ -lpthread -lz -L.
ansvif_CPPFLAGS = -O3
ansvif_LDFLAGS = -I./include/gzstream
find_suid_SOURCES = src/linux/find_suid.cpp
faulty_SOURCES = src/faulty.c
faulty_CFLAGS = -ansi
ansvifdir = bin
AM_CXXFLAGS = -Wall

