CC=gcc
UNITY_DIR=../../../tests/unity
STUB_DIR=../../../tests/stubs
CFLAGS=-I$(UNITY_DIR) -I$(STUB_DIR) -I../include -std=c99 -Wall -Wextra -g -DUNIT_TEST

TESTS=test_switch_case test_function_pointer test_transition_table test_state_pattern
OBJS=$(addsuffix .o,$(TESTS)) $(UNITY_DIR)/unity.o \
    $(STUB_DIR)/freertos_stubs.o $(STUB_DIR)/libopencm3_stubs.o
EXES=$(addsuffix _runner,$(TESTS))

all: $(EXES)

%_runner: %.o $(UNITY_DIR)/unity.o $(STUB_DIR)/freertos_stubs.o $(STUB_DIR)/libopencm3_stubs.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXES) $(OBJS)

run: $(EXES)
	@for t in $(EXES); do ./$$t; done
