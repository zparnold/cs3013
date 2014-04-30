all: doit shell shell2

doit: doit.o
	gcc doit.o -o doit

doit.o: doit.c
	gcc -g -c doit.c

shell: doit2.o
	gcc doit2.o -o shell

doit2.o: doit2.c
	gcc -g -c doit2.c

shell2: doit3.o
	gcc doit3.o -o shell2

doit3.o: doit3.c
	gcc -g -c doit3.c

clean:
	rm -f *o doit shell shell2
