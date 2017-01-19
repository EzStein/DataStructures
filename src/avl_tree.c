#include "avl_tree.h"
#include "queue.h"
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
static void pad_string(const char * string, size_t size, char * result);

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
  int bal;
  int8_t skip;
  avl_tree_node_t * node = tree->root;
  /*This double pointer is used to set the parents child pointer. Since it
  is unknown whether the parent's child is the left or right child or simply the root
  of the tree, this variable is used.*/
  avl_tree_node_t ** parents_child_ref;
  avl_tree_node_t * tmp_node;
  void * tmp_key;

  int8_t compare;
  int8_t to_remove_child_type;

  /*Search for the node to remove*/
  while(node) {
    compare = tree->compare(key, node->key);
    if(compare < 0) {
      node = node->left;
      to_remove_child_type = -1;
      if(node)
        parents_child_ref = &(node->parent->left);
    } else if (compare > 0) {
      node = node->right;
      to_remove_child_type = 1;
      if(node)
        parents_child_ref = &(node->parent->right);
    } else {
      /*Compare equals 0 and we found the node to remove*/
      break;
    }
  }
  /*node now points to the node containing the given key, or NULL if no such node existed*/
  if(!node) return;


  if(!node->parent) {
    parents_child_ref = &(tree->root);
  }

  /*parents_child_ref now points to the pointer that should point to the removed node in the tree*/
  /*We must now remove node*/
  if(node->left && node->right) {
    /*Find the first key that is larger than the one to remove*/
    tmp_node = node->right;
    parents_child_ref = &(node->right);
    to_remove_child_type = 1;
    while(tmp_node->left) {
      to_remove_child_type = -1;
      parents_child_ref = &(tmp_node->left);
      tmp_node = tmp_node->left;
    }
    /*Swap keys*/
    tmp_key = node->key;
    node->key = tmp_node->key;
    tmp_node->key = tmp_key;

    /*Set node to point to the node to remove*/
    node = tmp_node;
    /*Either splice out the node or simply remove it depending on whether it has a right child*/
    *parents_child_ref = node->right;

    if(node->right)
      node->right->parent = node->parent;
  } else if(node->left && !node->right) {
    /*Splice out the middle*/
    node->left->parent = node->parent;
    *parents_child_ref = node->left;
  } else if(!node->left && node->right) {
    /*Splice out the middle*/
    node->right->parent = node->parent;
    *parents_child_ref = node->right;
  } else {
    /*Remove a leaf*/
    *parents_child_ref = NULL;
  }

  /*Now we must rebalance the tree*/
  /*First we update the balance_factor of each node in the deleted path
  * We start by updating the deleted node's parent*/
  skip = 0;
  if(node->parent) {
    if(to_remove_child_type < 0) {
        node->parent->balance_factor++;
        if(node->parent->balance_factor >= 1) skip = 1;
    } else {
        node->parent->balance_factor--;
        if(node->parent->balance_factor <= -1) skip = 1;
    }
  }

  tmp_node = node->parent;
  /*REMOVE THE NODE*/
  tree->size--;
  if(tree->free_function)
    tree->free_function(node->key);
  free(node->key);
  free(node);
  node = tmp_node;

  if(!node) return;
  while(1) {
    while(!skip && node->parent && node->balance_factor >= -1 && node->balance_factor <= 1) {

      if(tree->compare(node->key, node->parent->key) < 0) {
        /*node is the left child*/
        node->parent->balance_factor++;
        /*When the parent's balance factor is 2 or 1 (formerly 1 or 0) that means the right sub tree was
        taller than or the same height as the left to begin with and the removal of one node did not effect the height of the parent.
        Thus we can stop moving up the tree*/
        if(node->parent->balance_factor >= 1) {
          node = node->parent;
          break;
        }
      } else {
        /*node is right child*/
        node->parent->balance_factor--;
        if(node->parent->balance_factor <= -1) {
          node = node->parent;
          break;
        }
      }
      node = node->parent;
    }

    /*No rebalancing necesary.*/
    if(node->balance_factor >= -1 && node->balance_factor <= 1) return;

    /*ROTATION TIME*/
    /*node's balance_factor is either 2 or -2*/
    if(node->balance_factor == 2) {
      /*node->right->balance_factor is either 1 0 or -1*/
      if(node->right->balance_factor == 1) {
        avl_tree_rotation(tree, node->right);
        node->balance_factor = 0;
        node->parent->balance_factor = 0;
      } else if(node->right->balance_factor == 0) {
        avl_tree_rotation(tree, node->right);
        node->balance_factor = 1;
        node->parent->balance_factor = -1;
      } else {
        bal = node->right->left->balance_factor;
        avl_tree_rotation(tree, node->right->left);
        avl_tree_rotation(tree, node->right);
        node->parent->balance_factor = 0;
        if(bal == 0) {
          node->balance_factor = 0;
          node->parent->right->balance_factor = 0;
        } else if(bal == 1) {
          node->balance_factor = -1;
          node->parent->right->balance_factor = 0;
        } else if(bal == -1) {
          node->balance_factor = 0;
          node->parent->right->balance_factor = 1;
        }
      }
    } else {
      /*node->right->balance_factor is either 1 0 or -1*/
      if(node->left->balance_factor == -1) {
        avl_tree_rotation(tree, node->left);
        node->balance_factor = 0;
        node->parent->balance_factor = 0;
      } else if(node->left->balance_factor == 0) {
        avl_tree_rotation(tree, node->left);
        node->balance_factor = -1;
        node->parent->balance_factor = 1;
      } else {
        bal = node->left->right->balance_factor;
        avl_tree_rotation(tree, node->left->right);
        avl_tree_rotation(tree, node->left);
        node->parent->balance_factor = 0;
        if(bal == 0) {
          node->balance_factor = 0;
          node->parent->left->balance_factor = 0;
        } else if(bal == 1) {
          node->balance_factor = 0;
          node->parent->left->balance_factor = -1;
        } else if(bal == -1) {
          node->balance_factor = 1;
          node->parent->left->balance_factor = 0;
        }
      }
    }
    /*The balancing decreased the subtree height so we must repeat the loop*/
    node = node->parent;
    if(!node->parent || node->balance_factor != 0) return;

    skip = 0;
    if(tree->compare(node->key, node->parent->key) < 0) {
      node->parent->balance_factor++;
      if(node->parent->balance_factor >= 1) skip = 1;
    } else {
      node->parent->balance_factor--;
      if(node->parent->balance_factor <= -1) skip = 1;
    }
    node = node->parent;
  }

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
    printf("%i: expected %i at node containing %i\n", node->balance_factor, h2-h1, *(int *)node->key);
    return 0;
  }
  if(h2 - h1 <= -2 || h2 - h1 >= 2) {
    printf("Node containing %i is imbalanced: %i\n",*(int *)node->key, h2-h1);
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
  char * buf;
  char * buf2;
  queue_t queue;
  avl_tree_node_t * node;
  int height, tmp, current_depth, current_index, tier_index, end, i, j, k, entry_width;
  long nul = 0;
  buf = malloc(100);
  buf2 = malloc(100);
  tmp = tree->size;
  for(height = 0; tmp; height++) {
    tmp >>= 1;
  }
  entry_width = 5;
  queue_new(&queue, sizeof(avl_tree_node_t *), NULL);
  if(tree->root)
    queue_enqueue(&queue, &(tree->root));
  current_index = 0;
  current_depth = 0;
  tier_index = 0;
  end = 0;
  while(!queue_is_empty(&queue)) {
    queue_dequeue(&queue, &node);
    if(tier_index == 0) {
      tmp = (1 << (height + 1))/(1 << (current_depth + 1));
      for(i = 0; i < tmp; i++) {
        for(j = 0; j < entry_width; j++) {
          printf(" ");
        }
      }
    }
    if(node) {
      sprintf(buf2, "%i", *(int *) node->key);
      pad_string(buf2, entry_width, buf);
      printf("%s", buf);
    } else {
      pad_string("#", entry_width, buf);
      printf("%s", buf);
    }
    tmp = (1 << (height + 1))/(1 << current_depth) - 1;
    for(i = 0; i < tmp; i++) {
      for(j = 0; j < entry_width; j++) {
        printf(" ");
      }
    }
    current_index++;
    tier_index++;
    if((current_index + 1) >> (current_depth + 1)) {
      if(end) break;
      end = 1;
      current_depth++;
      tier_index = 0;
      printf("\n");
      tmp = (1 << (height + 1))/(1 << (current_depth));
      for(i = 0; i < tmp; i++) {
        for(j = 0; j < entry_width; j++) {
          printf(" ");
        }
      }
      for(k = 0; k < (1 << (current_depth - 1)); k++) {
        pad_string("|", entry_width, buf);
        printf("%s", buf);
        tmp = (1 << (height + 1))/(1 << (current_depth - 1)) - 1;
        for(i = 0; i < tmp; i++) {
          for(j = 0; j < entry_width; j++) {
            printf(" ");
          }
        }
      }
      printf("\n");
      for(i = 0; i < entry_width - 2; i++)
        printf(" ");
      tmp = (1 << (height + 1))/(1 << (current_depth + 1));
      for(i = 0; i < tmp; i++) {
        for(j = 0; j < entry_width; j++) {
          printf(" ");
        }
      }
      for(k = 0; k < (1 << (current_depth)); k++) {
        tmp = (1 << (height + 1))/(1 << (current_depth)) - 1;

        for(i = 0; i < tmp + 1; i++) {
          for(j = 0; j < entry_width; j++) {
            if(!(k % 2))
              printf("-");
            else
              printf(" ");
          }
        }
      }
      printf("\n");
      tmp = (1 << (height + 1))/(1 << (current_depth + 1));
      for(i = 0; i < tmp; i++) {
        for(j = 0; j < entry_width; j++) {
          printf(" ");
        }
      }
      for(k = 0; k < (1 << (current_depth)); k++) {
        pad_string("|", entry_width, buf);
        printf("%s", buf);
        tmp = (1 << (height + 1))/(1 << (current_depth)) - 1;
        for(i = 0; i < tmp; i++) {
          for(j = 0; j < entry_width; j++) {
            printf(" ");
          }
        }
      }
      printf("\n");

    }

    if(node) end = 0;
    if(!node) {
      queue_enqueue(&queue, &nul);
      queue_enqueue(&queue, &nul);
      continue;
    }
    if(node->left)
      queue_enqueue(&queue, &(node->left));
    else
      queue_enqueue(&queue, &nul);

    if(node->right)
      queue_enqueue(&queue, &(node->right));
    else
      queue_enqueue(&queue, &nul);

  }
  queue_free(&queue);
  printf("\n");
  free(buf);
  free(buf2);
}

/*Note result must be at least size + 1 size
Assumes length of the string is less than the desired size*/
static void pad_string(const char * string, size_t size, char * result) {
  size_t init_len = strlen(string);
  size_t i;
  size_t pad_size = (size - init_len)/2;
  for(i = 0; i < pad_size; i++) {
    result[i] = ' ';
  }
  for(i = pad_size; i < pad_size + init_len; i++) {
    result[i] = string[i - pad_size];
  }
  for(i = pad_size + init_len; i < size; i++) {
    result[i] = ' ';
  }
  result[size] = '\0';
}
