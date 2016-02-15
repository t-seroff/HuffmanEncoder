CXX = g++

CXXFLAGS = -x c++ -std=c++11 -Wall -Werror -Wextra -Wunused-parameter

BINARIES = prog2

all: $(BINARIES)

prog2: Main.o Encoder.o
	${CXX} $^ -o $@

clean:
	/bin/rm -f $(BINARIES) *.o

