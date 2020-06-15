
#ifndef LIBS_LIBSYS_CCL_HEADER_FILES_CCL_SYS_H
#define LIBS_LIBSYS_CCL_HEADER_FILES_CCL_SYS_H

#include "ccl_base.h"

namespace ccl {

constexpr int FILE_OPEN_ERROR = 1;
constexpr int FILE_WRITE_ERROR = 2;
constexpr int FILE_FLUSH_ERROR = 3;
constexpr int FILE_READ_ERROR = 4;
constexpr int FILE_SEEK_ERROR = 5;
constexpr int FILE_TELL_ERROR = 6;

// === definition of structure file_s ==========================================

struct file_s
{/*{{{*/
private:
  FILE *m_file = nullptr;

public:
  ~file_s()
  {/*{{{*/
    if (m_file != nullptr)
    {
      fclose(m_file);
    }
  }/*}}}*/

  file_s() = default;
  file_s(const file_s &a_src) = delete;

  file_s(file_s &&a_src) noexcept
  {/*{{{*/
    m_file = a_src.m_file;
    a_src.m_file = nullptr;
  }/*}}}*/

  file_s(const char *a_pathname,const char *a_mode)
  {/*{{{*/
    m_file = fopen(a_pathname,a_mode);

    if (m_file == nullptr)
    {
      throw_error(FILE_OPEN_ERROR);
    }
  }/*}}}*/

  auto operator=(const file_s &a_src) -> file_s & = delete;
  auto operator=(file_s &&a_src) noexcept -> file_s &
  {/*{{{*/
    this->~file_s();

    m_file = a_src.m_file;
    a_src.m_file = nullptr;

    return *this;
  }/*}}}*/

  auto to_string(ccl::array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append_format("file_s{%p}",m_file);
  }/*}}}*/

  [[nodiscard]] auto fd() const noexcept -> int
  {/*{{{*/
    return fileno(m_file);
  }/*}}}*/

  auto write(size_t a_size,const void *a_data) -> file_s &
  {/*{{{*/
    debug_assert(m_file != nullptr);

    if (fwrite(a_data,1,a_size,m_file) != a_size)
    {
      throw_error(FILE_WRITE_ERROR);
    }

    return *this;
  }/*}}}*/

  auto flush() -> file_s &
  {/*{{{*/
    debug_assert(m_file != nullptr);

    if (fflush(m_file) != 0)
    {
      throw_error(FILE_FLUSH_ERROR);
    }

    return *this;
  }/*}}}*/

  auto read(array<char> *a_trg) -> file_s &;
  auto read_cnt(array<char> *a_trg,size_t a_size) -> file_s &;

  auto close() noexcept -> file_s &
  {/*{{{*/
    this->~file_s();
    new (this) file_s;

    return *this;
  }/*}}}*/

  auto seek(int64_t a_offset,int a_whence) -> file_s &
  {/*{{{*/
    debug_assert(m_file != nullptr);

    if (fseek(m_file,a_offset,a_whence) != 0)
    {
      throw_error(FILE_SEEK_ERROR);
    }

    return *this;
  }/*}}}*/

  auto tell() -> int64_t
  {/*{{{*/
    debug_assert(m_file != nullptr);

    int64_t offset;
    if ((offset = ftell(m_file)) < 0)
    {
      throw_error(FILE_TELL_ERROR);
    }

    return offset;
  }/*}}}*/
};/*}}}*/

} // namespace ccl

#endif

