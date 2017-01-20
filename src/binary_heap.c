#include "binary_heap.h"
#include <stdlib.h>
#include <string.h>
static const init_heap_capacity = 8;
static void binary_heap_bubble_down(binary_heap_t * heap, int index);
static void binary_heap_bubble_up(binary_heap_t * heap, int index);
/*In a complete binary heap that obeys the shape invariant, all nodes are stored in an array.
* The left child of a node at index i is 2i + 1 and the right child is 2i + 2
* Thus the parent of a node at i is (i-1)/2*/
void binary_heap_new(binary_heap_t * heap, size_t key_size, int8_t (*compare)(void *, void *)) {
  heap->capacity = init_heap_capacity;
  heap->size = 0;
  heap->compare = compare;
  heap->key_size = key_size;
  heap->array = malloc(init_heap_capacity * sizeof(void *));
}

void binary_heap_free(binary_heap_t * heap) {
  int i;
  for(i = 0; i < heap->size; i++) {
    free(heap->array[i]);
  }
  free(heap->array);
}

void binary_heap_insert(binary_heap_t * heap, void * key) {
  void * ptr;
  if(heap->size == heap->capacity) {
    /*Double the capacity*/
    heap->capacity *= 2;
    heap->array = realloc(heap->array, heap->capacity * sizeof(void *));
  }
  ptr = malloc(heap->key_size);
  memcpy(ptr, key, heap->key_size);
  heap->array[heap->size] = ptr;
  binary_heap_bubble_up(heap, heap->size);
  heap->size++;
}
void binary_heap_extract(binary_heap_t * heap, void * ptr) {
  if(heap->size == 0) return;
  memcpy(ptr, heap->array[0], heap->key_size);
  free(heap->array[0]);
  heap->size--;
  if(heap->size == 0) return;
  heap->array[0] = heap->array[heap->size];
  binary_heap_bubble_down(heap, 0);
  if(heap->size <= heap->capacity/2 && heap->size > init_heap_capacity/2) {
    heap->capacity /= 2;
    heap->array = realloc(heap->array, heap->capacity * sizeof(void *));
  }
}

static void binary_heap_bubble_up(binary_heap_t * heap, int index) {
  void * ptr;
  while(index != 0 && heap->compare(heap->array[index], heap->array[(index - 1)/2]) < 0) {
    /*Swap the nodes*/
    ptr = heap->array[index];
    heap->array[index] = heap->array[(index - 1)/2];
    heap->array[(index - 1)/2] = ptr;
    index = (index - 1)/2;
  }
}

static void binary_heap_bubble_down(binary_heap_t * heap, int index) {
  int left, right;
  void * ptr;
  while(1) {
    left = 2*index + 1;
    right = left + 1;
    /*No more children to swap with*/
    if(left >= heap->size) return;
    if(right >= heap->size) {
      /*No Right child*/
      if(heap->compare(heap->array[index], heap->array[left]) > 0) {
        ptr = heap->array[index];
        heap->array[index] = heap->array[left];
        heap->array[left] = ptr;
      }
      return;
    }
    /*Has two children*/
    if(heap->compare(heap->array[left], heap->array[right]) < 0) {
      /*Swap the smaller one which is the left if necessary*/
      if(heap->compare(heap->array[index], heap->array[left]) < 0) return;
      ptr = heap->array[index];
      heap->array[index] = heap->array[left];
      heap->array[left] = ptr;
      index = left;
    } else {
      if(heap->compare(heap->array[index], heap->array[right]) < 0) return;
      ptr = heap->array[index];
      heap->array[index] = heap->array[right];
      heap->array[right] = ptr;
      index = right;
    }
  }
}
