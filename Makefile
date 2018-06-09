all: Filter.o PNG.o main.o function.o
	g++ -m32 -o Filter Filter.o PNG.o main.o function.o -lSDL2 -lGL -lGLEW -lpng -lz
Filter.o: Filter.cpp function.h Filter.hpp Pixel.hpp PNG.hpp Exception.hpp
	g++ -m32 -c -o Filter.o Filter.cpp
PNG.o: PNG.cpp PNG.hpp Pixel.hpp Exception.hpp
	g++ -m32 -c -o PNG.o PNG.cpp
main.o: main.cpp Filter.hpp Pixel.hpp PNG.hpp
	g++ -m32 -c -o main.o main.cpp
function.o: function.s function.h
	nasm -f elf32 function.s
clean:
	rm -f *.o Filter
.PHONY: all clean
