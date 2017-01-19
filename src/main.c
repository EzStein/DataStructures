#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"
#include "queue.h"

int8_t compare(void * first, void * second);
void iterator(void * val, int a);
int main(void) {
  /*int a, i, j;
  avl_tree_t tree;
  srand(10000*time(NULL));*/
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

/*for(j = 0; j < 1000000; j++) {
  printf("%i\r", j);
  fflush(stdout);
  avl_tree_new(&tree, sizeof(int), NULL, compare);
  for(i = 0; i < 10000; i++) {
    a = rand() % 10000;

    avl_tree_add(&tree, &a);
  }
  for(i = 0; i < 10000; i++) {
    a = rand() % 10000;
    avl_tree_remove(&tree, &a);

  }
  a = avl_tree_invariant(&tree);
  if(!a) return 0;
  avl_tree_free(&tree);
}*/
  avl_tree_t tree;
  int a;
  avl_tree_new(&tree, sizeof(int), NULL, compare);
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
  avl_tree_print(&tree);
  avl_tree_free(&tree);
  /*queue_t queue;
  int a, i;
  queue_new(&queue, sizeof(int), NULL);
  for(i = 0; i < 100; i++) {
    a = i;
    queue_enqueue(&queue, &a);
  }
  for(i = 0; i < 50; i++) {
    queue_dequeue(&queue, &a);
    printf("%i\n", a);
  }
  for(i = 0; i < 50; i++) {
    a = i;
    queue_enqueue(&queue, &a);
  }
  for(i = 0; i < 100; i++) {
    queue_dequeue(&queue, &a);
    printf("%i\n", a);
  }

  queue_free(&queue);*/
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
