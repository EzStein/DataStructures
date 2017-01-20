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
