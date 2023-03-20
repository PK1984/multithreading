# Get the list of all C++ files in the current directory
SRCS := $(wildcard *.cpp)

# Get the list of all program names (without the .cpp extension)
PROGS := $(patsubst %.cpp,%,$(SRCS))

# Define the default target, which builds all programs
all: $(PROGS)

# Define a pattern rule to build a program from its source file
%: %.cpp
	g++ -o $@ $<

# Define a target for cleaning up the build artifacts
clean:
	rm -f $(PROGS)

# Define a target for building a single program
# Usage: make <program_name>
.PHONY: $(PROGS)
$(PROGS):
	g++ -o $@  -O2 --std=c++11 $@.cpp  -lssl -lcrypto
