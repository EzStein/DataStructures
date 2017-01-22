#include "binary_heap.h"
#include "array_list.h"
#include <stdlib.h>
#include <string.h>
static void binary_heap_bubble_down(binary_heap_t * heap, int index);
static void binary_heap_bubble_up(binary_heap_t * heap, int index);
/*In a complete binary heap that obeys the shape invariant, all nodes are stored in an array.
* The left child of a node at index i is 2i + 1 and the right child is 2i + 2
* Thus the parent of a node at i is (i-1)/2*/
void binary_heap_new(binary_heap_t * heap, size_t key_size, int (*compare)(void *, void *)) {
  heap->compare = compare;
  heap->array = malloc(sizeof(array_list_t));
  array_list_new(heap->array, key_size);
}

void binary_heap_free(binary_heap_t * heap) {
  array_list_free(heap->array);
  free(heap->array);
}

void binary_heap_insert(binary_heap_t * heap, void * key) {
  array_list_add(heap->array, key);
  binary_heap_bubble_up(heap, heap->array->size);
}
void binary_heap_extract(binary_heap_t * heap, void * ptr) {
  if(heap->array->size == 0) return;
  array_list_swap(heap->array, 0, heap->array->size - 1);
  array_list_remove(heap->array, heap->array->size - 1, ptr);
  if(heap->array->size == 0) return;
  binary_heap_bubble_down(heap, 0);
}

static void binary_heap_bubble_up(binary_heap_t * heap, int index) {

  void * child, * parent;
  int parent_index = (index - 1)/2;
  if(index == 0) return;
  array_list_get(heap->array, index, child);
  array_list_get(heap->array, parent_index, parent);
  while(heap->compare(child, parent) < 0) {
    /*Swap the nodes*/
    array_list_swap(heap->array, index, parent_index);
    index = parent_index;
    parent_index = (index - 1)/2;

    if(index == 0) return;
    array_list_get(heap->array, index, child);
    array_list_get(heap->array, parent_index, parent);
  }
}

static void binary_heap_bubble_down(binary_heap_t * heap, int index) {
  int left, right;
  void * left_ptr, * right_ptr, * parent_ptr;
  left_ptr = NULL; right_ptr = NULL; parent_ptr = NULL;
  while(1) {
    left = 2*index + 1;
    right = left + 1;

    array_list_get(heap->array, index, parent_ptr);

    /*No more children to swap with*/
    if(left >= heap->array->size) return;

    array_list_get(heap->array, left, left_ptr);
    if(right >= heap->array->size) {
      /*No Right child*/
      if(heap->compare(parent_ptr, left_ptr) > 0) {
        array_list_swap(heap->array, index, left);
      }
      return;
    }

    array_list_get(heap->array, right, right_ptr);
    /*Has two children*/
    if(heap->compare(left_ptr, right_ptr) < 0) {
      /*Swap the smaller one which is the left if necessary*/
      if(heap->compare(parent_ptr, left_ptr) < 0) return;
      array_list_swap(heap->array, index, left);
      index = left;
    } else {
      if(heap->compare(parent_ptr, right_ptr) < 0) return;
      array_list_swap(heap->array, index, right);
      index = right;
    }
  }
}
