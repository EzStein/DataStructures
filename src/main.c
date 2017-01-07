#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"

int8_t compare(void * first, void * second);

int main(void) {
  int i;
  int b;
  avl_tree_t tree;
  avl_tree_new(&tree, sizeof(int), NULL, compare);
  for(i = 0; i < 100; i++) {
    b = i*(i << 6);
    avl_tree_add(&tree, &b);
  }
  for(i = 0; i < 100; i+=2+i) {
    b = i*(i << 6);
    avl_tree_remove(&tree, &b);
  }
  for(i = 0; i < 100; i++) {
    b = i*(i << 6);
    printf("%i\n", avl_tree_contains(&tree, &b));
  }
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
