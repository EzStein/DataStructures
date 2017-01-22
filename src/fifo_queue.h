#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H
#include <stddef.h>
#include <stdlib.h>

typedef struct fifo_queue fifo_queue_t;
struct fifo_queue {
  array_list_t * array;
};
void fifo_queue_new(fifo_queue_t *, size_t);
void fifo_queue_free(fifo_queue_t *);
void fifo_queue_enqueue(fifo_queue_t *, void *);
int fifo_queue_is_empty(fifo_queue_t *);
/*Removes the item from the queue and stores it in the passed in pointer.*/
void fifo_queue_dequeue(fifo_queue_t *, void *);
#endif
