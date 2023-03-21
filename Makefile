CC=clang++
CFLAGS=-fextended-identifiers -w

all: example converter

example: example.cpp
	$(CC) $(CFLAGS) example.cpp -o example

converter: converter.cpp
	$(CC) $(CFLAGS) converter.cpp -o converter

clean:
	rm -f example converter