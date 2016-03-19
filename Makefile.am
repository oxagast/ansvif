AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = ansvif find_suid
noinst_LIBRARIES = libgzstream.a
libgzstream_a_SOURCES = includes/gzstream/gzstream.h
ansvif_SOURCES = src/common.cpp src/linux/bin2hex.cpp src/linux/popen2.cpp src/linux/main.cpp src/linux/help.cpp src/match_seg.cpp src/linux/sys_string.cpp src/linux/man_read.cpp
#ansvif_HEADERS = gzstream.h
ansvif_LDADD = -lstdc++ -lgzstream -lz -lpthread -L./include/gzstream
find_suid_SOURCES = src/linux/find_suid.cpp
ansvifdir = bin
AM_CXXFLAGS = -std=gnu++11
