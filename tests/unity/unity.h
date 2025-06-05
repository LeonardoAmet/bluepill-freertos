#ifndef UNITY_H
#define UNITY_H
#include <stdio.h>

void UnityBegin(void);
int UnityEnd(void);
void UnityAssertEqualNumber(int expected, int actual, const char* msg, int line);
void UnityFail(int line, const char* msg);
void UnityDefaultTestRun(void (*func)(void), const char* name, int line);

#define UNITY_BEGIN() UnityBegin()
#define UNITY_END() UnityEnd()
#define RUN_TEST(func) UnityDefaultTestRun(func, #func, __LINE__)
#define TEST_ASSERT_EQUAL_INT(expected, actual) UnityAssertEqualNumber((expected), (actual), "TEST_ASSERT_EQUAL_INT", __LINE__)
#define TEST_ASSERT_EQUAL_CHAR(expected, actual) UnityAssertEqualNumber((expected), (actual), "TEST_ASSERT_EQUAL_CHAR", __LINE__)
#define TEST_FAIL_MESSAGE(msg) UnityFail(__LINE__, msg)

#endif
