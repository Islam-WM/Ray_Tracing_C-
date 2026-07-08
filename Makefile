CXX       := /usr/bin/g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -fdiagnostics-color=always -g

SRC      := src
INCLUDE  := include

# OpenCV flags and libraries
OPENCV_CFLAGS := $(shell pkg-config --cflags opencv4)
OPENCV_LIBS   := $(shell pkg-config --libs opencv4)

all: clean main.exe

main.exe: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $(OPENCV_CFLAGS) -I$(INCLUDE) $^ -o $@ $(OPENCV_LIBS)

run: all
	./main.exe

clean:
	rm -f *.exe