#include "array_list.h"
#include <stdlib.h>
#include <string.h>
/*The array will automatically double and half its capacity when necessary. The capacity
will never fall below the initial capacity*/
static const int array_list_init_capacity = 8;
static void array_list_double_capacity(array_list_t *);
static void array_list_half_capacity(array_list_t *);
void array_list_new(array_list_t * array, size_t data_size) {
  array->size = 0;
  array->capacity = array_list_init_capacity;
  array->data_size = data_size;
  array->array = malloc(array->capacity * data_size);
}

void array_list_add(array_list_t * array, void * data) {
  void * ptr;
  /*Double the size of the array if necessary*/
  if(array->size == array->capacity) {
    array_list_double_capacity(array);
  }
  /*Points to the start of the first empty slot in memory*/
  ptr = ((char *)array->array) + array->size * array->data_size;
  /*Copies the data into the appropriate spot*/
  memcpy(ptr, data, array->data_size);
  array->size++;
}

void array_list_insert(array_list_t * array, int index, void * data) {
  char * ptr;
  size_t i;
  size_t last_byte_to_copy;

  /*Equivlent to appending to the array*/
  if(index == array->size) {
    array_list_add(array, data);
    return;
  }

  /*Double capacity if necessary*/
  if(array->size == array->capacity) {
    array_list_double_capacity(array);
  }

  /*Points to the start of the array*/
  ptr = (char *)array->array;
  /*This is the first byte of index + 1*/
  last_byte_to_copy = (index + 1) * array->data_size;
  /*Shifts elements from up by one index. Starting from the last byte of the first
  unallocated element in the array, we work backwords and copy byte by byte the element infront.*/
  for(i = (array->size + 1) * array->data_size - 1; i >= last_byte_to_copy; i--) {
    *(ptr + i) = *(ptr + i - array->data_size);
  }
  /*Points to the start of the first empty slot in memory*/
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ptr, data, array->data_size);
  array->size++;
}

void array_list_get(array_list_t * array, int index, void * ret) {
  void * ptr;
  /*points to the first byte of element at given index*/
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ret, ptr, array->data_size);
}

void array_list_remove(array_list_t * array, int index, void * ret) {
  char * ptr;
  size_t i;
  size_t last_byte_to_copy;
  /*Points to the start of the array*/
  ptr = (char *)array->array;

  /*Copies the value to remove into the return value*/
  ptr = ptr + index * array->data_size;
  memcpy(ret, ptr, array->data_size);

  /*Starting from the first byte of the removed element, copy byte by byte from the next element to the previous*/
  last_byte_to_copy = (array->size - 1) * array->data_size;
  for(i = index * array->data_size; i < last_byte_to_copy; i++) {
    *(ptr + i) = *(ptr + i + array->data_size);
  }
  array->size--;

  /*Half capacity if necessary. NOTE That to increase performance it will only half the capacity
  if add several elements immediatly would not cause an immediate resize.*/
  if(array->capacity/2 >= (array->size + array_list_init_capacity) && array->capacity/2 >= array_list_init_capacity) {
    array_list_half_capacity(array);
  }
}

int array_list_size(array_list_t * array) {
  return array->size;
}

int array_list_is_empty(array_list_t * array) {
  return array->size == 0;
}

void array_list_set(array_list_t * array, int index, void * data) {
  void * ptr;
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ptr, data, array->data_size);
}

void array_list_free(array_list_t * array) {
  free(array->array);
}

void array_list_clear(array_list_t * array) {
  array->size = 0;
  array->capacity = array_list_init_capacity;
  array->array = realloc(array->array, array_list_init_capacity * array->data_size);
}

void array_list_swap(array_list_t * array, int index1, int index2) {
  char * index1_ptr, * index2_ptr;
  char tmp_buf;
  size_t i;
  /*Points to the first byte of element at index1 and index2 respectively*/
  index1_ptr = ((char *)array->array) + index1 * array->data_size;
  index2_ptr = ((char *)array->array) + index2 * array->data_size;
  for(i = 0; i < array->data_size; i++) {
    /*Store a byte of index1 in the buffer and swap the bytes*/
    tmp_buf = *(index1_ptr + i);
    *(index1_ptr + i) = *(index2_ptr + i);
    *(index2_ptr + i) = tmp_buf;
  }
}

static void array_list_double_capacity(array_list_t * array) {
  array->capacity *= 2;
  array->array = realloc(array->array, array->capacity * array->data_size);
}

static void array_list_half_capacity(array_list_t * array) {
  array->capacity /= 2;
  array->array = realloc(array->array, array->capacity * array->data_size);
}
