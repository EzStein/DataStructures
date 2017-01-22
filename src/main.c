#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"

int8_t compare(void * first, void * second);
void iterator(void * val, int a);
int main(void) {
  avl_tree_t * tree;
  int a;
  tree = malloc(sizeof(avl_tree_t));
  avl_tree_new(tree, sizeof(int), NULL, compare);
  a = 50;
  avl_tree_add(tree, &a);
  a = 25;
  avl_tree_add(tree, &a);
  a = 75;
  avl_tree_add(tree, &a);
  a = 100;
  avl_tree_add(tree, &a);
  a = 60;
  avl_tree_add(tree, &a);
  a = 125;
  avl_tree_add(tree, &a);
  avl_tree_print(tree);
  avl_tree_free(tree);
  free(tree);

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
