# the compiler to use
CC=gcc
# options to pass to compiler
CFLAGS=-lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3
LDFLAGS=-L./gzstream
all: segfault_4me

default: segfault_4me

segfault_4me:
	${CC} segfault_4me.cpp ${CFLAGS} ${LDFLAGS} -o segfault_4me
	${CC} find_suid.cpp -std=gnu++11 -lstdc++ -O2 -o find_suid

test:
	${CC} faulty.c -o faulty
	./segfault_4me -t template -c ./faulty -b 2048
	rm faulty
	# Test success

faulty : faulty.c
	${CC} -o faulty faulty.c

clean:
	rm segfault_4me find_suid
