
#include "ccl_process.h"

namespace ccl {

// === methods of structure process_s ==========================================

process_s::process_s(std::string a_name) : m_name(std::move(a_name))
{/*{{{*/
  struct stat st;

  // - check log directory -
  if (stat(c_log_dir_path,&st) != 0 || !S_ISDIR(st.st_mode))
  {
    throw_error(PROCESS_DIRECTORY_ERROR);
  }

  // - create logger -
  m_logger = logger_s(m_name);

  array<char> buffer("%s/%s.log",c_log_dir_path,m_name.data());
  m_logger.add_file(9,buffer.data(),10485760,5);

  // - set global logger pointer -
  g_logger = &m_logger;

  // - check run directory -
  if (stat(c_run_dir_path,&st) != 0 || !S_ISDIR(st.st_mode))
  {
    throw_error(PROCESS_DIRECTORY_ERROR);
  }

  buffer.used() = 0;
  buffer.append_format("%s/%s.pid",c_run_dir_path,m_name.data());

  // - check process pid file -
  if (stat(buffer.data(),&st) == 0)
  {
    throw_error(PROCESS_RUN_FILE_ALREADY_EXIST);
  }

  pid_t pid = getpid();

  // - create process pid file -
  ccl::file_s file(buffer.data(),"w");

  buffer.used() = 0;
  buffer.append_format("%lld\n",static_cast<int64_t>(pid));
  file.write(buffer.used(),buffer.data()).close();

  m_pid = pid;

  // - log message -
  log_info_0("initialization");
}/*}}}*/

auto process_s::simple_signal_handler(signal_callback_t a_handler) -> void
{/*{{{*/

  // - create signal ignore action -
  struct sigaction ignore_action;
  ignore_action.sa_handler = SIG_IGN;
  if (sigemptyset(&ignore_action.sa_mask) != 0)
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }
  ignore_action.sa_flags = 0;

  // - ignore SIGPIPE signal -
  if (sigaction(SIGPIPE,&ignore_action,nullptr) != 0)
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }

  // - create signal handler action -
  struct sigaction signal_action;
  signal_action.sa_handler = a_handler;
  if (sigemptyset(&signal_action.sa_mask) != 0)
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }
  signal_action.sa_flags = 0;

  struct sigaction old_action;
  constexpr std::array<int,6> signals{SIGINT,SIGTERM,SIGHUP,SIGCHLD,SIGUSR1,SIGALRM};

  for (auto signal : signals)
  {
    if (sigaction(signal,nullptr,&old_action) != 0)
    {
      throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
    }

    if (old_action.sa_handler != SIG_IGN)
    {
      if (sigaction(signal,&signal_action,nullptr) != 0)
      {
        throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
      }
    }
  }
}/*}}}*/

} // namespace ccl

