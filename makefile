define COMPILE
@echo COMPILING: $^ INTO $@
	@mkdir obj > /dev/null 2>&1 ||:
	@gcc $(FLAGS) -c $(filter src/%.c, $^) -o $@
endef
define LINK
@echo LINKING: $^ INTO $@
	@mkdir exe > /dev/null 2>&1 ||:
	@gcc $(FLAGS) $^ -o $@
	@echo BUILD SUCCESFUL
endef

OBJECTS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
FLAGS = -g -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
-Wstrict-prototypes -Wold-style-definition

all : exe/a.out exe/tests.out
tests : exe/tests.out
exe : exe/a.out

exe/a.out: $(OBJECTS)
	$(LINK)

exe/tests.out : obj/tests.o obj/array_list.o obj/bst_tree.o
	$(LINK)

obj/main.o : src/main.c src/avl_tree.h src/bst_tree.h src/queue.h
	$(COMPILE)
obj/avl_tree.o : src/avl_tree.c src/avl_tree.h src/queue.h
	$(COMPILE)
obj/bst_tree.o : src/bst_tree.c src/bst_tree.h
	$(COMPILE)
obj/tests.o : src/tests/tests.c src/tests/minunit.h src/array_list.h src/bst_tree.h
	$(COMPILE)
obj/fifo_queue.o : src/fifo_queue.c src/fifo_queue.h
	$(COMPILE)
obj/lifo_stack.o : src/lifo_stack.c src/lifo_stack.h
	$(COMPILE)
obj/binary_heap.o : src/binary_heap.c src/binary_heap.h
	$(COMPILE)
obj/array_list.o : src/array_list.c src/array_list.h
	$(COMPILE)

clean:
	@echo CLEANING UP
	@rm obj/*.o exe/*.out > /dev/null 2>&1 ||:
