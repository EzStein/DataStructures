#include <stdio.h>
#include "minunit.h"
int tests_run = 0;
static char * test1() {
  mu_assert("1 == 2", 1 != 2);
  return 0;
}

static char * test2() {
  mu_assert("1 != 1", 1 == 1);
  return 0;
}

static char * run_all_tests() {
    mu_run_test(test1);
    mu_run_test(test2);
    return 0;
}

int main() {
  char * result = run_all_tests();
  if(result) {
    printf("%s\n", result);
  } else {
    printf("All tests passed!\n");
  }
  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
