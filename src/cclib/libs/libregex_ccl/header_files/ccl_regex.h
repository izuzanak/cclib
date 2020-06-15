
#ifndef LIBS_LIBREGEX_CCL_HEADER_FILES_CCL_REGEX_H
#define LIBS_LIBREGEX_CCL_HEADER_FILES_CCL_REGEX_H

#include "ccl_base.h"

#include <regex.h>

namespace ccl {

constexpr int REGEX_COMPILE_ERROR = 1;

struct regex_s;

// === definition of structure regmatch_s ======================================

using regmatch_s = ::regmatch_t;

inline auto to_string(const regmatch_s &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("regmatch_s{%d,%d}",a_value.rm_so,a_value.rm_eo);
}/*}}}*/

// === definition of structure regex_s =========================================

struct regex_s
{/*{{{*/
private:
  regex_t *m_regp = nullptr;

public:
  ~regex_s()
  {/*{{{*/
    if (m_regp != nullptr)
    {
      regfree(m_regp);
      cfree(m_regp);
    }
  }/*}}}*/

  regex_s() = default;
  regex_s(const regex_s &a_src) = delete;

  regex_s(regex_s &&a_src) noexcept
  {/*{{{*/
    m_regp = a_src.m_regp;
    a_src.m_regp = nullptr;
  }/*}}}*/

  explicit regex_s(const std::string &a_data)
  {/*{{{*/
    m_regp = static_cast<regex_t *>(cmalloc(sizeof(regex_t)));

    if (regcomp(m_regp,a_data.data(),0) != 0)
    {
      cfree(m_regp);

      throw_error(REGEX_COMPILE_ERROR);
    }
  }/*}}}*/

  auto operator=(const regex_s &a_src) -> regex_s & = delete;
  auto operator=(regex_s &&a_src) noexcept -> regex_s &
  {/*{{{*/
    this->~regex_s();

    m_regp = a_src.m_regp;
    a_src.m_regp = nullptr;

    return *this;
  }/*}}}*/

  auto to_string(ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append_format("regex_s{%p}",m_regp);
  }/*}}}*/

  auto match(const char *a_data,regmatch_s *a_match) noexcept -> bool
  {/*{{{*/
    debug_assert(m_regp != nullptr);

    return regexec(m_regp,a_data,1,a_match,0) == 0;
  }/*}}}*/

  auto match_n(const char *a_data,size_t a_match_cnt,ccl::array<regmatch_s> *a_matches) noexcept -> bool;
};/*}}}*/

} // namespace ccl

#endif

