
#ifndef TESTS_LIBVALIDATOR_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBVALIDATOR_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_test.h"
#include "ccl_validator.h"

// === test execution functions ================================================

constexpr int test_cnt = 19;

void test_prop_type();
void test_prop_equal();
void test_prop_not_equal();
void test_prop_lesser();
void test_prop_greater();
void test_prop_lesser_equal();
void test_prop_greater_equal();
void test_prop_length_equal();
void test_prop_length_not_equal();
void test_prop_length_lesser();
void test_prop_length_greater();
void test_prop_length_lesser_equal();
void test_prop_length_greater_equal();
void test_prop_reference();
void test_prop_regex();
void test_prop_items();
void test_prop_opt_items();
void test_prop_all_items();
void test_prop_all_keys();

#endif

