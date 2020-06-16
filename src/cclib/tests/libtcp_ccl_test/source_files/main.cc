
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libtcp_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "tcp",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_tcp,
};/*}}}*/

// === test execution functions ================================================

void test_tcp()
{/*{{{*/
  auto server_pid = ccl::pid_s({"./libtcp_ccl_test_tcp_server"});
  usleep(100000);

  auto client_pid = ccl::pid_s({"./libtcp_ccl_test_tcp_client"});
  usleep(1000000);

  cassert(kill(server_pid,SIGTERM) == 0);

  int status;
  cassert(waitpid(client_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(server_pid,&status,0) != -1 && status == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

