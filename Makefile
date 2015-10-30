CXX=gcc
CC=gcc
CFLAGS=-lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3
LDFLAGS=-L./include/gzstream
INCLUDES=
PATH=/usr/local/bin:/usr/bin:/bin:/usr/games
all: ansvif

default: ansvif

ansvif:
	cd ./include/gzstream;make;cd ../../
	PATH=/usr/local/bin:/usr/bin:/bin:/usr/games gcc ./src/ansvif.cpp -lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3 -L./include/gzstream  -o ./bin/ansvif
	PATH=/usr/local/bin:/usr/bin:/bin:/usr/games gcc ./src/find_suid.cpp -lstdc++ -lgzstream -lz -std=gnu++11 -lpthread -O3 -L./include/gzstream  -O2 -o ./bin/find_suid

test:
	gcc ./src/faulty.c -o ./bin/faulty
	./bin/ansvif -t examples/template -c ./bin/faulty -b 64
	rm ./bin/faulty
	# Test success

faulty : ./src/faulty.c
	gcc -o ./bin/faulty ./src/faulty.c

clean:
	rm Makefile ./bin/ansvif ./bin/find_suid ./include/gzstream/*.o ./include/gzstream/*.a
