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
ansvif_SOURCES = src/common.cpp src/bin2hex.cpp src/popen2.cpp src/main.cpp src/help.cpp src/match_seg.cpp src/sys_string.cpp src/man_read.cpp src/trash.cpp src/randomizer.cpp include/gzstream/gzstream.h include/xmlwriter/xml_writer.hpp src/main.h
if OS_IS_REDHAT
ansvif_LDADD = -lgzstream -lcryptopp -lstdc++ -lpthread -lz -L.
ansvif_CPPFLAGS = -O3 -D __REDHAT -Wall -std=c++11
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
endif
if OS_IS_DEBIAN
ansvif_LDADD = -lgzstream -lcrypto++ -lstdc++ -lpthread -lz -L.
ansvif_CPPFLAGS = -O3 -D __DEBIAN -Wall -std=c++11
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
endif
if OS_IS_FREEBSD
ansvif_LDADD = -lstdc++ -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux -pthread
ansvif_CPPFLAGS = -O3 -D __DEBIAN -D __FREEBSD -Wl,--no-as-needed -std=c++11
else
ansvif_LDADD = -lestdc++ -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux -pthread
ansvif_CPPFLAGS = -O3 -D __DEBIAN -Wl,--no-as-needed -std=c++11
endif
find_suid_SOURCES = src/linux/find_suid.cpp
faulty_SOURCES = src/faulty.c
faulty_CFLAGS = -ansi -fno-stack-protector -D_GNU_SOURCE -Wno-implicit-function-declaration
if WITH_SYSCALLS
syscalls_SOURCES = src/linux/syscalls.cpp
syscalls_CPPFLAGS = -ansi -Wno-unused-result -Wno-nonnull -std=c++11
endif
ansvifdir = bin
check: all
	./ansvif -t examples/space.txt -c ./faulty -b 32 -o checkfaulty && rm checkfaulty.*
	@echo "*** CHECK 1 PASSED ***"
	./ansvif -t examples/space.txt -c ./faulty -b 32
	@echo "*** CHECK 2 PASSED ***"
	@echo "*** ALL TESTS PASSED ***"
