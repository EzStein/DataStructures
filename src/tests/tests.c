#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../array_list.h"
#include "minunit.h"
#include "../bst_tree.h"

static int traverse_arr[10];
static char str_buf[1024];

int tests_run = 0;

static int8_t compare(void *, void *);
static char * bst_tree_test();
static char * array_list_test();
static void iterator(void *, int);
static char * test_traversal(bst_tree_t * tree, int expected_size, int * in_order_arr, int * pre_order_arr, int * post_order_arr);

static void iterator(void * ptr, int index) {
  int val = *(int *)ptr;
  traverse_arr[index] = val;
}

static int8_t compare(void * a, void * b) {
  int aInt = *(int *)a;
  int bInt = *(int *)b;
  if(aInt < bInt) return -1;
  if(aInt > bInt) return 1;
  return 0;
}

static char * bst_tree_test() {
  int val;
  char * msg;
  int tmp_pre_order_arr[10] = {5, 2, 0, 1, 4, 3, 8, 6, 7, 9};
  int tmp_post_order_arr[10] = {1, 0, 3, 4, 2, 7, 6, 9, 8, 5};
  int tmp_in_order_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int * in_order_arr = tmp_in_order_arr;
  int * pre_order_arr = tmp_pre_order_arr;
  int * post_order_arr = tmp_post_order_arr;
  int original_pre[10];
  int original_post[10];
  int original_in[10];
  bst_tree_t tree;
  memcpy(original_pre, pre_order_arr, 10 * sizeof(int));
  memcpy(original_post, post_order_arr, 10 * sizeof(int));
  memcpy(original_in, in_order_arr, 10 * sizeof(int));
  /* Creates a tree with items added in as : 5 2 8 6 9 7 4 3 0 1
  *  Expected in order : 0 1 2 3 4 5 6 7 8 9
  *  Expected pre order : 5 2 0 1 4 3 8 6 7 9
  *  Expected post order : 1 0 3 4 2 7 6 9 8 5
  */
  bst_tree_new(&tree, sizeof(int), NULL, compare);
  val = 5;
  bst_tree_add(&tree, &val);
  val = 2;
  bst_tree_add(&tree, &val);
  val = 8;
  bst_tree_add(&tree, &val);
  val = 6;
  bst_tree_add(&tree, &val);
  val = 9;
  bst_tree_add(&tree, &val);
  val = 7;
  bst_tree_add(&tree, &val);
  val = 4;
  bst_tree_add(&tree, &val);
  val = 3;
  bst_tree_add(&tree, &val);
  val = 0;
  bst_tree_add(&tree, &val);
  val = 1;
  bst_tree_add(&tree, &val);

  msg = test_traversal(&tree, 10, in_order_arr, pre_order_arr, post_order_arr);
  if(msg) return msg;

  val = 7;
  bst_tree_remove(&tree, &val);
  in_order_arr[7] = 8;
  in_order_arr[8] = 9;
  pre_order_arr[8] = 9;
  post_order_arr[5] = 6;
  post_order_arr[6] = 9;
  post_order_arr[7] = 8;
  post_order_arr[8] = 5;
  msg = test_traversal(&tree, 9, in_order_arr, pre_order_arr, post_order_arr);
  if(msg) return msg;

  in_order_arr = original_in;
  pre_order_arr = original_pre;
  post_order_arr = original_post;
  val = 7;
  bst_tree_add(&tree, &val);
  msg = test_traversal(&tree, 10, in_order_arr, pre_order_arr, post_order_arr);
  if(msg) return msg;

  val = 8;
  bst_tree_remove(&tree, &val);
  in_order_arr[8] = 9;
  pre_order_arr[6] = 9;
  pre_order_arr[7] = 6;
  pre_order_arr[8] = 7;
  post_order_arr[8] = 5;
  msg = test_traversal(&tree, 9, in_order_arr, pre_order_arr, post_order_arr);
  if(msg) return msg;


  bst_tree_free(&tree);
  return 0;
}

static char * test_traversal(bst_tree_t * tree, int expected_size, int * in_order_arr, int * pre_order_arr, int * post_order_arr) {
  int i;
  static version = 0;
  int size = bst_tree_size(tree);
  sprintf(str_buf, "[%i] Expected size %i, found %i", version, expected_size, size);
  mu_assert(str_buf, size == expected_size);
  bst_tree_traverse_in_order(tree, iterator);
  for(i = 0; i < size; i++) {
    sprintf(str_buf, "[%i] IN ORDER: iteration %i. Expected %i, found %i", version, i, in_order_arr[i], traverse_arr[i]);
    mu_assert(str_buf, traverse_arr[i] == in_order_arr[i]);
  }

  bst_tree_traverse_pre_order(tree, iterator);
  for(i = 0; i < size; i++) {
    sprintf(str_buf, "[%i] PRE ORDER: iteration %i. Expected %i, found %i", version, i, pre_order_arr[i], traverse_arr[i]);
    mu_assert(str_buf, traverse_arr[i] == pre_order_arr[i]);
  }

  bst_tree_traverse_post_order(tree, iterator);
  for(i = 0; i < size; i++) {
    sprintf(str_buf, "[%i] POST ORDER: iteration %i. Expected %i, found %i", version, i, post_order_arr[i], traverse_arr[i]);
    mu_assert(str_buf, traverse_arr[i] == post_order_arr[i]);
  }

  for(i = 0; i < size; i++) {
    sprintf(str_buf, "[%i] CONTAINS: iteration %i. Expected to contain %i", version, i, post_order_arr[i]);
    mu_assert(str_buf, bst_tree_contains(tree, &(in_order_arr[i])));
  }
  version++;
  return 0;
}

static char * array_list_test() {
  array_list_t list;
  int tmp, tmp1, tmp2, i, index1, index2;
  array_list_new(&list, sizeof(int));
  /*Test Size*/
  for(i = 0; i < 1000; i++) {

    tmp = array_list_size(&list);
    sprintf(str_buf, "array_list_size() gives %i as size. Expected %i.", tmp, i);
    mu_assert(str_buf, tmp == i);
    array_list_add(&list, &i);
  }

  /*Test get*/
  for(i = 0; i < 1000; i++) {
    array_list_get(&list, i, &tmp);
    sprintf(str_buf, "array_list_get() gives %i as element. Expected %i.", tmp, i);
    mu_assert(str_buf, tmp == i);
  }

  /*Test remove from front*/
  for(i = 0; i < 1000; i++) {
    array_list_remove(&list, 0, &tmp);
    sprintf(str_buf, "array_list_remove() gives %i as element. Expected %i.", tmp, i);
    mu_assert(str_buf, tmp == i);
    tmp = array_list_size(&list);
    sprintf(str_buf, "array_list_size() gives %i as size. Expected %i.", tmp, 999 - i);
    mu_assert(str_buf, tmp == 999 - i);
  }

  /*Rebuild array*/
  for(i = 0; i < 1000; i++) {
    array_list_add(&list, &i);
  }

  /*Test remove from last*/
  for(i = 0; i < 1000; i++) {
    array_list_remove(&list, 999 - i, &tmp);
    sprintf(str_buf, "array_list_remove() gives %i as element. Expected %i.", tmp, 999 - i);
    mu_assert(str_buf, tmp == 999 - i);
    tmp = array_list_size(&list);
    sprintf(str_buf, "array_list_size() gives %i as size. Expected %i.", tmp, 999 - i);
    mu_assert(str_buf, tmp == 999 - i);
  }

  /*Rebuild array by inserting at head and test get*/
  for(i = 0; i < 1000; i++) {
    array_list_insert(&list, 0, &i);
    array_list_get(&list, 0, &tmp);
    sprintf(str_buf, "array_list_get() gives %i as element. Expected %i.", tmp, i);
    mu_assert(str_buf, tmp == i);
  }

  /*Test clear*/
  array_list_clear(&list);
  tmp = array_list_size(&list);
  sprintf(str_buf, "array_list_size() gives %i as size. Expected %i.", tmp, 0);
  mu_assert(str_buf, tmp == 0);

  i = 999;
  array_list_insert(&list, 0, &i);
  /*Test insert second to last*/
  for(i = 0; i < 998; i++) {
    array_list_insert(&list, i, &i);
    array_list_get(&list, i, &tmp);
    sprintf(str_buf, "array_list_get() gives %i as element. Expected %i.", tmp, i);
    mu_assert(str_buf, tmp == i);
  }

  /*Test Swap*/
  for(i = 0; i < 1000; i++) {
    index1 = rand() % 1000;
    index2 = rand() & 1000;
    array_list_get(&list, index1, &tmp1);
    array_list_get(&list, index2, &tmp2);
    array_list_swap(&list, index1, index2);
    array_list_get(&list, index1, &tmp);
    sprintf(str_buf, "array_list_get() gives %i as element. Expected %i.", tmp, tmp2);
    mu_assert(str_buf, tmp == tmp2);
    array_list_get(&list, index2, &tmp);
    sprintf(str_buf, "array_list_get() gives %i as element. Expected %i.", tmp, tmp1);
    mu_assert(str_buf, tmp == tmp1);
  }

  array_list_free(&list);
  return 0;
}

static char * run_all_tests() {
    mu_run_test(bst_tree_test);
    mu_run_test(array_list_test);
    return 0;
}

int main() {
  char * result;
  printf("%s\n", "RUNNING...");
  srand(time(NULL));
  result = run_all_tests();
  if(result) {
    printf("%s\n", result);
  } else {
    printf("All tests passed!\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
