#include "bst_tree.h"
#include <stdlib.h>
#include <string.h>

static void bst_tree_free_sub_tree(void (*free_function)(void *), bst_tree_node_t *);
static void bst_tree_traverse_sub_tree_in_order(bst_tree_node_t * node, void (*iterator)(void *));

struct bst_tree_node {
  bst_tree_node_t * parent, * left, * right;
  void * key;
};

void bst_tree_new(bst_tree_t * tree, size_t key_size, void (*free_function)(void *), int8_t (*compare)(void *, void *)) {
  tree->key_size = key_size;
  tree->free_function = free_function;
  tree->compare = compare;
  tree->root = NULL;
}

void bst_tree_add(bst_tree_t * tree, void * key) {
  int8_t last_compare;
  bst_tree_node_t * tmp_node;
  bst_tree_node_t * tmp_parent_node;

  bst_tree_node_t * new_node = malloc(sizeof(bst_tree_node_t));
  new_node->key = malloc(tree->key_size);
  memcpy(new_node->key, key, tree->key_size);
  new_node->left = new_node->right = NULL;

  if(!tree->root) {
    new_node->parent = NULL;
    tree->root = new_node;
    return;
  }
  tmp_node = tree->root;
  tmp_parent_node = NULL;
  while(tmp_node) {
    tmp_parent_node = tmp_node;
    last_compare = tree->compare(key, tmp_node->key);
    if(last_compare < 0) {
      tmp_node = tmp_node->left;
    } else if (last_compare > 0) {
      tmp_node = tmp_node->right;
    } else {
      return;
    }
  }
  new_node->parent = tmp_parent_node;
  if(last_compare < 0) {
    tmp_parent_node->left = new_node;
  } else {
    tmp_parent_node->right = new_node;
  }
}

void bst_tree_remove(bst_tree_t * tree, void * key) {
  bst_tree_node_t * node = tree->root;
  bst_tree_node_t ** parents_child_ref;
  bst_tree_node_t * tmp_node;
  void * tmp_key;

  int8_t compare;
  while(node) {
    compare = tree->compare(key, node->key);
    if(compare < 0) {
      node = node->left;
      if(node)
        parents_child_ref = &(node->parent->left);
    } else if (compare > 0) {
      node = node->right;
      if(node)
        parents_child_ref = &(node->parent->right);
    } else {
      break;
    }
  }

  if(!node) return;

  /*We must now remove node*/
  if(!node->parent) {
    parents_child_ref = &(tree->root);
  }


  if(node->left && node->right) {
    tmp_node = node->right;
    parents_child_ref = &(node->right);
    while(tmp_node->left) {
      parents_child_ref = &(tmp_node->left);
      tmp_node = tmp_node->left;
    }
    tmp_key = node->key;
    node->key = tmp_node->key;
    tmp_node->key = tmp_key;

    node = tmp_node;
    *parents_child_ref = node->right;

    if(node->right)
      node->right->parent = node->parent;
  } else if(node->left && !node->right) {
    node->left->parent = node->parent;
    *parents_child_ref = node->left;
  } else if(!node->left && node->right) {
    node->right->parent = node->parent;
    *parents_child_ref = node->right;
  } else {
    *parents_child_ref = NULL;
  }

  if(tree->free_function)
    tree->free_function(node->key);
  free(node->key);
  free(node);
}

int8_t bst_tree_contains(bst_tree_t * tree, void * key) {
  int8_t compare;
  bst_tree_node_t * node = tree->root;
  while(node) {
    compare = tree->compare(key, node->key);
    if(compare < 0) {
      node = node->left;
    } else if(compare > 0) {
      node = node->right;
    } else {
      return 1;
    }
  }
  return 0;
}

void bst_tree_free(bst_tree_t * tree) {
  bst_tree_free_sub_tree(tree->free_function, tree->root);
}

static void bst_tree_free_sub_tree(void (*free_function)(void *), bst_tree_node_t * node) {
  if(!node) return;
  bst_tree_free_sub_tree(free_function, node->left);
  bst_tree_free_sub_tree(free_function, node->right);

  if(free_function)
    free_function(node->key);

  free(node->key);
  free(node);
}

void bst_tree_traverse_in_order(bst_tree_t * tree, void (*iterator)(void *)) {
  bst_tree_traverse_sub_tree_in_order(tree->root, iterator);
}

static void bst_tree_traverse_sub_tree_in_order(bst_tree_node_t * node, void (*iterator)(void *)) {
  if(!node) return;
  bst_tree_traverse_sub_tree_in_order(node->left, iterator);
  iterator(node->key);
  bst_tree_traverse_sub_tree_in_order(node->right, iterator);
}
