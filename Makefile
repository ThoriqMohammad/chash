.RECIPEPREFIX = >
CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Werror -O2 -g \
           -fsanitize=address,undefined \
           -Iinclude
LDFLAGS := -fsanitize=address,undefined

SRC     := $(wildcard src/*.c)
TESTS   := $(wildcard tests/test_*.c)
TEST_BINS := $(patsubst tests/%.c,build/%,$(TESTS))

.PHONY: all test clean

all: build/chash_demo

build/chash_demo: src/chash.c build/demo.o
> @mkdir -p build
> $(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build/demo.o: src/chash.c include/chash.h
> @mkdir -p build
> $(CC) $(CFLAGS) -c $< -o $@

build/%: tests/%.c src/chash.c include/chash.h tests/test.h
> @mkdir -p build
> $(CC) $(CFLAGS) $< src/chash.c -o $@ $(LDFLAGS)

test: $(TEST_BINS)
> @set -e; for t in $(TEST_BINS); do echo "== $$t"; ./$$t; done

clean:
> rm -rf build
