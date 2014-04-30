#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <strings.h>
#include <string.h>

#define SBRK_SCALE 100
#define CHECKSUM ~0
#define MALLOC_OVERHEAD 20
#define USED_OVERHEAD 8
#define LINKED_LIST_OVERHEAD 12

#define FREE_SIZE 0
#define NEXT_PTR 1
#define PREV_PTR 2
#define USED_SIZE 3
#define CHECKSUM_LOC 4
#define USER_PTR 5

#define TRUE 1
#define FALSE 0

#define MIN(a,b) a < b ? a : b

void *malloc_head = NULL;

/*
4 bytes each:
1. Free Space Size
2. Next Pointer
3. Prev Pointer
4. Used Space Size
5. Checksum
*/

void *malloc(size_t size) {
	void *start = malloc_head;
	int malloc_size = size * SBRK_SCALE;
	int searching = TRUE;

	if (malloc_head == NULL) {
		malloc_head = sbrk(malloc_size);

		if ((int) malloc_head == -1) {
			return NULL;
		} else {
			start = malloc_head;
			((int *) malloc_head)[0] = malloc_size - LINKED_LIST_OVERHEAD; // Set the free size
		}
	}

	while (searching) {
		if (((int *) start)[FREE_SIZE] >= size + MALLOC_OVERHEAD) {
			searching = FALSE;
		} else if ((void *) ((int*) start)[NEXT_PTR] == NULL) {
			// Now check to see if we need to sbrk again and that we are at the last linked list node
			void *next_loc = sbrk(malloc_size);

			if ((int) next_loc == -1) {
				return NULL;
			} else {
				// Since sbrk may not give us continuous memory,
				// so set up a linked list header at the new memory location
				((int *) start)[NEXT_PTR] = (int) next_loc;
				((int *) next_loc)[FREE_SIZE] = malloc_size - LINKED_LIST_OVERHEAD;
				((int *) next_loc)[NEXT_PTR] = (int) NULL;
				((int *) next_loc)[PREV_PTR] = (int) start;
			}
		} else {
			// We haven't hit the last node, so just go to the next node
			start = (void *) ((int *) start)[NEXT_PTR];
		}
	}

	// Set up the headers for the next free spot
	if (((int *) start)[NEXT_PTR] == (int) NULL) {
		void *next_header = start + size + MALLOC_OVERHEAD;
		((int *) next_header)[FREE_SIZE] = ((int *) start)[FREE_SIZE] - (USED_OVERHEAD + size);
		((int *) next_header)[NEXT_PTR] = (int) NULL;
		((int *) next_header)[PREV_PTR] = (int) start;
	}

	// Update the headers for this allocation
	((int *) start)[FREE_SIZE] = 0;
	((int *) start)[USED_SIZE] = size;
	((int *) start)[CHECKSUM_LOC] = CHECKSUM;

	return &((int *) start)[USER_PTR];
}

void free(void *ptr) {
	if (ptr != NULL) {
		// Go back 20 bytes to the start of the linked list headers
		void *node = ptr - MALLOC_OVERHEAD;
		((int *) node)[FREE_SIZE] = ((int *) node)[USED_SIZE] + USED_OVERHEAD;
	}
}

void *calloc(size_t nmem, size_t size) {
	if (nmem == 0) {
		return NULL;
	} else {
		void *user_ptr = malloc(nmem * size);
		bzero(user_ptr, nmem * size);
		return user_ptr;
	}
}

void *realloc(void *ptr, size_t size) {
	if (ptr == NULL && size == 0) {
		return NULL;
	} else if (ptr == NULL && size > 0) {
		return malloc(size);
	} else if (ptr != NULL && size == 0) {
		free(ptr);
		return NULL;
	} else {
		void *new_ptr = malloc(size);
		void *node = ptr - MALLOC_OVERHEAD;
		memcpy(new_ptr, ptr, ((int *) node)[USED_SIZE]);
		free(ptr);
		return new_ptr;
	}
}
