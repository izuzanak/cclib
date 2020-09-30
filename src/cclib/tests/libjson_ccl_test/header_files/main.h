
#ifndef TESTS_LIBJSON_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBJSON_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_json.h"
#include "ccl_sys.h"
#include "ccl_test.h"

// === test execution functions ================================================

constexpr int test_cnt = 4;

void test_json();
void test_string();
void test_comments();
void test_load();

#endif

