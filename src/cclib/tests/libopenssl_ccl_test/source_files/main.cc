
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libopenssl_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "openssl",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_openssl,
};/*}}}*/

// === test execution functions ================================================

void test_openssl()
{/*{{{*/
  auto server = ccl::ssl_context_s::server();
  server.use_certificate_file("tests/libopenssl_ccl_test/resources/mycert.pem",SSL_FILETYPE_PEM);

  auto client = ccl::ssl_context_s::client();
  server.use_private_key_file("tests/libopenssl_ccl_test/resources/mycert.pem",SSL_FILETYPE_PEM);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

