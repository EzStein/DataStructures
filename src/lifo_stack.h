#ifndef LIFO_STACK_H
#define LIFO_STACK_H
#include <stdlib.h>
#include "array_list.h"
typedef struct lifo_stack lifo_stack_t;
struct lifo_stack {
  array_list_t * array;
};

void lifo_stack_new(lifo_stack_t *, size_t);
void lifo_stack_free(lifo_stack_t *);
void lifo_stack_push(lifo_stack_t *, void *);
void lifo_stack_pop(lifo_stack_t *, void *);
size_t lifo_stack_size(lifo_stack_t *);
int lifo_stack_is_empty(lifo_stack_t *);
#endif
