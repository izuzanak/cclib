
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libtcp_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "tcp",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_tcp,
};/*}}}*/

// === test execution functions ================================================

void test_tcp()
{/*{{{*/

  // FIXME TODO continue ...
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

