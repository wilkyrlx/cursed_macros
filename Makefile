.PHONY: all clean 

# all: clean format reverse_test
all: clean example

clean:
	rm -f example example.o

CFLAGS = -Wall -Werror -O3
%: %.c
	gcc $(CFLAGS) -c $< -o $@

example: example.o
	gcc $(CFLAGS) $^ -o $@