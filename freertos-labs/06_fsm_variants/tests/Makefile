CC=gcc
UNITY_DIR=../../../tests/unity
CFLAGS=-I$(UNITY_DIR) -I../include -std=c99 -Wall -Wextra

TESTS=test_switch_case test_func_pointer test_transition_table test_state_pattern
OBJS=$(addsuffix .o,$(TESTS)) $(UNITY_DIR)/unity.o
EXES=$(addsuffix _runner,$(TESTS))

all: $(EXES)

%_runner: %.o $(UNITY_DIR)/unity.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXES) $(OBJS)

run: $(EXES)
	@for t in $(EXES); do ./$$t; done
