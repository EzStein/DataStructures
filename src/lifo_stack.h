#ifndef LIFO_STACK_H
#define LIFO_STACK_H
#include <stdlib.h>
typedef struct lifo_stack lifo_stack_t;
struct lifo_stack {
  int size;
  size_t entry_size;
  int head;
  int capacity;
  void (*free_function)(void *);
  void ** array;
};

void lifo_stack_new(lifo_stack_t *, size_t, void (*free_function)(void *));
void lifo_stack_free(lifo_stack_t *);
void lifo_stack_push(lifo_stack_t *, void *);
void lifo_stack_pop(lifo_stack_t *, void *);
size_t lifo_stack_size(lifo_stack_t *);
uint8_t lifo_stack_is_empty(lifo_stack_t *);
#endif
