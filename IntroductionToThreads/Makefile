CC=g++
CFLAGS=-c -Wall -O0 -pthread
LDFLAGS=-pthread

SOURCES1=00_StartJoin/example1.cpp
OBJECTS1=$(SOURCES1:.cpp=.o)
EXECUTABLE1=./start_join.out

SOURCES2=05_FalseSharing/SimpleMultithreadedSorter.cpp
OBJECTS2=$(SOURCES2:.cpp=.o)
EXECUTABLE2=./false_sharing.out

all: start_join false_sharing

start_join: $(OBJECTS1)
	$(CC) $(LDFLAGS) $(OBJECTS1) -o $(EXECUTABLE1)

false_sharing: $(OBJECTS2)
	$(CC) $(LDFLAGS) $(OBJECTS2) -o $(EXECUTABLE2)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3) $(OBJECTS1) $(OBJECTS2)
