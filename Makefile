# the compiler to use
CC=gcc
# options to pass to compiler
CFLAGS=-lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3
LDFLAGS=-L./gzstream
all: peachfuzz

default: peachfuzz

peachfuzz:
	${CC} peachfuzz.cpp ${CFLAGS} ${LDFLAGS} -o peachfuzz
	${CC} find_suid.cpp -std=gnu++11 -lstdc++ -O2 -o find_suid

test:
	${CC} faulty.c -o faulty
	./peachfuzz -t template -c ./faulty -b 2048
	rm faulty
	# Test success

faulty : faulty.c
	${CC} -o faulty faulty.c

clean:
	rm peachfuzz find_suid
