#include "lifo_stack.h"
#include "array_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void lifo_stack_new(lifo_stack_t * stack, size_t entry_size) {
  stack->array = malloc(sizeof(array_list_t));
  array_list_new(stack->array, entry_size);
}

void lifo_stack_free(lifo_stack_t * stack) {
  array_list_free(stack->array);
  free(stack->array);
}

void lifo_stack_push(lifo_stack_t * stack, void * data) {
  array_list_add(stack->array, data);
}

void lifo_stack_pop(lifo_stack_t * stack, void * ptr) {
  array_list_remove(stack->array, stack->array->size - 1, ptr);
}

size_t lifo_stack_size(lifo_stack_t * stack) {
    return stack->array->size;
}

int lifo_stack_is_empty(lifo_stack_t * stack) {
  return array_list_is_empty(stack->array);
}
