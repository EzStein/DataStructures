#ifndef BST_TREE_H
#define BST_TREE_H
#include <stdlib.h>
typedef struct bst_tree bst_tree_t;
typedef struct bst_tree_node bst_tree_node_t;
struct bst_tree {
  bst_tree_node_t * root;
  void (*free_function)(void *);
  int8_t (*compare)(void *, void *);
  size_t key_size;
};

/*
* Initializes a new bst_tree using the given pointer.
* The tree does not allow duplicates.
*/
void bst_tree_new(bst_tree_t *, size_t, void (*free_function)(void *), int8_t (*compare)(void *, void *));
void bst_tree_add(bst_tree_t *, void *);
void bst_tree_remove(bst_tree_t *, void *);
int8_t bst_tree_contains(bst_tree_t *, void *);
void bst_tree_free(bst_tree_t *);
void bst_tree_traverse_in_order(bst_tree_t *, void (*iterator)(void *));

#endif
