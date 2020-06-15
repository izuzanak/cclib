
#ifndef TESTS_PERFORMANCE_TEST_HEADER_FILES_MAIN_H
#define TESTS_PERFORMANCE_TEST_HEADER_FILES_MAIN_H

#include "ccl_json.h"
#include "ccl_sys.h"
#include "ccl_test.h"
#include "ccl_time.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ccl {

template <typename VALUE>
auto operator<(const array<VALUE> &a_first,const array<VALUE> &a_second) -> bool
{/*{{{*/
  return a_first.compare(a_second) < 0;
}/*}}}*/

} // namespace ccl

// === test execution functions ================================================

constexpr int test_cnt = 4;

void test_array();
void test_tree();
void test_var();
void test_json();

#endif

