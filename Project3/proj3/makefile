all: restroomRun restroom.o

restroomRun: restroomRun.o restroom.o restroom.h
	gcc restroomRun.o restroom.o -o restroomRun -Wall -pthread -lm
restroomRun.o: restroomRun.c restroom.h
	gcc -g -c restroomRun.c -Wall -pthread
restroom.o: restroom.c restroom.h
	gcc -g -c restroom.c -Wall -pthread
clean:
	rm -f *.o restroomRun

