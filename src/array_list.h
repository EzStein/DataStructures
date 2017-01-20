#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdlib.h>
/*This array will work a bit differently than other generic types used here. Instead
of storing void pointers in a resizable array, this implementation will store the data
directly, potentially saving lots of memory by avoiding references. This is not possible
in structures that involve nodes represented as structs. But it is possible for this array list
and the simple binary heap queue and stack which use a resizable array*/
/*Note that no error handling is done in this structure. Input values to a function
are expected to be valid and conform to the documentation of that method. Failure to
do so will result in UNDEFINED behaviour. For all functions the passed in array_list_t pointer Should
be properly allocated with array_list_new() and properly freed with array_list_free()*/
typedef struct array_list array_list_t;
struct array_list {
  int size;
  size_t data_size;
  int capacity;
  void * array;
};
/*Returns non-zero value if the list is empty and zero otherwise*/
int array_list_is_empty(array_list_t *);
/*Constructs a new resizable with some initial capcity and the given element size*/
void array_list_new(array_list_t *, size_t);
/*Appends the element to the end of the array. Copies the data pointed to by the given pointer into internal memory.*/
void array_list_add(array_list_t *, void *);
/*Inserts the given element at the specified index. All elements aboce that index
will be pushed up. The index may be anywhere from [0...size] (inclusive)*/
void array_list_insert(array_list_t *, int, void *);
/*retrieves the element at the given index and copies that data into the area of memory pointed
* to by the given pointer. The given pointer must point to valid memory of enough size.
* The index can be anywhere between [0...size)*/
void array_list_get(array_list_t *, int, void *);
/*Removes the element at the specified index. The removed element is stored in the given pointer if it
is not null. If it is not a null pointer, it must point to a valid memory address of some the required size.*/
void array_list_remove(array_list_t *, int, void *);
/*Returns the size of the array*/
int array_list_size(array_list_t *);
/*Sets the element at the given index to the given element. Index must be in the range of
* [0...size)*/
void array_list_set(array_list_t *, int, void *);
/*Free's the array. Should only be called once for every call to array_list_new()*/
void array_list_free(array_list_t *);
/*Swaps the elements at the given indices. Both indices must be in the range [0...size).
They may be the same index*/
void array_list_swap(array_list_t *, int, int);

/*Deletes the contents of the array and empties it*/
void array_list_clear(array_list_t *);
#endif
