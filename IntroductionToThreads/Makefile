CC=g++
CFLAGS=-Wall -O0 --std=c++20 -I
LDFLAGS=-pthread

SRC := $(wildcard */*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRC))
EXE := $(patsubst %.cpp,%,$(SRC))

all: $(OBJS) $(EXE)

%.out: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@rm -f $(EXE) $(OBJS)
