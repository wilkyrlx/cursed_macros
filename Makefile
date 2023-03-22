CC=clang++
CFLAGS=-fextended-identifiers -w

all: example converter output

example: simple.c
	$(CC) $(CFLAGS) simple.c -o example

output: output.cpp
	$(CC) $(CFLAGS) output.cpp -o output

converter: converter.cpp
	$(CC) $(CFLAGS) converter.cpp -o converter

clean:
	rm -f example converter output