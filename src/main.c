#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"

int8_t compare(void * first, void * second);
void iterator(void * val, int a);
int main(void) {
  int a, i, j;
  avl_tree_t tree;
  srand(10000*time(NULL));
  /*for(j = 0; j < 100000; j++) {
    avl_tree_new(&tree, sizeof(int), NULL, compare);
    for(i = 0; i < 10; i++) {
      a = rand() % 10;
      printf("%i\n", a);
      avl_tree_add(&tree, &a);
    }
    for(i = 0; i < 10; i++) {
      a = rand() % 10;
      printf("Removed: %i\n", a);
      avl_tree_remove(&tree, &a);
      a = avl_tree_invariant(&tree);
      printf("%i\n", a);
      if(!a) return 0;
    }
    avl_tree_free(&tree);
  }*/

for(j = 0; j < 1000000; j++) {
  avl_tree_new(&tree, sizeof(int), NULL, compare);
  for(i = 0; i < 10000; i++) {
    a = rand() % 10000;
    printf("Adding %i\n", a);
    avl_tree_add(&tree, &a);
    a = avl_tree_invariant(&tree);
    if(!a) return 0;
  }
  for(i = 0; i < 10000; i++) {
    a = rand() % 10000;
    printf("Removing %i\n", a);
    avl_tree_remove(&tree, &a);
    a = avl_tree_invariant(&tree);
    if(!a) return 0;
  }
  avl_tree_free(&tree);
}
  /*avl_tree_new(&tree, sizeof(int), NULL, compare);
  a = 4;
  avl_tree_add(&tree, &a);
  a = 2;
  avl_tree_add(&tree, &a);
  a = 10;
  avl_tree_add(&tree, &a);
  a = 0;
  avl_tree_add(&tree, &a);
  a = 3;
  avl_tree_add(&tree, &a);
  a = 7;
  avl_tree_add(&tree, &a);
  a = 11;
  avl_tree_add(&tree, &a);
  a = 1;
  avl_tree_add(&tree, &a);
  a = 5;
  avl_tree_add(&tree, &a);
  a = 8;
  avl_tree_add(&tree, &a);
  a = 12;
  avl_tree_add(&tree, &a);
  a = 9;
  avl_tree_add(&tree, &a);
  a = 5;
  avl_tree_remove(&tree, &a);
  a = avl_tree_invariant(&tree);
  printf("%i\n", a);
  avl_tree_traverse_pre_order(&tree, iterator);
  printf("\n");
  avl_tree_free(&tree);*/
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
