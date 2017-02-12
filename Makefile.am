AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
#SUBDIRS = tests
if WITH_SYSCALLS
bin_PROGRAMS = ansvif find_suid faulty syscalls
else
bin_PROGRAMS = ansvif find_suid faulty
endif
noinst_LIBRARIES = libgzstream.a
libgzstream_a_SOURCES = include/gzstream/gzstream.h include/gzstream/gzstream.C
ansvif_SOURCES = src/common.cpp src/linux/bin2hex.cpp src/linux/bin2hex_pc.cpp src/linux/popen2.cpp src/linux/main.cpp src/linux/help.cpp src/match_seg.cpp src/linux/sys_string.cpp src/linux/sys_string_pc.cpp src/linux/man_read.cpp src/trash.cpp src/randomizer.cpp include/gzstream/gzstream.h include/xmlwriter/xml_writer.hpp src/linux/main.h
if OS_IS_REDHAT
ansvif_LDADD = -lgzstream -lcryptopp -lstdc++ -lpthread -lz -L.
else
ansvif_LDADD = -lgzstream -lcrypto++ -lstdc++ -lpthread -lz -L.
endif
if OS_IS_REDHAT
ansvif_CPPFLAGS = -O3 -D __REDHAT
else
ansvif_CPPFLAGS = -O3 -D __DEBIAN
endif
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
find_suid_SOURCES = src/linux/find_suid.cpp
faulty_SOURCES = src/faulty.c
faulty_CFLAGS = -ansi -fno-stack-protector
if WITH_SYSCALLS
syscalls_SOURCES = src/linux/syscalls.cpp
syscalls_CPPFLAGS = -ansi -Wno-unused-result -Wno-nonnull
endif
ansvifdir = bin
AM_CXXFLAGS = -Wall
check: all
	./ansvif -t examples/space.txt -c ./faulty -b 32 -o checkfaulty && rm checkfaulty.*
	@echo "*** CHECK 1 PASSED ***"
	./ansvif -t examples/space.txt -c ./faulty -b 32
	@echo "*** CHECK 2 PASSED ***"
	@echo "*** ALL TESTS PASSED ***"
