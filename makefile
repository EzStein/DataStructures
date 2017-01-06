OBJECTS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
FLAGS = -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
-Wstrict-prototypes -Wold-style-definition

exe/a.out: $(OBJECTS)
	@echo linking $^
	-@mkdir exe > /dev/null 2>&1
	@gcc $(FLAGS) $^ -o $@
	@echo build succesful
obj/main.o : src/main.c
	@echo compiling $^
	-@mkdir obj > /dev/null 2>&1
	@gcc $(FLAGS) -c $^ -o $@
clean:
	-rm obj/*.o exe/*.out
