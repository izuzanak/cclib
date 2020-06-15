
#include "ccl_base.h"

namespace ccl {

template <>
auto array<char>::append_format(const char *a_format,va_list a_ap) noexcept -> array<char> &
{/*{{{*/
  int reserved = m_size - m_used;

  va_list ap;
  va_copy(ap,a_ap);
  int length = vsnprintf(m_data + m_used,reserved,a_format,ap);
  va_end(ap);

  if (length < reserved)
  {
    m_used += length;
  }
  else
  {
    auto old_used = m_used;

    decltype(old_used) size = length + 1;
    push_blanks(size);

    vsnprintf(m_data + old_used,size,a_format,a_ap);

    // - remove terminating character -
    --m_used;
  }

  return *this;
}/*}}}*/

} // namespace ccl

auto to_json(const std::string &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  a_trg->push('"');

  if (!a_value.empty())
  {
    const char *ptr = a_value.data();
    const char *ptr_end = ptr + a_value.length();
    do {

      switch (*ptr)
      {
        case '\b':
          (*a_trg)
            .push('\\')
            .push('b');
          break;
        case '\f':
          (*a_trg)
            .push('\\')
            .push('f');
          break;
        case '\n':
          (*a_trg)
            .push('\\')
            .push('n');
          break;
        case '\r':
          (*a_trg)
            .push('\\')
            .push('r');
          break;
        case '\t':
          (*a_trg)
            .push('\\')
            .push('t');
          break;
        case '\\':
          (*a_trg)
            .push('\\')
            .push('\\');
          break;
        case '"':
          (*a_trg)
            .push('\\')
            .push('"');
          break;
        default:
          a_trg->push(*ptr);
      }

    } while(++ptr < ptr_end);
  }

  return a_trg->push('"');
}/*}}}*/

auto string_format(const char *a_format,...) noexcept -> std::string // NOLINT(cert-dcl50-cpp)
{/*{{{*/
  constexpr int init_size = 128;

  std::string result;
  result.resize(init_size);

  va_list ap;
  va_start(ap,a_format);
  int length = vsnprintf(const_cast<char *>(result.data()),init_size,a_format,ap);
  va_end(ap);

  result.resize(length);

  if (length >= init_size)
  {
    va_start(ap,a_format);
    vsnprintf(const_cast<char *>(result.data()),length + 1,a_format,ap);
    va_end(ap);
  }

  return result;
}/*}}}*/

