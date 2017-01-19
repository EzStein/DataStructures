#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H
typedef struct binary_heap binary_heap_t;
struct binary_heap {
  int size;
  int capacity;
  size_t key_size;
  void ** array;
  int8_t (*compare)(void *, void *);
  void (*free_function)(void *);
};
/*NOTE: This is a min-heap*/
void binary_heap_new(binary_heap_t *, size_t key_size, int8_t (*compare)(void *, void *), void (*free_function)(void *));
void binary_heap_free(binary_heap_t *);
void binary_heap_insert(binary_heap_t *, void *);
void binary_heap_extract(binary_heap_t *, void *);
#endif