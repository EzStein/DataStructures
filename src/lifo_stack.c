#include "lifo_stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void lifo_stack_new(lifo_stack_t * stack, size_t entry_size, void (*free_function)(void *)) {
  stack->entry_size = entry_size;
  stack->free_function = free_function;
  stack->capacity = 8;
  stack->size = 0;
  stack->head = 0;
  stack->array = malloc(8 * sizeof(void *));
}

void lifo_stack_free(lifo_stack_t * stack) {
  int i;
  for(i = 0; i < stack->size; i++) {
    if(stack->free_function)
      stack->free_function(stack->array[i]);
    free(stack->array[i]);
  }
  free(stack->array);
}

void lifo_stack_push(lifo_stack_t * stack, void * data) {
  void * ptr;
  if(stack->size == stack->capacity) {
    stack->array = realloc(stack->array, 2 * stack->capacity * sizeof(void *));
    stack->capacity *= 2;
  }
  ptr = malloc(stack->entry_size);
  memcpy(ptr, data, stack->entry_size);
  stack->array[stack->head] = ptr;
  stack->head++;
  stack->size++;
}

void lifo_stack_pop(lifo_stack_t * stack, void * ptr) {
  if(stack->size == 0) return;
  stack->head--;
  stack->size--;
  memcpy(ptr, stack->array[stack->head], stack->entry_size);
  if(stack->free_function)
    stack->free_function(stack->array[stack->head]);
  free(stack->array[stack->head]);
}

size_t lifo_stack_size(lifo_stack_t * stack) {
    return stack->size;
}

uint8_t lifo_stack_is_empty(lifo_stack_t * stack) {
  return stack->size == 0;
}
