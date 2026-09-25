CC = gcc
CFLAGS = -Iinclude



MAIN_SRC = $(wildcard main/*.c)
SRC = $(wildcard src/*.c)

MAIN_OBJ = $(patsubst main/%.c,build/o/%.o,$(MAIN_SRC))
SRC_OBJ = $(patsubst src/%.c,build/o/%.o,$(SRC))

PROGRAMS = $(patsubst main/%.c,build/out/%.out,$(MAIN_SRC))


OS := $(shell uname -s)
ifneq ($(OS),Linux)
    $(error Sorry, I prefer Linux)
endif

all: $(PROGRAMS)

test: $(PROGRAMS)
	@./test.sh 10 20 

build/out/%.out: build/o/%.o $(SRC_OBJ)
	$(CC) $^ -o $@

build/o/%.o: main/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/o/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build
	mkdir -p build/out
	mkdir -p build/o

clean:
	rm -rf build
