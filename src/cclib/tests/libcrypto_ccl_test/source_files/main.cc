
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libcrypto_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "random",
  "base16",
  "base64",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_random,
  test_base16,
  test_base64,
};/*}}}*/

// === test execution functions ================================================

void test_random()
{/*{{{*/
  ccl::array<char> buffer;
  ccl::crypto_s::random(256,&buffer);
}/*}}}*/

void test_base16()
{/*{{{*/
  ccl::array<char> data;
  ccl::crypto_s::random(512,&data);

  ccl::array<char> base16;
  ccl::crypto_s::encode_base16(data.used(),data.data(),&base16);

  printf("base16: %.*s\n",base16.used(),base16.data());

  ccl::array<char> decoded;
  ccl::crypto_s::decode_base16(base16.used(),base16.data(),&decoded);
  cassert(data == decoded);
}/*}}}*/

void test_base64()
{/*{{{*/
  ccl::array<char> data;
  ccl::crypto_s::random(512,&data);

  ccl::array<char> base64;
  ccl::crypto_s::encode_base64(data.used(),data.data(),&base64);

  printf("base64: %.*s\n",base64.used(),base64.data());

  ccl::array<char> decoded;
  ccl::crypto_s::decode_base64(base64.used(),base64.data(),&decoded);
  cassert(data == decoded);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

