LIBS  = -lpigpio -lrt
CFLAGS = -Wall -g #-O3

SRC=$(wildcard *.c)

test: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)


