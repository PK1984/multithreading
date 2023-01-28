CC=g++
CFLAGS=-c -Wall -O0 --std=c++20 -I
LDFLAGS=-pthread

SRC := $(wildcard */*.cpp)
EXE := $(patsubst %.cpp,%,$(SRC))

all: $(EXE)

%.out: %.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(EXE) */*.o
