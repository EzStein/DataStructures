#ifndef AVL_TREE_H
#define AVL_TREE_H
#include <stdlib.h>
typedef struct avl_tree avl_tree_t;
typedef struct avl_tree_node avl_tree_node_t;
struct avl_tree {
  avl_tree_node_t * root;
  void (*free_function)(void *);
  int8_t (*compare)(void *, void *);
  size_t key_size;
};

/*
* Initializes a new avl_tree using the given pointer.
* The tree does not allow duplicates.
*/
void avl_tree_new(avl_tree_t *, size_t, void (*free_function)(void *), int8_t (*compare)(void *, void *));
void avl_tree_add(avl_tree_t *, void *);
void avl_tree_remove(avl_tree_t *, void *);
int8_t avl_tree_contains(avl_tree_t *, void *);
void avl_tree_free(avl_tree_t *);

#endif
