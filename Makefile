CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O2
BIN=prog
HEADER=header.h

all: prep $(BIN)

$(BIN): main.o func.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

prep:
	touch catalog.dat
	touch receipt.txt

clean:
	rm -f *.o prog

.PHONY: all prep clean
