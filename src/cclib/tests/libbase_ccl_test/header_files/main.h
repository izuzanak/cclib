
#ifndef TESTS_LIBBASE_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBBASE_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_base.h"
#include "ccl_test.h"

// === structure definitions ===================================================

struct static_s
{/*{{{*/
  public:
  uint32_t m_first;  // NOLINT(misc-non-private-member-variables-in-classes) 
  uint16_t m_second; // NOLINT(misc-non-private-member-variables-in-classes)
  uint32_t m_third;  // NOLINT(misc-non-private-member-variables-in-classes)

  auto operator==(const static_s &a_second) const noexcept -> bool
  {/*{{{*/
    return
      m_first == a_second.m_first &&
      m_second == a_second.m_second &&
      m_third == a_second.m_third;
  }/*}}}*/

  [[nodiscard]] auto compare(const static_s &a_second) const noexcept -> int
  {/*{{{*/
    return m_first < a_second.m_first ? -1 : m_first > a_second.m_first ? 1 : 0;
  }/*}}}*/

  auto to_string(ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append_format("{first:%u,second:%hu,third:%u}",m_first,m_second,m_third);
  }/*}}}*/

  auto to_json(ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append_format(R"'({"first":%u,"second":%hu,"third":%u})'",m_first,m_second,m_third);
  }/*}}}*/

  auto to_json_nice(ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return (*a_trg)
      .push('{')
      .json(a_json_nice->push())
      .append_format("\"first\":%u,",m_first)
      .json(*a_json_nice)
      .append_format("\"second\":%hu,",m_second)
      .json(*a_json_nice)
      .append_format("\"third\":%u",m_third)
      .json(a_json_nice->pop())
      .push('}');
  }/*}}}*/
};/*}}}*/

// === test execution functions ================================================

constexpr int test_cnt = 9;

void test_string();
void test_basic_type_array();
void test_static_type_array();
void test_dynamic_type_array();
void test_basic_type_list();
void test_basic_type_queue();
void test_basic_type_tree();
void test_static_type_tree();
void test_dynamic_type_tree();

#endif

