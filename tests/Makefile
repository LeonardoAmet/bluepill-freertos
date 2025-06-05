CC=gcc
CFLAGS=-I../tests/stubs -I../tests/unity -DUNIT_TEST -std=c99

SRCS=$(wildcard test_*.c)
OBJS=$(SRCS:.c=.o) stubs/freertos_stubs.o stubs/libopencm3_stubs.o unity/unity.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o test_runner $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) test_runner

run: all
	./test_runner
