#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"

int8_t compare(void * first, void * second);

int main(void) {
  int a, i;
  avl_tree_t tree;
  srand(time(NULL));
  avl_tree_new(&tree, sizeof(int), NULL, compare);
  for(i = 0; i < 1000000; i++) {
    a = rand();
    printf("%i\n", a);
    avl_tree_add(&tree, &a);
  }
  a = avl_tree_invariant(&tree);
  printf("%i\n", a);
  avl_tree_free(&tree);
  return 0;
}

int8_t compare(void * first, void * second) {
  int a = *((int *)first);
  int b = *((int *)second);
  if(a < b) return -1;
  if(a > b) return 1;
  return 0;
}
