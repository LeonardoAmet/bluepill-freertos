#include "unity.h"
static int tests_run;
static int tests_failed;

void UnityBegin(void) {
    tests_run = 0;
    tests_failed = 0;
}

void UnityAssertEqualNumber(int expected, int actual, const char* msg, int line) {
    tests_run++;
    if (expected != actual) {
        tests_failed++;
        printf("%s:%d: %s Expected %d Was %d\n", __FILE__, line, msg, expected, actual);
    }
}

void UnityFail(int line, const char* msg) {
    tests_run++;
    tests_failed++;
    printf("%s:%d: %s\n", __FILE__, line, msg);
}

void UnityDefaultTestRun(void (*func)(void), const char* name, int line) {
    (void)name; (void)line;
    func();
}

int UnityEnd(void) {
    printf("Ran %d tests: %d failures\n", tests_run, tests_failed);
    return tests_failed;
}
