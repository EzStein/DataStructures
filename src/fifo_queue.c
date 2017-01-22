#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

void queue_new(queue_t * queue, size_t entry_size) {
  queue->array = malloc(array_list_t);
  array_list_new(queue->array, entry_size);
}

void queue_free(queue_t * queue) {
  array_list_free(queue->array);
  free(queue->array);
}

void queue_enqueue(queue_t * queue, void * key) {
  array_list_add(queue->array, key);
}
/*Removes the item from the queue and stores it in the passed in pointer.*/
void queue_dequeue(queue_t * queue, void * ptr) {
  array_list_remove(queue->array, 0, ptr);
}

int queue_is_empty(queue_t * queue) {
  return array_list_is_empty(queue);
}
