CC=gcc
CFLAGS=-I../include -std=c99 -Wall -Wextra

all: main

main: main.c fsm_state_pattern.c
$(CC) $(CFLAGS) $^ -o main

clean:
rm -f main

run: main
./main
