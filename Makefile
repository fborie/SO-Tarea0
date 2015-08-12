CC = gcc
CFLAGS = -Wall -Werror -std=c11
EXE = minisort

all: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE)

.PHONY: clean

clean:
	rm -f $(EXE) 
