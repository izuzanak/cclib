
#ifndef LIBS_LIBLOGGER_CCL_HEADER_FILES_CCL_LOGGER_H
#define LIBS_LIBLOGGER_CCL_HEADER_FILES_CCL_LOGGER_H

#include "ccl_sys.h"
#include "ccl_time.h"

namespace ccl {

constexpr int LOG_FILE_UNLINK_ERROR = 1;
constexpr int LOG_FILE_RENAME_ERROR = 2;

constexpr int LOGGER_LOG_FILE_INVALID_PARAMETERS = 1;

// === constants and definitions ===============================================

struct logger_s;
extern logger_s *g_logger;

#define LOG_MSG_MAX 1024
#define LOG_MSG_FORMAT "%.*s%s"
#define LOG_MSG_PARAMETERS(LENGTH,DATA) \
  (LENGTH) < LOG_MSG_MAX ? (LENGTH) : LOG_MSG_MAX,DATA,(LENGTH) < LOG_MSG_MAX ? "" : " ..."

// - processes log levels -
enum
{/*{{{*/
  log_lvl_fatal   = 0,
  log_lvl_error   = 1,
  log_lvl_warning = 3,
  log_lvl_info_0  = 1,
  log_lvl_info_1  = 3,
  log_lvl_info_2  = 5,
  log_lvl_verbose = 8,
};/*}}}*/

// - define log macros -
#define log_fatal(...) \
  if (g_logger) { g_logger->write(log_lvl_fatal,__VA_ARGS__); }

#define log_error(...) \
  if (g_logger) { g_logger->write(log_lvl_error,__VA_ARGS__); }

#define log_warning(...) \
  if (g_logger) { g_logger->write(log_lvl_warning,__VA_ARGS__); }

#define log_info_0(...) \
  if (g_logger) { g_logger->write(log_lvl_info_0,__VA_ARGS__); }

#define log_info_1(...) \
  if (g_logger) { g_logger->write(log_lvl_info_1,__VA_ARGS__); }

#define log_info_2(...) \
  if (g_logger) { g_logger->write(log_lvl_info_2,__VA_ARGS__); }

#define log_verbose(...) \
  if (g_logger) { g_logger->write(log_lvl_verbose,__VA_ARGS__); }

// === definition of structure log_file_s ======================================

struct log_file_s
{/*{{{*/
private:
  uint32_t m_level;
  std::string m_path;
  uint32_t m_max_size;
  uint32_t m_max_count;
  file_s m_file;

public:
  ~log_file_s() = default;

  log_file_s() = default;
  log_file_s(const log_file_s &a_src) = delete;
  log_file_s(log_file_s &&a_src) = default;

  log_file_s(uint32_t a_level,std::string a_path,uint32_t a_max_size,uint32_t a_max_count) :
    m_level(a_level), m_path(std::move(a_path)), m_max_size(a_max_size), m_max_count(a_max_count), m_file(m_path.data(),"a") {}

  auto operator=(const log_file_s &a_src) -> log_file_s & = delete;
  auto operator=(log_file_s &&a_src) -> log_file_s & = default;

  [[nodiscard]] auto compare(const log_file_s &a_second) const noexcept -> int
  {/*{{{*/
    return ::compare(m_path,a_second.m_path);
  }/*}}}*/

  auto static to_string(array<char> *a_trg) noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append(10,"log_file_s{}");
  }/*}}}*/

  auto write(uint32_t a_level,uint32_t a_size,const char *a_data) -> log_file_s &
  {/*{{{*/
    if (m_level >= a_level)
    {
      int64_t count = m_file.tell();

      // - maximal byte count exceeded -
      if (count + a_size > m_max_size)
      {
        rotate();
      }

      m_file.write(a_size,a_data).flush();
    }

    return *this;
  }/*}}}*/

  auto rotate() -> log_file_s &;
};/*}}}*/

// === definition of structure logger_s ========================================

struct logger_s
{/*{{{*/
private:
  std::string m_user;
  tree<log_file_s> m_log_files;
  array<char> m_buffer;

public:
  ~logger_s() = default;

  logger_s() = default;
  logger_s(const logger_s &a_src) = delete;
  logger_s(logger_s &&a_src) = default;

  explicit logger_s(std::string a_user) : m_user(std::move(a_user)) {}

  auto operator=(const logger_s &a_src) -> logger_s & = delete;
  auto operator=(logger_s &&a_src) -> logger_s & = default;

  auto static to_string(array<char> *a_trg) noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->append(8,"logger_s{}");
  }/*}}}*/

  auto add_file(uint32_t a_level,std::string a_path,uint32_t a_max_size,uint32_t a_max_count) -> logger_s &
  {/*{{{*/
    if (a_max_size < 4096 || a_max_count < 1)
    {
      throw_error(LOGGER_LOG_FILE_INVALID_PARAMETERS);
    }

    m_log_files.unique_insert(log_file_s(a_level,std::move(a_path),a_max_size,a_max_count));

    return *this;
  }/*}}}*/

  auto write(unsigned a_level,const char *a_format,...) -> logger_s & // NOLINT(cert-dcl50-cpp)
  {/*{{{*/
    va_list ap;
    va_start(ap,a_format);
    write_ap(a_level,a_format,ap);
    va_end(ap);

    return *this;
  }/*}}}*/

  auto write_ap(unsigned a_level,const char *a_format,va_list a_ap) -> logger_s &;
};/*}}}*/

} // namespace ccl

#endif

