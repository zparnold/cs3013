#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <strings.h>

int main(int argc, char *argv[]) {
	printf("Trying to malloc 4 bytes.\n");
	void *byte4 = malloc(4);
	printf("Value of byte4: %p\n\n", byte4);

	printf("Trying to malloc 64 more bytes.\n");
	void *byte64 = malloc(64);
	printf("Value of byte64: %p\n\n", byte64);

	printf("Trying to malloc 128 more bytes.\n");
	void *byte128 = malloc(128);
	printf("Value of byte128: %p\n\n", byte128);

	printf("Trying to malloc 1024 more bytes.\n");
	void *byte1024 = malloc(1024);
	printf("Value of byte1024: %p\n\n", byte1024);

	printf("Freeing byte4 then allocating 32 bytes\n");
	printf("The address of a should be the same as the address of byte4\n");
	free(byte4);
	void *byte32 = malloc(32);
	printf("Value of byte32: %p\n\n", byte32);

	printf("Trying to calloc a 16 element array of 8 bytes\n");
	void *array16 = calloc(16, 8);
	printf("Value of array16: %p\n\n", array16);

	printf("Trying to realloc byte64 to 5000 bytes\n");
	((int *) byte64)[0] = 314159;
	printf("Value of byte64 before the realloc: %d\n", ((int *) byte64)[0]);
	printf("Location of byte64 before the realloc: %p\n", byte64);
	byte64 = realloc(byte64, 5000);
	printf("Value of byte64 after the realloc: %d\n", ((int *) byte64)[0]);
	printf("Location of byte64 after the realloc: %p\n\n", byte64);

	printf("Trying to allocate 64 bytes\n");
	printf("This should have the same address as byte64\n");
	void *c = malloc(64);
	printf("Value of c: %p\n", c);
	exit(0);
}
