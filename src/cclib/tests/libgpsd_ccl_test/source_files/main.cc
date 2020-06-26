
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libgpsd_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "gpsd",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_gpsd,
};/*}}}*/

// === test execution functions ================================================

void test_gpsd()
{/*{{{*/

  // TODO(devel): continue ...

}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

