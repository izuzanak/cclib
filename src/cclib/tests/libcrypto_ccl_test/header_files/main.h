
#ifndef TESTS_LIBCRYPTO_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBCRYPTO_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_crypto.h"
#include "ccl_test.h"

// === test execution functions ================================================

constexpr int test_cnt = 3;

void test_random();
void test_base16();
void test_base64();

#endif

