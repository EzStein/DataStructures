# Data Structures and Sorting Algorithms in C
This project is a collection of many generic data structures and sorting algorithms used throughout
computer science. Structures include trees, heaps, hashes, lists and graphs. All structures are
written in plain old C in compliance with the C89 standard with as much portability as possible.

##Generic Simulation
All structures are generic meaning they handle solely void pointers as their data.
The void pointers point to areas in memory that are maintained by the structure internally. By
default all structures maintain their **own** copy of any memory given to them. For instance, when
adding an element to a structure, a pointer to that element is passed to the structure.
The functions will then copy the memory at that pointer into the structure's own memory.
When retrieving something from a structure a pointer to some vacant memory is passed in.
The structure will then copy its own memory into the vacant memory pointed to by the provided
pointer. Note that the provided pointer must point to valid memory of the right size. In some cases
the structure will remove an element. In this case, the structure will free its own memory. It is
a mistake to free any pointer that you did not explicitly allocate. Some structures require a comparator
which takes two pointers and compares them. Note that these pointers point to memory handled by the
structure internally. It is a mistake to free these pointers in the compare method, modify them
in any way, or use them outside of the compare method. If you do not want the structure to copy
all data given to it into internal memory, you may pass it a double pointer in its add element functions.
Effectively the structure will do the same thing as before only it will maintain pointers to pointers that point
to memory handled outside of the structure.

## Structures and Algorithms:
### Lists:
* Linked List
* Queue (queue.h)
* Stack (lifo_stack.h)

### Trees:
* Binary Search Tree (bst_tree.h)
* AVL Tree (avl_tree.h)
* Splay Tree
* 2-3 Tree
* B-Tree

### Heaps:
* Binary Heap (binary_heap.h)
* Binomial Heap

### Hashes:
* Hash Table
