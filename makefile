define COMPILE
@echo COMPILING: $^
	@mkdir obj > /dev/null 2>&1 ||:
	@gcc $(FLAGS) -c $(filter src/%.c, $^) -o $@
endef

OBJECTS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
FLAGS = -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
-Wstrict-prototypes -Wold-style-definition

exe/a.out: $(OBJECTS)
	@echo LINKING: $^
	@mkdir exe > /dev/null 2>&1 ||:
	@gcc $(FLAGS) $^ -o $@
	@echo BUILD SUCCESFUL

obj/main.o : src/main.c src/avl_tree.h
	$(COMPILE)
obj/avl_tree.o : src/avl_tree.c src/avl_tree.h
	$(COMPILE)

clean:
	@echo CLEANING UP
	@rm obj/*.o exe/*.out > /dev/null 2>&1 ||:
