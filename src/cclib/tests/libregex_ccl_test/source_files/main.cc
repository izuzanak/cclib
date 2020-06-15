
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libregex_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "regex",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_regex,
};/*}}}*/

// === test execution functions ================================================

void test_regex()
{/*{{{*/
  ccl::array<char> buffer;

  // - regex create -
  ccl::regex_s regex_0{"^Hello regex$"};

  // - regex match -
  ccl::regmatch_s match_0;
  cassert(regex_0.match("Hello regex",&match_0));

  // - regex array -
  ccl::array<ccl::regex_s> regex_array;

  uint32_t idx = 0;
  do {
    regex_array.push(ccl::regex_s("^Hello regex$"));
  } while(++idx < 10);

  ccl::regex_s regex_1{R"(^\[\(regex_s{.\+},\)\{9,9\}regex_s{.\+}\]$)"};
  cassert(regex_1.match(buffer.debug(regex_array),&match_0));

  // - regex match_n -
  ccl::regex_s regex_2{R"(^\([^ ]\+\) \(regex\)$)"};

  ccl::array<ccl::regmatch_s> matches_0;
  cassert(regex_2.match_n("Hello regex",3,&matches_0));

  cassert(strcmp(buffer.debug(matches_0),"[regmatch_s{0,11},regmatch_s{0,5},regmatch_s{6,11}]") == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

