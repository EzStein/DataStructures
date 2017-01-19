#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

static const int init_ring_capacity = 8;
void queue_new(queue_t * queue, size_t entry_size, void (*free_function)(void *)) {
  queue->size = 0;
  queue->entry_size = entry_size;
  queue->free_function = free_function;
  queue->capacity = init_ring_capacity;
  queue->ring = malloc(init_ring_capacity * sizeof(void *));
  queue->head = 0;
  queue->tail = 0;
}

void queue_free(queue_t * queue) {
  int i;
  void * ptr;
  for(i = queue->tail; i < queue->tail + queue->size; i++) {
    ptr = queue->ring[i % queue->capacity];
    if(queue->free_function)
      queue->free_function(ptr);
    free(ptr);
  }
  free(queue->ring);
}

void queue_enqueue(queue_t * queue, void * key) {
  void * ptr;
  void ** new_ring;
  int i;
  if(queue->size == queue->capacity) {
    new_ring = malloc(2 * queue->capacity * sizeof(void *));
    for(i = queue->tail; i < queue->tail + queue->size; i++) {
      new_ring[i - queue->tail] = queue->ring[i % queue->capacity];
    }

    free(queue->ring);
    queue->ring = new_ring;
    queue->capacity *= 2;

    queue->tail = 0;
    queue->head = queue->size;

  }
  ptr = malloc(queue->entry_size);
  memcpy(ptr, key, queue->entry_size);

  queue->ring[queue->head] = ptr;

  queue->size++;
  queue->head = (queue->head + 1) % queue->capacity;
}
/*Removes the item from the queue and stores it in the passed in pointer.*/
void queue_dequeue(queue_t * queue, void * ptr) {
  void * data;
  if(queue->size == 0) return;
  data = queue->ring[queue->tail];
  queue->tail = (queue->tail + 1) % queue->capacity;
  queue->size--;
  memcpy(ptr, data, queue->entry_size);
  if(queue->free_function)
    queue->free_function(data);
  free(data);
}
