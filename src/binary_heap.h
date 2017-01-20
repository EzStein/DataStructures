#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H
#include <stdlib.h>
#include "array_list.h"
/*A binary heap implemented using an array_list*/
typedef struct binary_heap binary_heap_t;
struct binary_heap {
  array_list_t * array;
  int (*compare)(void *, void *);
};
/*NOTE: This is a min-heap but a max heap can be simulated by changing the provided compare method*/
/*Constructs a new empty heap with the given key_size and compare function.*/
void binary_heap_new(binary_heap_t *, size_t key_size, int (*compare)(void *, void *));
/*Frees the heap and all its memory. Should be called exactly once for every call to new*/
void binary_heap_free(binary_heap_t *);
/*Inserts an element into the heap, copying the memory into internal memory.*/
void binary_heap_insert(binary_heap_t *, void *);
/*Removes the element and copies it into the provided pointer. The pointer
must point to valid memory of the proper size.*/
void binary_heap_extract(binary_heap_t *, void *);
#endif
