LIBS  = -lpigpio -lrt
CFLAGS = -Wall -g #-O3

SRC=$(wildcard *.cpp)

test: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


