all: getprinfo

clean:
	rm -f *.o getprinfo

getprinfo: getprinfo.o
	gcc getprinfo.o -o getprinfo
	
getprinfo.o: getprinfo.c
	gcc getprinfo.c -c getprinfo.o

