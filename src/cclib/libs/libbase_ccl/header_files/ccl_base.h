
#ifndef LIBS_LIBBASE_CCL_HEADER_FILES_CCL_BASE_H
#define LIBS_LIBBASE_CCL_HEADER_FILES_CCL_BASE_H

#define ENABLED 1
#define MP_COMMA ,

// - system type selection -
#define SYSTEM_TYPE_UNIX    1 // - for system calls use unix environment
#define SYSTEM_TYPE_WINDOWS 2 // - for system calls use windows environment
#define SYSTEM_TYPE_DSP     3 // - target platform is DSP processor
// --

// - atomic subsystem selection -
#define ATOMIC_TYPE_GCC     1 // - for atomic structure implementation use GCC compiler functions
#define ATOMIC_TYPE_WINDOWS 2 // - for atomic structure implementation use win32 library
// --

// - basic system configuration -
#ifdef LINUX
#define SYSTEM_TYPE SYSTEM_TYPE_UNIX
#define ATOMIC_TYPE ATOMIC_TYPE_GCC
#define WUR __attribute__((warn_unused_result))
#endif

#ifdef WINDOWS
#define WUR _Check_return_
#define SYSTEM_TYPE SYSTEM_TYPE_WINDOWS
#define ATOMIC_TYPE ATOMIC_TYPE_WINDOWS
#endif

// - system includes -
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <sys/time.h>
#include <unistd.h>
#endif

#if SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#include <winsock2.h>
#include <windows.h>
#include <signal.h>
#include <direct.h>
#endif

#define uint64_t long long unsigned
#define int64_t long long int

#define cmalloc malloc
#define crealloc realloc
#define cfree free

// - basic configuration -
#ifndef NDEBUG
#define OPTION_DEBUG_ASSERT ENABLED
#define OPTION_DEBUG_THROW ENABLED
#define OPTION_DEBUG_CHECK_FDS ENABLED
#else
#endif
//#define OPTION_PERFORMANCE_TEST ENABLED

// - error macros -
#if OPTION_DEBUG_THROW == ENABLED
#define throw_error(ERROR_ID)\
do {\
  fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
  throw int(ERROR_ID);\
} while(false)

#define return_error(ERROR_ID)\
do {\
  fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
  return ERROR_ID;\
} while(false)
#else
#define throw_error(ERROR_ID) throw int(ERROR_ID)
#define return_error(ERROR_ID) return ERROR_ID
#endif

// - assert macros -
#define cassert(A_COND)\
do {\
  if (!(A_COND)) {\
    fprintf(stderr,"CASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
} while(false)

#if OPTION_DEBUG_ASSERT == ENABLED
#define debug_assert(A_COND)\
do {\
  if (!(A_COND)) {\
    fprintf(stderr,"DEBUG_ASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
} while(false)
#else
#define debug_assert(A_COND)
#endif

#if OPTION_BRUTAL_ASSERT == ENABLED
#define brutal_assert(A_COND)\
do {\
  if (!(A_COND)) {\
    fprintf(stderr,"BRUTAL_ASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
} while(false)
#else
#define brutal_assert(A_COND)
#endif

#if OPTION_DEBUG_CHECK_FDS == ENABLED
inline auto debug_check_fds() -> void
{/*{{{*/
  constexpr int command_size = 256;
  std::array<char,command_size> command;

  cassert(snprintf(command.data(),command_size,
        "ls -l /proc/%d/fd 1>&2",static_cast<int>(getpid())) < command_size);
  cassert(system(command.data()) == 0); // NOLINT(cert-env33-c)
}/*}}}*/
#else
inline auto debug_check_fds() -> void {}
#endif

constexpr uint32_t c_array_add = 4;
constexpr uint32_t c_idx_not_exist = UINT_MAX;

template<class TYPE>
auto compare(const TYPE &a_first,const TYPE &a_second) noexcept -> typename std::enable_if<std::is_compound<TYPE>::value,int>::type
{/*{{{*/
  return a_first.compare(a_second);
}/*}}}*/

template<class TYPE>
auto compare(const TYPE &a_first,const TYPE &a_second) noexcept -> typename std::enable_if<!std::is_compound<TYPE>::value,int>::type
{/*{{{*/
  return a_first < a_second ? -1 : a_first > a_second ? 1 : 0;
}/*}}}*/

namespace ccl {

struct json_nice_s;

} // namespace ccl

#include "ccl_array.h"

// - function to_string -
inline auto to_string(char a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  if (isprint(a_value) != 0)
  {
    a_trg->push(a_value);
  }

  a_trg->append_format("0x%hhx",a_value);

  return *a_trg;
}/*}}}*/

inline auto to_string(const unsigned char &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%hhu",a_value);
}/*}}}*/

inline auto to_string(const int16_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%hd",a_value);
}/*}}}*/

inline auto to_string(const uint16_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%hu",a_value);
}/*}}}*/

inline auto to_string(const int32_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%d",a_value);
}/*}}}*/

inline auto to_string(const uint32_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%u",a_value);
}/*}}}*/

inline auto to_string(const int64_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%lld",a_value);
}/*}}}*/

inline auto to_string(const uint64_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%llu",a_value);
}/*}}}*/

inline auto to_string(const float &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_string(const double &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_string(const bool &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  if (a_value) {
    a_trg->append(6,"<true>");
  }
  else {
    a_trg->append(7,"<false>");
  }

  return *a_trg;
}/*}}}*/

inline auto to_string(void *a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%p",a_value);
}/*}}}*/

template <typename TYPE>
auto to_string(const TYPE &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_value.to_string(a_trg);
}/*}}}*/

inline auto to_string(const std::string &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append(a_value.length(),a_value.data());
}/*}}}*/

template <typename VALUE>
auto to_string(const ccl::array<VALUE> &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  a_trg->push('[');

  if (a_value.used() > 0)
  {
    const VALUE *ptr = a_value.data();
    const VALUE *ptr_end = a_value.data() + a_value.used();

    do {
      a_trg->string(*ptr);

      if (++ptr >= ptr_end)
      {
        break;
      }

      a_trg->push(',');

    } while(true);
  }

  return a_trg->push(']');
}/*}}}*/

// - function to_json -
inline auto to_json(const int16_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%hd",a_value);
}/*}}}*/

inline auto to_json(const uint16_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%hu",a_value);
}/*}}}*/

inline auto to_json(const int32_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%d",a_value);
}/*}}}*/

inline auto to_json(const uint32_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%u",a_value);
}/*}}}*/

inline auto to_json(const int64_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%lld",a_value);
}/*}}}*/

inline auto to_json(const uint64_t &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%llu",a_value);
}/*}}}*/

inline auto to_json(const float &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_json(const double &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_json(const bool &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  if (a_value) {
    a_trg->append(4,"true");
  }
  else {
    a_trg->append(5,"false");
  }

  return *a_trg;
}/*}}}*/

template <typename TYPE>
auto to_json(const TYPE &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_value.to_json(a_trg);
}/*}}}*/

auto to_json(const std::string &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &;

template <typename VALUE>
auto to_json(const ccl::array<VALUE> &a_value,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  a_trg->push('[');

  if (a_value.used() > 0)
  {
    const VALUE *ptr = a_value.data();
    const VALUE *ptr_end = a_value.data() + a_value.used();

    do {
      a_trg->json(*ptr);

      if (++ptr >= ptr_end)
      {
        break;
      }

      a_trg->push(',');

    } while(true);
  }

  return a_trg->push(']');
}/*}}}*/

namespace ccl {

// === definition of structure json_nice_s =====================================

struct json_nice_s
{/*{{{*/
private:
  array<char> m_tab;
  array<char> m_indent;
  uint32_t m_indent_size = 0;
  void *m_user = nullptr;

public:
  json_nice_s() = delete;
  json_nice_s(const json_nice_s &a_src) = delete;
  json_nice_s(json_nice_s &&a_src) = delete;

  json_nice_s(const char *a_tab,const char *a_indent,void *a_user) noexcept
  {/*{{{*/
    m_tab.append(strlen(a_tab),a_tab);
    m_indent.push('\n');
    m_indent.append(strlen(a_indent),a_indent);
    m_indent_size = m_indent.used();
    m_user = a_user;
  }/*}}}*/

  auto operator=(const json_nice_s &a_src) -> json_nice_s & = delete;
  auto operator=(json_nice_s &&a_src) -> json_nice_s & = delete;

  auto push() noexcept -> json_nice_s &
  {/*{{{*/
    if ((m_indent_size += m_tab.used()) > m_indent.used())
    {
      m_indent.append(m_tab.used(),m_tab.data());
    }

    return *this;
  }/*}}}*/

  auto pop() noexcept -> json_nice_s &
  {/*{{{*/
    m_indent_size -= m_tab.used();

    return *this;
  }/*}}}*/

  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append(m_indent_size,m_indent.data());
  }/*}}}*/
};/*}}}*/

} // namespace ccl

// - function to_json_nice -
inline auto to_json_nice(const int16_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%hd",a_value);
}/*}}}*/

inline auto to_json_nice(const uint16_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%hu",a_value);
}/*}}}*/

inline auto to_json_nice(const int32_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%d",a_value);
}/*}}}*/

inline auto to_json_nice(const uint32_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%u",a_value);
}/*}}}*/

inline auto to_json_nice(const int64_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%lld",a_value);
}/*}}}*/

inline auto to_json_nice(const uint64_t &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%llu",a_value);
}/*}}}*/

inline auto to_json_nice(const float &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_json_nice(const double &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->append_format("%f",a_value);
}/*}}}*/

inline auto to_json_nice(const bool &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  if (a_value) {
    a_trg->append(4,"true");
  }
  else {
    a_trg->append(5,"false");
  }

  return *a_trg;
}/*}}}*/

template <typename TYPE>
auto to_json_nice(const TYPE &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  return a_value.to_json_nice(a_json_nice,a_trg);
}/*}}}*/

inline auto to_json_nice(const std::string &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  (void)a_json_nice;

  return a_trg->json(a_value);
}/*}}}*/

template <typename VALUE>
auto to_json_nice(const ccl::array<VALUE> &a_value,ccl::json_nice_s *a_json_nice,ccl::array<char> *a_trg) noexcept -> ccl::array<char> &
{/*{{{*/
  if (a_value.used() > 0)
  {
    (*a_trg)
      .push('[')
      .json(a_json_nice->push());

    const VALUE *ptr = a_value.data();
    const VALUE *ptr_end = a_value.data() + a_value.used();

    do {
      a_trg->json_nice(*ptr,a_json_nice);

      if (++ptr >= ptr_end)
      {
        break;
      }

      (*a_trg)
        .push(',')
        .json(*a_json_nice);

    } while(true);

    (*a_trg)
      .json(a_json_nice->pop())
      .push(']');
  }
  else
  {
    (*a_trg)
      .push('[')
      .push(']');
  }

  return *a_trg;
}/*}}}*/

template <>
template <typename VALUE>
auto ccl::array<char>::string(const VALUE &a_value) noexcept -> array<char> &
{/*{{{*/
  return to_string(a_value,this);
}/*}}}*/

template <>
template <typename VALUE>
auto ccl::array<char>::json(const VALUE &a_value) noexcept -> array<char> &
{/*{{{*/
  return to_json(a_value,this);
}/*}}}*/

template <>
template <typename VALUE>
auto ccl::array<char>::json_nice(const VALUE &a_value,ccl::json_nice_s *a_json_nice) noexcept -> array<char> &
{/*{{{*/
  return to_json_nice(a_value,a_json_nice,this);
}/*}}}*/

#include "ccl_list.h"
#include "ccl_queue.h"
#include "ccl_tree.h"

// - function string_format -
auto string_format(const char *a_format,...) noexcept -> std::string;

#endif

