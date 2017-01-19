#ifndef QUEUE_H
#define QUEUE_H
#include <stddef.h>
typedef struct queue queue_t;
struct queue {
  int size;
  size_t entry_size;
  int capacity;
  void (*free_function)(void *);
  void ** ring;

  /*Where items are added to the queue with enqueue*/
  int head;

  /*Where items are removed from the queue with dequeue*/
  int tail;
};
void queue_new(queue_t *, size_t entry_size, void (*free_function)(void *));
void queue_free(queue_t *);
void queue_enqueue(queue_t *, void *);
/*Removes the item from the queue and stores it in the passed in pointer.*/
void queue_dequeue(queue_t *, void *);
#endif
