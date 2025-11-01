/**
  ******************************************************************************
  * @file    unity.c
  * @author  Unity Project Contributors
  * @brief   Unity Test Framework Implementation
  ******************************************************************************
  * This is a simplified version of Unity test framework for embedded systems
  * For full version, visit: https://github.com/ThrowTheSwitch/Unity
  ******************************************************************************
  */

#include "unity.h"
#include <stdarg.h>

struct UNITY_STORAGE_T Unity;

/* Simple protection macro for embedded systems */
#define TEST_PROTECT() 1

/* Color definitions for terminal output */
#define UNITY_COLOR_PASS    "\033[32m"  /* Green */
#define UNITY_COLOR_FAIL    "\033[31m"  /* Red */
#define UNITY_COLOR_IGNORE  "\033[33m"  /* Yellow */
#define UNITY_COLOR_RESET   "\033[0m"   /* Reset */

/* Private function declarations */
static void UnityPrint(const char* string);
static void UnityPrintNumber(const long number);
static void UnityPrintFloat(const float number);

/**
  * @brief  Initialize Unity test framework
  * @param  filename: Name of the test file
  * @retval None
  */
void UnityBegin(const char* filename)
{
    Unity.TestFile = filename;
    Unity.CurrentTestName = NULL;
    Unity.CurrentTestLineNumber = 0;
    Unity.NumberOfTests = 0;
    Unity.TestFailures = 0;
    Unity.TestIgnores = 0;
    Unity.CurrentTestFailed = 0;
    
    UNITY_OUTPUT_START();
    UnityPrint(filename);
    UnityPrint(":");
    UNITY_OUTPUT_CHAR('\n');
}

/**
  * @brief  Finalize Unity test framework and return results
  * @retval 0 if all tests passed, number of failures otherwise
  */
int UnityEnd(void)
{
    UNITY_OUTPUT_CHAR('\n');
    UnityPrint("-----------------------\n");
    UnityPrintNumber(Unity.NumberOfTests);
    UnityPrint(" Tests ");
    UnityPrintNumber(Unity.TestFailures);
    UnityPrint(" Failures ");
    UnityPrintNumber(Unity.TestIgnores);
    UnityPrint(" Ignored");
    UNITY_OUTPUT_CHAR('\n');
    
    if (Unity.TestFailures == 0U) {
        UnityPrint(UNITY_COLOR_PASS "OK" UNITY_COLOR_RESET);
    } else {
        UnityPrint(UNITY_COLOR_FAIL "FAIL" UNITY_COLOR_RESET);
    }
    UNITY_OUTPUT_CHAR('\n');
    UNITY_OUTPUT_FLUSH();
    UNITY_OUTPUT_COMPLETE();
    
    return (int)(Unity.TestFailures);
}

/**
  * @brief  Run a single test
  * @param  func: Test function pointer
  * @param  funcName: Test function name
  * @param  funcLineNum: Line number where test is called
  * @retval None
  */
void UnityDefaultTestRun(void (*func)(void), const char* funcName, const int funcLineNum)
{
    Unity.CurrentTestName = funcName;
    Unity.CurrentTestLineNumber = (uint32_t)funcLineNum;
    Unity.NumberOfTests++;
    Unity.CurrentTestFailed = 0;
    
    if (TEST_PROTECT()) {
        setUp();
        func();
    }
    if (TEST_PROTECT()) {
        tearDown();
    }
    
    if (Unity.CurrentTestFailed) {
        Unity.TestFailures++;
    }
    
    Unity.CurrentTestName = NULL;
}

/* Weak implementations of setUp and tearDown */
__attribute__((weak)) void setUp(void) {}
__attribute__((weak)) void tearDown(void) {}

/**
  * @brief  Assert equal numbers
  * @param  expected: Expected value
  * @param  actual: Actual value
  * @param  msg: Custom message
  * @param  lineNumber: Line number
  * @param  expressions: Expression string
  * @retval None
  */
void UnityAssertEqualNumber(const long expected, const long actual, const char* msg, const uint32_t lineNumber, const char* expressions)
{
    if (expected != actual) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((long)lineNumber);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL:");
        if (msg != NULL) {
            UnityPrint(msg);
        }
        UnityPrint(" Expected ");
        UnityPrintNumber(expected);
        UnityPrint(" Was ");
        UnityPrintNumber(actual);
        if (expressions != NULL) {
            UnityPrint(" ");
            UnityPrint(expressions);
        }
        UNITY_OUTPUT_CHAR('\n');
    }
}

/**
  * @brief  Assert equal strings
  * @param  expected: Expected string
  * @param  actual: Actual string
  * @param  msg: Custom message
  * @param  lineNumber: Line number
  * @retval None
  */
void UnityAssertEqualString(const char* expected, const char* actual, const char* msg, const uint32_t lineNumber)
{
    if (expected == NULL && actual == NULL) return;
    if (expected == NULL || actual == NULL || strcmp(expected, actual) != 0) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((long)lineNumber);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL:");
        if (msg != NULL) {
            UnityPrint(msg);
        }
        UnityPrint(" Expected \"");
        UnityPrint(expected ? expected : "NULL");
        UnityPrint("\" Was \"");
        UnityPrint(actual ? actual : "NULL");
        UnityPrint("\"");
        UNITY_OUTPUT_CHAR('\n');
    }
}

/**
  * @brief  Assert floats within tolerance
  * @param  delta: Tolerance
  * @param  expected: Expected value
  * @param  actual: Actual value
  * @param  msg: Custom message
  * @param  lineNumber: Line number
  * @retval None
  */
void UnityAssertFloatsWithin(const float delta, const float expected, const float actual, const char* msg, const uint32_t lineNumber)
{
    float diff = actual - expected;
    if (diff < 0.0f) diff = -diff;
    
    if (diff > delta) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((long)lineNumber);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL:");
        if (msg != NULL) {
            UnityPrint(msg);
        }
        UnityPrint(" Expected ");
        UnityPrintFloat(expected);
        UnityPrint(" Was ");
        UnityPrintFloat(actual);
        UNITY_OUTPUT_CHAR('\n');
    }
}

/**
  * @brief  Assert bits in a value
  * @param  mask: Bit mask
  * @param  expected: Expected bits
  * @param  actual: Actual value
  * @param  msg: Custom message
  * @param  lineNumber: Line number
  * @retval None
  */
void UnityAssertBits(const long mask, const long expected, const long actual, const char* msg, const uint32_t lineNumber)
{
    if ((expected & mask) != (actual & mask)) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((long)lineNumber);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL:");
        if (msg != NULL) {
            UnityPrint(msg);
        }
        UnityPrint(" Expected 0x");
        UnityPrintNumber(expected & mask);
        UnityPrint(" Was 0x");
        UnityPrintNumber(actual & mask);
        UNITY_OUTPUT_CHAR('\n');
    }
}

/**
  * @brief  Fail a test with message
  * @param  msg: Failure message
  * @param  lineNumber: Line number
  * @retval None
  */
void UnityFail(const char* msg, const uint32_t lineNumber)
{
    Unity.CurrentTestFailed = 1;
    UnityPrint(Unity.TestFile);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber((long)lineNumber);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UnityPrint(":FAIL:");
    if (msg != NULL) {
        UnityPrint(msg);
    }
    UNITY_OUTPUT_CHAR('\n');
}

/**
  * @brief  Ignore a test with message
  * @param  msg: Ignore message
  * @param  lineNumber: Line number
  * @retval None
  */
void UnityIgnore(const char* msg, const uint32_t lineNumber)
{
    Unity.TestIgnores++;
    UnityPrint(Unity.TestFile);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber((long)lineNumber);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UnityPrint(":IGNORE:");
    if (msg != NULL) {
        UnityPrint(msg);
    }
    UNITY_OUTPUT_CHAR('\n');
}

/* Private helper functions */

/**
  * @brief  Print a string
  * @param  string: String to print
  * @retval None
  */
static void UnityPrint(const char* string)
{
    const char* pch = string;
    if (pch != NULL) {
        while (*pch) {
            UNITY_OUTPUT_CHAR(*pch);
            pch++;
        }
    }
}

/**
  * @brief  Print a number
  * @param  number: Number to print
  * @retval None
  */
static void UnityPrintNumber(const long number)
{
    char buffer[32];
    sprintf(buffer, "%ld", number);
    UnityPrint(buffer);
}

/**
  * @brief  Print a float number
  * @param  number: Float to print
  * @retval None
  */
static void UnityPrintFloat(const float number)
{
    char buffer[32];
    sprintf(buffer, "%.6f", number);
    UnityPrint(buffer);
}