CC=cc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O2
BIN=prog
HEADER=header.h

all: prep $(BIN)

$(BIN): main.o func.o
	$(CC) -o $@ main.o func.o $(CFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

prep:
	touch catalog.dat
	touch economy.dat
	touch receipt.dat
	touch history.dat

clean:
	rm -f *.o prog

.PHONY: all prep clean
