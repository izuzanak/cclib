
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libprocess_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "process",
  "signal",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_process,
  test_signal,
};/*}}}*/

// === test execution functions ================================================

std::atomic<bool> g_terminate(false);

void test_process()
{/*{{{*/
  cassert(system(string_format("mkdir -p '%s'; mkdir -p '%s'",             // NOLINT(cert-env33-c)
    ccl::process_s::c_log_dir_path,ccl::process_s::c_run_dir_path).data()) == 0);

  struct stat st;
  ccl::array<char> log_file("%s/%s.log",ccl::process_s::c_log_dir_path,test_name);
  ccl::array<char> run_file("%s/%s.pid",ccl::process_s::c_run_dir_path,test_name);

  {
    ccl::process_s process(test_name);

    cassert(stat(log_file.data(),&st) == 0 && !S_ISDIR(st.st_mode));
    cassert(stat(run_file.data(),&st) == 0 && !S_ISDIR(st.st_mode));
  }

  cassert(stat(run_file.data(),&st) != 0);
}/*}}}*/

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  g_terminate.store(true);
}/*}}}*/

void test_signal()
{/*{{{*/
  ccl::process_s::simple_signal_handler(signal_handler);
  cassert(kill(getpid(),SIGTERM) == 0);

  while (!g_terminate.load())
  {
    sleep(1);
  }
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

