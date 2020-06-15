
#include "ccl_regex.h"

namespace ccl {

// === methods of structure regex_s ============================================

auto regex_s::match_n(const char *a_data,size_t a_match_cnt,ccl::array<regmatch_s> *a_matches) noexcept -> bool
{/*{{{*/
  debug_assert(m_regp != nullptr);

  a_matches->used() = 0;

  if (a_match_cnt > 0)
  {
    a_matches->reserve(a_match_cnt);

    // - execute regular expression -
    if (regexec(m_regp,a_data,a_match_cnt,a_matches->data(),0) != 0)
    {
      return false;
    }

    regmatch_s *m_ptr = a_matches->data();
    regmatch_s *m_ptr_end = m_ptr + a_match_cnt;
    do {

      // - first no match -
      if (m_ptr->rm_so == -1)
      {
        break;
      }
    } while(++m_ptr < m_ptr_end);

    a_matches->used() = m_ptr - a_matches->data();
  }

  return true;
}/*}}}*/

} // namespace ccl

