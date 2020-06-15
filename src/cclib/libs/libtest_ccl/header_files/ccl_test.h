
#ifndef LIBS_LIBTEST_CCL_HEADER_FILES_CCL_TEST_H
#define LIBS_LIBTEST_CCL_HEADER_FILES_CCL_TEST_H

#include "ccl_base.h"

namespace ccl {

// - test function type -
using test_function_t = void (*)();

// === definition of global functions ==========================================

void execute_tests(char **a_argv,const char *a_test_name,
    const char **a_test_names,test_function_t *a_test_functions,
    uint32_t a_test_cnt);

} // namespace ccl

#endif

