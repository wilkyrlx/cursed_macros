CC=clang++
CFLAGS=-fextended-identifiers -w

all: example

example: example.cpp
	$(CC) $(CFLAGS) example.cpp -o example

clean:
	rm -f example