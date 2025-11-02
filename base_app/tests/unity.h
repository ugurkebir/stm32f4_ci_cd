/**
  ******************************************************************************
  * @file    unity.h
  * @author  Unity Project Contributors
  * @brief   Unity Test Framework Header
  ******************************************************************************
  * This is a simplified version of Unity test framework for embedded systems
  * For full version, visit: https://github.com/ThrowTheSwitch/Unity
  ******************************************************************************
  */

#ifndef UNITY_H
#define UNITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Unity Configuration */
#define UNITY_OUTPUT_CHAR(a)    printf("%c", a)
#define UNITY_OUTPUT_FLUSH()    fflush(stdout)
#define UNITY_OUTPUT_START()    /* Start output */
#define UNITY_OUTPUT_COMPLETE() /* Complete output */

/* Test Results */
typedef enum {
    UNITY_PASS = 0,
    UNITY_FAIL,
    UNITY_IGNORE
} UNITY_TEST_RESULT_T;

/* Unity Internal Structure */
struct UNITY_STORAGE_T {
    uint32_t NumberOfTests;
    uint32_t TestFailures;
    uint32_t TestIgnores;
    uint32_t CurrentTestFailed;
    const char* TestFile;
    const char* CurrentTestName;
    uint32_t CurrentTestLineNumber;
};

extern struct UNITY_STORAGE_T Unity;

/* Macros for Test Organization */
#define RUN_TEST(func) UnityDefaultTestRun(func, #func, __LINE__)

/* Setup and Teardown */
void setUp(void);
void tearDown(void);

/* Unity Core Functions */
void UnityBegin(const char* filename);
int UnityEnd(void);
void UnityDefaultTestRun(void (*func)(void), const char* funcName, const int funcLineNum);

/* Unity Control Macros */
#define UNITY_BEGIN()       UnityBegin(__FILE__)
#define UNITY_END()         UnityEnd()

/* Assertion Functions */
void UnityAssertEqualNumber(const long expected, const long actual, const char* msg, const uint32_t lineNumber, const char* expressions);
void UnityAssertEqualString(const char* expected, const char* actual, const char* msg, const uint32_t lineNumber);
void UnityAssertFloatsWithin(const float delta, const float expected, const float actual, const char* msg, const uint32_t lineNumber);
void UnityAssertBits(const long mask, const long expected, const long actual, const char* msg, const uint32_t lineNumber);
void UnityFail(const char* msg, const uint32_t lineNumber);
void UnityIgnore(const char* msg, const uint32_t lineNumber);

/* Test Assertion Macros */
#define TEST_ASSERT(condition)                                                  UnityAssertEqualNumber((condition) ? 1 : 0, 1, " Expression Evaluated To FALSE", __LINE__, #condition)
#define TEST_ASSERT_TRUE(condition)                                             UnityAssertEqualNumber((condition) ? 1 : 0, 1, " Expected TRUE Was FALSE", __LINE__, #condition)
#define TEST_ASSERT_FALSE(condition)                                            UnityAssertEqualNumber((condition) ? 1 : 0, 0, " Expected FALSE Was TRUE", __LINE__, #condition)
#define TEST_ASSERT_NULL(pointer)                                               UnityAssertEqualNumber((long)pointer, 0, " Expected NULL", __LINE__, #pointer)
#define TEST_ASSERT_NOT_NULL(pointer)                                           UnityAssertEqualNumber(((pointer) == NULL) ? 0 : 1, 1, " Expected Non-NULL", __LINE__, #pointer)

/* Integer Assertions */
#define TEST_ASSERT_EQUAL(expected, actual)                                     UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_INT(expected, actual)                                 UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_INT8(expected, actual)                                UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_INT16(expected, actual)                               UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_INT32(expected, actual)                               UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_UINT(expected, actual)                                UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_UINT8(expected, actual)                               UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_UINT16(expected, actual)                              UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_UINT32(expected, actual)                              UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")
#define TEST_ASSERT_EQUAL_HEX32(expected, actual)                               UnityAssertEqualNumber((long)(expected), (long)(actual), NULL, __LINE__, " Expected <" #expected "> Actual <" #actual ">")

/* String Assertions */
#define TEST_ASSERT_EQUAL_STRING(expected, actual)                              UnityAssertEqualString((const char*)(expected), (const char*)(actual), NULL, __LINE__)
#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)                         UnityAssertEqualString((const char*)(expected), (const char*)(actual), NULL, __LINE__)

/* Float Assertions */
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)                       UnityAssertFloatsWithin((float)(delta), (float)(expected), (float)(actual), NULL, __LINE__)
#define TEST_ASSERT_EQUAL_FLOAT(expected, actual)                               UnityAssertFloatsWithin(0.00001f, (float)(expected), (float)(actual), NULL, __LINE__)

/* Bit Assertions */
#define TEST_ASSERT_BITS(mask, expected, actual)                                UnityAssertBits((long)(mask), (long)(expected), (long)(actual), NULL, __LINE__)
#define TEST_ASSERT_BITS_HIGH(mask, actual)                                     UnityAssertBits((long)(mask), (long)(mask), (long)(actual), NULL, __LINE__)
#define TEST_ASSERT_BITS_LOW(mask, actual)                                      UnityAssertBits((long)(mask), (long)(0), (long)(actual), NULL, __LINE__)
#define TEST_ASSERT_BIT_HIGH(bit, actual)                                       UnityAssertBits((long)(1 << (bit)), (long)(1 << (bit)), (long)(actual), NULL, __LINE__)
#define TEST_ASSERT_BIT_LOW(bit, actual)                                        UnityAssertBits((long)(1 << (bit)), (long)(0), (long)(actual), NULL, __LINE__)

/* Range Assertions */
#define TEST_ASSERT_INT_WITHIN(delta, expected, actual)                         UnityAssertEqualNumber(((actual) >= ((expected) - (delta)) && (actual) <= ((expected) + (delta))) ? 1 : 0, 1, NULL, __LINE__, " Expected <" #actual "> to be within <" #delta "> of <" #expected ">")

/* Failure and Ignore */
#define TEST_FAIL_MESSAGE(message)                                              UnityFail((message), __LINE__)
#define TEST_FAIL()                                                             UnityFail(NULL, __LINE__)
#define TEST_IGNORE_MESSAGE(message)                                            UnityIgnore((message), __LINE__)
#define TEST_IGNORE()                                                           UnityIgnore(NULL, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* UNITY_H */