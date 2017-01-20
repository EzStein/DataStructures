#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"
#include "queue.h"
#include "lifo_stack.h"
#include "binary_heap.h"
#include "array_list.h"

int8_t compare(void * first, void * second);
void iterator(void * val, int a);
int main(void) {
  int i, a;
  array_list_t array;
  array_list_new(&array, sizeof(int));
  for(i = 0; i < 100; i++)
    array_list_add_at(&array, 0, &i);
  for(i = 99; i >= 0; i--) {
    array_list_get(&array, i, &a);
    printf("%i\n", a);
  }

  array_list_free(&array);
  /*binary_heap_t heap;
  int a, i;
  srand(10000*time(NULL));
  binary_heap_new(&heap, sizeof(int), compare);
  for(i = 0; i < 1000; i++) {
    a = rand() % 1000;
    binary_heap_insert(&heap, &a);
  }
  for(i = 0; i < 500; i++) {
    binary_heap_extract(&heap, &a);
    printf("%i\n", a);
  }
  for(i = 0; i < 1000; i++) {
    a = rand() % 1000;
    binary_heap_insert(&heap, &a);
  }
  for(i = 0; i < 1000; i++) {
    binary_heap_extract(&heap, &a);
    printf("%i\n", a);
  }
  for(i = 0; i < 1000; i++) {
    a = rand() % 1000;
    binary_heap_insert(&heap, &a);
  }
  for(i = 0; i < 1500; i++) {
    binary_heap_extract(&heap, &a);
    printf("%i\n", a);
  }
  binary_heap_free(&heap);*/
  return 0;
}

int8_t compare(void * first, void * second) {
  int a = *((int *)first);
  int b = *((int *)second);
  if(a < b) return -1;
  if(a > b) return 1;
  return 0;
}

void iterator(void * val, int a) {
  a = *(int *)val;
  printf("%i ", a);
}
