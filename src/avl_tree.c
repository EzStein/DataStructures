#include "avl_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint8_t avl_tree_sub_tree_invariant(avl_tree_t *, avl_tree_node_t *);
static void avl_tree_rotation(avl_tree_t *, avl_tree_node_t *);
static void avl_tree_free_sub_tree(void (*free_function)(void *), avl_tree_node_t *);
static void avl_tree_traverse_sub_tree_in_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t);
static void avl_tree_traverse_sub_tree_pre_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t);
static void avl_tree_traverse_sub_tree_post_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t);
static int avl_tree_height_sub_tree(avl_tree_t * tree, avl_tree_node_t * node);

struct avl_tree_node {
  int8_t balance_factor;
  avl_tree_node_t * parent, * left, * right;
  void * key;
};

/**
* Initializes the passed in pointer with the given values.
* The tree pointer may be allocated on the static or the heap,
* however all nodes in the tree will be allocated on the heap.
* Thus in general for every call to avl_tree_new you must call avl_tree_free at
* which point the tree must be reinitialized if it is to be used again.
*/
void avl_tree_new(avl_tree_t * tree, size_t key_size, void (*free_function)(void *), int8_t (*compare)(void *, void *)) {
  tree->key_size = key_size;
  tree->free_function = free_function;
  tree->compare = compare;
  tree->root = NULL;
  tree->size = 0;
}

/**
* Adds the the key to the tree in the appropriate spot. Rebalancing is then
* performed to maintain the avl invariant.
*/
void avl_tree_add(avl_tree_t * tree, void * key) {
  int8_t last_compare;
  avl_tree_node_t * tmp_node;
  avl_tree_node_t * tmp_parent_node;
  avl_tree_node_t * new_node;
  int bal;

  /*Search for the leaf to insert the node to*/
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
      /*The key already exists in the tree*/
      return;
    }
  }
  /*tmp_node is now null and tmp_parent_node is a node which will become the parent
  * the parent of the new key*/
  tree->size++;

  /*Allocate a new node. The key is copied into the node.*/
  new_node = malloc(sizeof(avl_tree_node_t));
  new_node->key = malloc(tree->key_size);
  memcpy(new_node->key, key, tree->key_size);

  /*The new node is a leaf*/
  new_node->left = new_node->right = NULL;
  new_node->parent = tmp_parent_node;
  new_node->balance_factor = 0;

  if(!tree->root) {
    tree->root = new_node;
  } else if(last_compare < 0) {
    tmp_parent_node->left = new_node;
  } else {
    tmp_parent_node->right = new_node;
  }

  /*Time to rebalance the tree. First we update the balance factors along the path from the leaf to
  the root. Not all nodes will need updating.*/
  tmp_node = new_node;
  while(tmp_node->parent && tmp_node->balance_factor >= -1 && tmp_node->balance_factor <= 1) {
    if(tree->compare(tmp_node->key, tmp_node->parent->key) < 0) {
      /*tmp_node is the left child of tmp_node->parent*/
      tmp_node->parent->balance_factor--;

      /*when tmp_node->parent->balance_factor is zero than its previous value was 1.
      Thus its right child had a greator height, and the total height of the tmp_node->parent
      remains unchanged. Thus no other changes need to be made above tmp_node->parent*/
      if(tmp_node->parent->balance_factor == 0) break;
    } else {
      /*tmp_node is the right child of tmp_node->parent*/
      tmp_node->parent->balance_factor++;

      /*when tmp_node->parent->balance_factor is zero then its previous value was -1.
      Thus its left child had a greator height than its right, and the total height of the tmp_node->parent
      remains unchanged by incrementing the height of the right.
      Thus no other changes need to be made above tmp_node->parent*/
      if(tmp_node->parent->balance_factor == 0) break;
    }
    tmp_node = tmp_node->parent;
  }
  /*Either tmp_node points to the root, or an unbalanced node*/
  /*The root has been reached and the root is balanced*/
  if(tmp_node->balance_factor >= -1 && tmp_node->balance_factor <= 1) return;

  /*tmp_node is unbalanced. Tree rotations will be used to balance the node and restore
  the balance factors of each affected node. Note that traversing up the tree
  to restore balance factors above tmp_node is unnecessary since the height of tmp_node
  remains unchanged by the addition of a descendent*/
  /*GENERAL CASES:*/
  /*tmp_node->balance_factor is either 2 or -2*/
  if(tmp_node->balance_factor == 2) {
    /*tmp_node->right->balance_factor is either 1 or -1*/
    if(tmp_node->right->balance_factor == 1) {
      avl_tree_rotation(tree, tmp_node->right);
      tmp_node->parent->balance_factor = 0;
      tmp_node->balance_factor = 0;
    } else {
      bal = tmp_node->right->left->balance_factor;
      avl_tree_rotation(tree, tmp_node->right->left);
      avl_tree_rotation(tree, tmp_node->right);

      /*bal is either 1 or -1 or 0 when degenerate*/
      if(bal == 1) {
        tmp_node->balance_factor = -1;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->right->balance_factor = 0;
      } else if(bal == -1){
        tmp_node->balance_factor = 0;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->right->balance_factor = 1;
      } else {
        tmp_node->balance_factor = 0;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->right->balance_factor = 0;
      }
    }
  } else {
    /*tmp_node->left->balance_factor is either 1 or -1*/
    if(tmp_node->left->balance_factor == -1) {
      avl_tree_rotation(tree, tmp_node->left);
      tmp_node->parent->balance_factor = 0;
      tmp_node->balance_factor = 0;
    } else {
      bal = tmp_node->left->right->balance_factor;
      avl_tree_rotation(tree, tmp_node->left->right);
      avl_tree_rotation(tree, tmp_node->left);
      /*bal is either 1 or -1 or 0 when degenerate*/
      if(bal == 1) {
        tmp_node->balance_factor = 0;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->left->balance_factor = -1;
      } else if(bal == -1){
        tmp_node->balance_factor = 1;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->left->balance_factor = 0;
      } else {
        tmp_node->balance_factor = 0;
        tmp_node->parent->balance_factor = 0;
        tmp_node->parent->left->balance_factor = 0;
      }
    }
  }
}

void avl_tree_remove(avl_tree_t * tree, void * key) {
  avl_tree_node_t * node = tree->root;
  avl_tree_node_t ** parents_child_ref;
  avl_tree_node_t * tmp_node;
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

  tree->size--;
  if(tree->free_function)
    tree->free_function(node->key);
  free(node->key);
  free(node);
}

int8_t avl_tree_contains(avl_tree_t * tree, void * key) {
  int8_t compare;
  avl_tree_node_t * node = tree->root;
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

void avl_tree_free(avl_tree_t * tree) {
  avl_tree_free_sub_tree(tree->free_function, tree->root);
}

static void avl_tree_free_sub_tree(void (*free_function)(void *), avl_tree_node_t * node) {
  if(!node) return;
  avl_tree_free_sub_tree(free_function, node->left);
  avl_tree_free_sub_tree(free_function, node->right);

  if(free_function)
    free_function(node->key);

  free(node->key);
  free(node);
}

int avl_tree_size(avl_tree_t * tree) {
  return tree->size;
}

void avl_tree_traverse_in_order(avl_tree_t * tree, void (*iterator)(void *, int)) {
  avl_tree_traverse_sub_tree_in_order(tree->root, iterator, 1);
}

void avl_tree_traverse_pre_order(avl_tree_t * tree, void (*iterator)(void *, int)) {
  avl_tree_traverse_sub_tree_pre_order(tree->root, iterator, 1);
}

void avl_tree_traverse_post_order(avl_tree_t * tree, void (*iterator)(void *, int)) {
  avl_tree_traverse_sub_tree_post_order(tree->root, iterator, 1);
}

static void avl_tree_traverse_sub_tree_in_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t reset) {
  static int index = 0;
  if(reset) index = 0;
  if(!node) return;
  avl_tree_traverse_sub_tree_in_order(node->left, iterator, 0);
  iterator(node->key, index);
  index++;
  avl_tree_traverse_sub_tree_in_order(node->right, iterator, 0);
}

static void avl_tree_traverse_sub_tree_pre_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t reset) {
  static int index = 0;
  if(reset) index = 0;
  if(!node) return;
  iterator(node->key, index);
  index++;
  avl_tree_traverse_sub_tree_pre_order(node->left, iterator, 0);
  avl_tree_traverse_sub_tree_pre_order(node->right, iterator, 0);
}

static void avl_tree_traverse_sub_tree_post_order(avl_tree_node_t * node, void (*iterator)(void *, int), uint8_t reset) {
  static int index = 0;
  if(reset) index = 0;
  if(!node) return;
  avl_tree_traverse_sub_tree_post_order(node->left, iterator, 0);
  avl_tree_traverse_sub_tree_post_order(node->right, iterator, 0);
  iterator(node->key, index);
  index++;
}

/**
* Performs a tree rotation with the given node as the pivot if possible.
* Results in premoting the node up one level, and demoting its parent down one level.
* This method fails if the pivot is the root.
* NOTE: This function will NOT update the balance_factor of the nodes involved.
*/
static void avl_tree_rotation(avl_tree_t * tree, avl_tree_node_t * pivot) {
  int compare;
  avl_tree_node_t * root;
  root = pivot->parent;
  if(!root) return;

  /*Set up the ptr ptr from the first unchanged node to root of the changing subtree*/
  if(!root->parent) {
    tree->root = pivot;
  } else {
    compare = tree->compare(root->key, root->parent->key);
    if(compare < 0) {
      root->parent->left = pivot;
    } else {
      root->parent->right = pivot;
    }
  }
  pivot->parent = root->parent;
  root->parent = pivot;
  compare = tree->compare(pivot->key, root->key);
  if(compare < 0) {
    root->left = pivot->right;
    pivot->right = root;
    if(root->left)
      root->left->parent = root;
  } else {
    root->right = pivot->left;
    pivot->left = root;
    if(root->right)
      root->right->parent = root;
  }
}

uint8_t avl_tree_invariant(avl_tree_t * tree) {
  return avl_tree_sub_tree_invariant(tree, tree->root);
}

static uint8_t avl_tree_sub_tree_invariant(avl_tree_t * tree, avl_tree_node_t * node) {
  int h1, h2;
  if(!node) return 1;
  if(node->left) {
    if(tree->compare(node->left->key, node->key) >= 0)
      return 0;
    if(node->left->parent != node)
      return 0;
  }
  if(node->right) {
    if(tree->compare(node->right->key, node->key) <= 0)
      return 0;
    if(node->right->parent != node)
      return 0;
  }
  h1 = avl_tree_height_sub_tree(tree, node->left);
  h2 = avl_tree_height_sub_tree(tree, node->right);

  if(h2 - h1 != node->balance_factor){
    printf("%i: expected %i\n", node->balance_factor, h2-h1);
    return 0;
  }

  if(!avl_tree_sub_tree_invariant(tree, node->left)) return 0;
  if(!avl_tree_sub_tree_invariant(tree, node->right)) return 0;
  return 1;
}

/**
* NOTE THIS IS EXTREMELY INEFFICIENT AND SHOULD BE USED FOR TESTING ONLY
*/
static int avl_tree_height_sub_tree(avl_tree_t * tree, avl_tree_node_t * node) {
  int h1, h2;
  if(!node) return 0;
  h1 = avl_tree_height_sub_tree(tree, node->left);
  h2 = avl_tree_height_sub_tree(tree, node->right);
  return h1>h2?h1+1:h2+1;
}

void avl_tree_print(avl_tree_t * tree) {
  /*A crude queue*/
  int index = 0, i, offset;
  int max_index = 1;
  int depth = 0;
  int pop = 0;
  int push = 0;
  avl_tree_node_t ** nodes = malloc(1000 * sizeof(avl_tree_node_t *));
  avl_tree_node_t * node;

  int height = 0;
  int val = tree->size;
  while((val = val >> 1)) {height++;}

  val = 1;
  while(height) {
    val = 2 * val;
    height--;
  }

  nodes[push++] = tree->root;
  nodes[push++] = NULL; /*INDICATING THE END OF A LINE*/
  while(push != pop) {
    node = nodes[pop++];
    if(!node) {

      depth++;
      index=0;
      if(push != pop) {
        nodes[push++] = NULL;
      }

      printf("\n");
      offset = val/max_index/2;
      for(i = 0; i < val; i++) {
        if((i-offset)%(val/max_index) == 0)
          printf("|");
        else
          printf(" ");
      }
      printf("\n");
      for(i = 0; i < val; i++) {
        printf("-");
      }
      printf("\n");
      max_index = max_index * 2;
      offset = val/max_index/2;
      for(i = 0; i < val; i++) {
        if((i-offset)%(val/max_index) == 0)
          printf("|");
        else
          printf(" ");
      }
      printf("\n");
      continue;
    }
    if(node->left)
      nodes[push++] = node->left;
    if(node->right)
      nodes[push++] = node->right;

    if(index == 0) {
      for(i = 0; i < val/max_index/2; i++) {
        printf(" ");
      }
    }
    printf("%i", node->balance_factor+1);
    for(i = 0; i < val/max_index - 1; i++) {
      printf(" ");
    }
    index++;
  }
  free(nodes);
}
