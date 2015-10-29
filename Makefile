CXX=gcc
CC=gcc
CFLAGS=-lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3
LDFLAGS=-L./gzstream
INCLUDES=
PATH=/usr/local/bin:/usr/bin:/bin:/usr/games
all: ansvif

default: ansvif

ansvif:
	cd ./gzstream;make;cd ../
	PATH=/usr/local/bin:/usr/bin:/bin:/usr/games gcc ansvif.cpp -lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3 -L./gzstream  -o ansvif
	PATH=/usr/local/bin:/usr/bin:/bin:/usr/games gcc find_suid.cpp -lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3 -L./gzstream  -O2 -o find_suid

test:
	gcc faulty.c -o faulty
	./ansvif -t examples/template -c ./faulty -b 2048
	rm faulty
	# Test success

faulty : faulty.c
	gcc -o faulty faulty.c

clean:
	rm Makefile ansvif find_suid ./gzstream/*.o ./gzstream/*.a
