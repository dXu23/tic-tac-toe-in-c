.POSIX:

CC=cc

all: tictactoe

tictactoe: tictactoe.c
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f tictactoe
