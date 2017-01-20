#include "array_list.h"
#include <stdlib.h>
#include <string.h>
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
  if(array->size == array->capacity) {
    array_list_double_capacity(array);
  }
  /*Points to the start of the first empty slot in memory*/
  ptr = ((char *)array->array) + array->size * array->data_size;
  memcpy(ptr, data, array->data_size);
  array->size++;
}

void array_list_add_at(array_list_t * array, int index, void * data) {
  void * ptr;
  size_t i;
  if(index == array->size) {
    array_list_add(array, data);
    return;
  }
  if(array->size == array->capacity) {
    array_list_double_capacity(array);
  }
  for(i = (array->size + 1) * array->data_size - 1; i >= (index + 1) * array->data_size; i--) {
    *(((char *)array->array) + i) = *(((char *)array->array) + i - array->data_size);
  }
  /*Points to the start of the first empty slot in memory*/
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ptr, data, array->data_size);
  array->size++;
}

void array_list_get(array_list_t * array, int index, void * ret) {
  void * ptr;
  if(index >= array->size) return;
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ret, ptr, array->data_size);
}

void array_list_remove(array_list_t * array, int index, void * ret) {
  void * ptr;
  size_t i;
  if(index >= array->size) return;
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ret, ptr, array->data_size);
  for(i = index * array->data_size; i < (array->size - 1) * array->data_size; i++) {
    *(((char *)array->array) + i) = *(((char *)array->array) + i + array->data_size);
  }
  array->size--;
  if(array->capacity/2 >= array->size && array->capacity/2 >= array_list_init_capacity) {
    array_list_half_capacity(array);
  }
}

int array_list_size(array_list_t * array) {
  return array->size;
}
uint8_t array_list_is_empty(array_list_t * array) {
  return array->size == 0;
}
void array_list_set(array_list_t * array, int index, void * data) {
  void * ptr;
  if(index >= array->size) return;
  ptr = ((char *)array->array) + index * array->data_size;
  memcpy(ptr, data, array->data_size);
}

void array_list_free(array_list_t * array) {
  free(array->array);
}

static void array_list_double_capacity(array_list_t * array) {
  array->capacity *= 2;
  array->array = realloc(array->array, array->capacity * array->data_size);
}
static void array_list_half_capacity(array_list_t * array) {
  array->capacity /= 2;
  array->array = realloc(array->array, array->capacity * array->data_size);
}
