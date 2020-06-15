
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "liblogger_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "logger",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_logger,
};/*}}}*/

// === test execution functions ================================================

void test_logger()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::logger_s logger("test");

  logger.add_file(1,"tests/liblogger_ccl_test/logger/test.log",4096,3);
  logger.add_file(2,"tests/liblogger_ccl_test/logger/test1.log",4096,3);
  logger.add_file(3,"tests/liblogger_ccl_test/logger/test2.log",4096,3);

  unsigned idx = 0;
  do {
    logger.write(2,"Hello logger #%u ...",idx);
  } while(++idx < 100);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

