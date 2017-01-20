#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdlib.h>
/*This array will work a bit differently than other generic types used here. Instead
of storing void pointers in a resizable array, this implementation will store the data
directly, potentially saving lots of memory by avoiding references. This is not possible
in structures that involve nodes represented as structs. But it is possible for this array list
and the simple binary heap and stack which use a resizable array*/
typedef struct array_list array_list_t;
struct array_list {
  int size;
  size_t data_size;
  int capacity;
  void * array;
};
uint8_t array_list_is_empty(array_list_t *);
void array_list_new(array_list_t *, size_t);
void array_list_add(array_list_t *, void *);
void array_list_add_at(array_list_t *, int, void *);
void array_list_get(array_list_t *, int, void *);
void array_list_remove(array_list_t *, int, void *);
int array_list_size(array_list_t *);
void array_list_set(array_list_t *, int, void *);
void array_list_free(array_list_t *);
#endif
