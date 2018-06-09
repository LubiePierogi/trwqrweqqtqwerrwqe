all: Filter.o main.o
	g++ -o Filter Filter.o main.o -lSDL2 -lGL -lGLEW
Filter.o:	Filter.cpp Filter.hpp Exception.hpp function.h
	g++ -c -o Filter.o Filter.cpp
main.o: main.cpp Filter.hpp
	g++ -c -o main.o main.cpp
clean:
	rm -f *.o Filter
.PHONY: all clean
