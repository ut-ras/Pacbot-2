LIBS  = -lpigpio -lrt -lbcm2835
CFLAGS = -Wall -g #-O3

SRC=$(wildcard *.cpp)

test: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)


