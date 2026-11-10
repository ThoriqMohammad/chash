#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT(cond) do { \
    tests_run++; \
    if (!(cond)) { \
        fprintf(stderr, "  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_EQ_INT(a, b) do { \
    tests_run++; \
    long _a = (long)(a), _b = (long)(b); \
    if (_a != _b) { \
        fprintf(stderr, "  FAIL %s:%d: %s (%ld) != %s (%ld)\n", \
                __FILE__, __LINE__, #a, _a, #b, _b); \
        tests_failed++; \
    } \
} while (0)

#define TEST_SUMMARY() do { \
    printf("%d/%d assertions passed\n", tests_run - tests_failed, tests_run); \
    return tests_failed ? 1 : 0; \
} while (0)

#endif /* TEST_H */
