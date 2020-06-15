
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libtime_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "time",
  "clock",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_time,
  test_clock,
};/*}}}*/

// === test execution functions ================================================

void test_time()
{/*{{{*/
  ccl::array<char> buffer;

  puts(buffer.debug(ccl::time_s::now()));

  ccl::time_s time{"20000101103025"};
  buffer.debug(time);
  cassert(strcmp(buffer.data(),"2000/01/01 10:30:25") == 0);
}/*}}}*/

void test_clock()
{/*{{{*/
  printf("resolution: %llu\n",ccl::time_s::clock_getres(CLOCK_MONOTONIC).nano());
  printf("time: %llu\n",ccl::time_s::clock_gettime(CLOCK_MONOTONIC).nano());
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

