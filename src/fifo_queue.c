#include "fifo_queue.h"
#include "array_list.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

void fifo_queue_new(fifo_queue_t * queue, size_t entry_size) {
  queue->array = malloc(sizeof(array_list_t));
  array_list_new(queue->array, entry_size);
}

void fifo_queue_free(fifo_queue_t * queue) {
  array_list_free(queue->array);
  free(queue->array);
}

void fifo_queue_enqueue(fifo_queue_t * queue, void * key) {
  array_list_add(queue->array, key);
}
/*Removes the item from the queue and stores it in the passed in pointer.*/
void fifo_queue_dequeue(fifo_queue_t * queue, void * ptr) {
  array_list_remove(queue->array, 0, ptr);
}

int fifo_queue_is_empty(fifo_queue_t * queue) {
  return array_list_is_empty(queue->array);
}
