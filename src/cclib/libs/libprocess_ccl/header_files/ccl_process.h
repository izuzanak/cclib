
#ifndef LIBS_LIBPROCESS_CCL_HEADER_FILES_CCL_PROCESS_H
#define LIBS_LIBPROCESS_CCL_HEADER_FILES_CCL_PROCESS_H

#include "ccl_logger.h"

#include <csignal>

namespace ccl {

constexpr int PROCESS_DIRECTORY_ERROR = 1;
constexpr int PROCESS_RUN_FILE_ALREADY_EXIST = 2;

constexpr int SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR = 1;

// === definition of structure process_s =======================================

struct process_s
{/*{{{*/
public:
  static constexpr const char *c_run_dir_path = "/dev/shm/run";
  static constexpr const char *c_log_dir_path = "/dev/shm/log";

private:
  std::string m_name;
  logger_s m_logger;
  pid_t m_pid = -1;

public:
  ~process_s()
  {/*{{{*/
    if (m_pid != -1)
    {
      // - log message -
      log_info_0("termination");

      // - read process pid from file -
      array<char> pid_path("%s/%s.pid",c_run_dir_path,m_name.data());
      array<char> pid_buff;

      file_s(pid_path.data(),"r").read(&pid_buff).close();
      pid_t pid = strtoll(pid_buff.data(),nullptr,10);

      if (pid == m_pid)
      {
        // - unlink pid file -
        unlink(pid_path.data());
      }
    }

    // - reset global logger pointer -
    g_logger = nullptr;
  }/*}}}*/

  process_s() = default;
  process_s(const process_s &a_src) = delete;
  process_s(process_s &&a_src) = default;

  explicit process_s(std::string a_name);

  auto operator=(const process_s &a_src) -> process_s & = delete;
  auto operator=(process_s &&a_src) -> process_s & = default;

  using signal_callback_t = void (int);
  static auto simple_signal_handler(signal_callback_t a_handler) -> void;
};/*}}}*/

} // namespace ccl

#endif

