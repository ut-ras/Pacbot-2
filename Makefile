TARGET = test
LIBS  = -lpigpio -lrt
CFLAGS = -Wall -g #-O3

.PHONY: default all clean

default: $(TARGET)
all: default

SRC = $(wildcard src/*.cpp)
INC = $(wildcard inc/*.h)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))

.PRECIOUS: obj/%.o

obj/%.o: src/%.cpp $(INC)
	g++ -c $(CFLAGS) -o $@ $<

$(TARGET): $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	-rm -f obj/*.o
	-rm -f $(TARGET)
