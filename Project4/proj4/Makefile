all: test_malloc malloc.o

test: test.c malloc.o
	gcc -g -o test test.c malloc.o

malloc.o: malloc.c malloc.h
	gcc -g -c malloc.c -o malloc.o -Wall

clean:
	rm -rf test_malloc malloc.o *.o