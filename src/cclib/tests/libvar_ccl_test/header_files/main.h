
#ifndef TESTS_LIBVAR_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBVAR_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_test.h"
#include "ccl_var.h"

// === test execution functions ================================================

constexpr int test_cnt = 8;

void test_var_blank();
void test_var_bool();
void test_var_int();
void test_var_float();
void test_var_string();
void test_var_array();
void test_var_dict();
void test_var_constr();

#endif

