LIBS  = -lpigpio -lrt
CFLAGS = -Wall -g #-O3

SRC=$(wildcard src/*.cpp)

test: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


