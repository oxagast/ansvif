AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
if WITH_GTK
bin_PROGRAMS = ansvif ansvif_gtk generic_buffer_overflow string_format null_pointer
endif
if WITH_SYSCALLS
bin_PROGRAMS = ansvif syscalls
endif
if WITH_TESTCODE
bin_PROGRAMS = ansvif generic_buffer_overflow string_format null_pointer
else
bin_PROGRAMS = ansvif
endif

noinst_LIBRARIES = libgzstream.a
libgzstream_a_SOURCES = include/gzstream/gzstream.h include/gzstream/gzstream.cpp
if OS_IS_KALI
SUBDIRS= include/gzstream/
endif
ansvif_SOURCES = src/log.cpp src/bin2hex.cpp src/popen2.cpp src/main.cpp src/help.cpp src/match_fault.cpp src/sys_string.cpp src/man_read.cpp src/trash.cpp src/randomizer.cpp src/remove_chars.cpp src/to_int.cpp src/file_check.cpp src/reaper.cpp src/write_file.cpp src/templates.cpp include/gzstream/gzstream.h include/xmlwriter/xml_writer.hpp src/version.h
if OS_IS_REDHAT
ansvif_LDADD = -lgzstream -lstdc++ -lpthread -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
ansvif_CPPFLAGS = -O3 -D __REDHAT__ -D __NOTANDROID__ -Wall -std=c++11 -ftemplate-depth=1024
endif
if OS_IS_DEBIAN
ansvif_LDADD = -lgzstream -lstdc++ -lpthread -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
ansvif_CPPFLAGS = -O3 -D __DEBIAN__ -D __NOTANDROID__ -Wall -std=c++11 -ftemplate-depth=1024
endif
if OS_IS_FREEBSD
ansvif_LDADD = -lstdc++ -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux -pthread
ansvif_CPPFLAGS = -O3 -D __DEBIAN__ -D __FREEBSD__ -D NOTANDROID -Wl,--no-as-needed -std=c++11 -ftemplate-depth=1024
endif
if OS_IS_KALI
ansvif_LDADD = -lgzstream -lstdc++ -lpthread -lz -L.
ansvif_LDFLAGS = -I./include/gzstream -I./include/xml_writer -I./src/linux
ansvif_CPPFLAGS = -O3 -D __KALI__ -D __DEBIAN__ -D __NOTANDROID__ -Wall -std=c++11 -ftemplate-depth=1024
endif
if WITH_TESTCODE
generic_buffer_overflow_SOURCES = src/generic_buffer_overflow.c
generic_buffer_overflow_CFLAGS = -ansi -fno-stack-protector -D_GNU_SOURCE -Wno-implicit-function-declaration
string_format_SOURCES = src/string_format.c
string_format_CFLAGS = -ansi -D_GNU_SOURCE -Wno-format-security
null_pointer_SOURCES = src/null_pointer.c
endif
if WITH_SYSCALLS
syscalls_SOURCES = src/linux/syscalls.cpp
syscalls_CPPFLAGS = -ansi -Wno-unused-result -Wno-nonnull -std=c++11
endif
if WITH_GTK
ansvif_gtk_SOURCES = src/gtk.cpp src/popen2.cpp
ansvif_gtk_LDADD = @GTK_LIBS@
ansvif_gtk_CPPFLAGS = @GTK_CFLAGS@ -fpermissive
endif
ansvifdir = bin
check: all
	./ansvif -t examples/specific/space.txt -c ./generic_buffer_overflow -b 64 -o checkfaulty && rm checkfaulty.*
	@echo "*** CHECK 1 PASSED ***"
	./ansvif -t examples/specific/space.txt -c ./generic_buffer_overflow -b 64
	@echo "*** CHECK 2 PASSED ***"
	./ansvif -t examples/specific/space.txt -c ./string_format -b 32
	@echo "*** CHECK 3 PASSED ***"
	@echo "*** ALL TESTS PASSED ***"
