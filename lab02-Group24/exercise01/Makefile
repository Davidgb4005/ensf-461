CC = gcc

.PHONY: all clean test

TARGETS = gen_numbers.out select_arg.out gen_in_range.out

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)

all: $(TARGETS)

else

all:
	@echo "Sorry, I prefer Linux"

endif

gen_numbers.out select_arg.out: %.out: %.c utils.c utils.h
	$(CC) -o $@ $(filter %.c,$^)

gen_in_range.out: %.out: %.c
	$(CC) -o $@ $<

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	./gen_numbers.out 5
	./gen_in_range.out 1 10
	./select_arg.out a b c
	@echo "All tests passed"