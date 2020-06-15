
#include "ccl_logger.h"

namespace ccl {

// === constants and definitions ===============================================

logger_s *g_logger = nullptr;

// === methods of structure log_file_s =========================================

auto log_file_s::rotate() -> log_file_s &
{/*{{{*/
  debug_assert(!m_path.empty());

  struct stat st;

  // - close log file -
  m_file.close();

  array<char> old_buff;
  array<char> new_buff;

  uint32_t new_idx = m_max_count - 1;
  do
  {
    // - format name of new file -
    if (new_idx > 0)
    {
      new_buff.used() = 0;
      new_buff.append_format("%s.%u",m_path.data(),new_idx);
    }
    else
    {
      new_buff.used() = 0;
      new_buff.append(m_path.length() + 1,m_path.data());
    }

    if (new_idx + 1 < m_max_count)
    {
      // - format name of old file -
      old_buff.used() = 0;
      old_buff.append_format("%s.%u",m_path.data(),new_idx + 1);

      // - unlink old file -
      if (stat(old_buff.data(),&st) == 0)
      {
        if (unlink(old_buff.data()) != 0)
        {
          throw_error(LOG_FILE_UNLINK_ERROR);
        }
      }

      // - rename new file to old file -
      if (stat(new_buff.data(),&st) == 0)
      {
        if (rename(new_buff.data(),old_buff.data()) != 0)
        {
          throw_error(LOG_FILE_RENAME_ERROR);
        }
      }
    }
    else
    {
      // - unlink new file -
      if (stat(new_buff.data(),&st) == 0)
      {
        if (unlink(new_buff.data()) != 0)
        {
          throw_error(LOG_FILE_UNLINK_ERROR);
        }
      }
    }

  } while(new_idx-- != 0);

  // - open new log file -
  m_file = file_s(new_buff.data(),"a");

  return *this;
}/*}}}*/

// === methods of structure logger_s ===========================================

auto logger_s::write_ap(unsigned a_level,const char *a_format,va_list a_ap) -> logger_s &
{/*{{{*/
  time_s time = time_s::now();

  m_buffer.reset()
    .string(time)
    .append_format(".%3.3u %lld %s: ",time.milli() % 1000ULL,a_level,m_user.data())
    .append_format(a_format,a_ap)
    .push('\n');

  if (m_log_files.root_idx() != c_idx_not_exist)
  {
    const auto *lftn_ptr = m_log_files.data();
    const auto *lftn_ptr_end = lftn_ptr + m_log_files.used();
    do {
      if (lftn_ptr->is_valid())
      {
        const_cast<log_file_s &>(lftn_ptr->object())
          .write(a_level,m_buffer.used(),m_buffer.data());
      }
    } while(++lftn_ptr < lftn_ptr_end);
  }

  return *this;
}/*}}}*/

} // namespace ccl

