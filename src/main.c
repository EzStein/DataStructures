#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bst_tree.h"

int8_t compare(void * first, void * second);
void iterator(void *);

int main(void) {
  int i, a;
  bst_tree_t tree;
  srand(time(NULL));
  bst_tree_new(&tree, sizeof(int), NULL, compare);

  for(i = 4000000; i >= 0; i--) {
    a = rand();
    bst_tree_add(&tree, &a);
  }
  bst_tree_traverse_in_order(&tree, iterator);

  bst_tree_free(&tree);
  return 0;
}

void iterator(void * ptr) {
  int val = *(int *) ptr;
  printf("%i\n", val);
}

int8_t compare(void * first, void * second) {
  int a = *((int *)first);
  int b = *((int *)second);
  if(a < b) return -1;
  if(a > b) return 1;
  return 0;
}
