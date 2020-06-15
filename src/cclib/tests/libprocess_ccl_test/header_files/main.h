
#ifndef TESTS_LIBPROCESS_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBPROCESS_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_process.h"
#include "ccl_test.h"

#include <atomic>

// === test execution functions ================================================

constexpr int test_cnt = 2;

void test_process();
void test_signal();

#endif

