CC = gcc
LD = ld
CFLAGS = -Wall -Wextra -Wno-unused-parameter
CFLAGS += -Iinclude
CFLAGS += -MMD -MP
CFLAGS += $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2) $(shell pkg-config --libs SDL2_image) -ldl -lm

override SRC = $(shell find . -name '*.c')
override OBJ = $(addprefix build/,$(SRC:.c=.c.o))
override DEP = $(addprefix build/,$(SRC:.c=.c.d))

.PHONY: all
all: vo

-include $(DEP)

.PHONY: vo
vo: $(OBJ)
	mkdir -p build
	$(CC) $(OBJ) -o build/$@ $(LDFLAGS)

build/%.c.o: %.c
	mkdir -p "$$(dirname $@)"
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: run
run: vo
	./build/vo

.PHONY: clean
clean:
	rm -rf build


