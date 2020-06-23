
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libmqtt_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "varint",
  "mqtt",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_varint,
  test_mqtt,
};/*}}}*/

// === test execution functions ================================================

void test_varint()
{/*{{{*/
  ccl::array<char> buffer;
  const char *end;

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(0,&buffer);
  cassert(memcmp(buffer.data(),"\x00",1) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 0);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(127,&buffer);
  cassert(memcmp(buffer.data(),"\x7f",1) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 127);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(128,&buffer);
  cassert(memcmp(buffer.data(),"\x80\x01",2) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 128);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(16383,&buffer);
  cassert(memcmp(buffer.data(),"\xff\x7f",2) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 16383);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(16384,&buffer);
  cassert(memcmp(buffer.data(),"\x80\x80\x01",3) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 16384);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(2097151,&buffer);
  cassert(memcmp(buffer.data(),"\xff\xff\x7f",3) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 2097151);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(2097152,&buffer);
  cassert(memcmp(buffer.data(),"\x80\x80\x80\x01",4) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 2097152);

  buffer.used() = 0;
  ccl::mqtt_conn_s::varint_enc(268435455,&buffer);
  cassert(memcmp(buffer.data(),"\xff\xff\xff\x7f",4) == 0);
  cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == 268435455);

  uint32_t idx = 0;
  do {
    uint32_t number = rand() & 0x0fffffff; // NOLINT(cert-msc30-c,cert-msc50-cpp)

    buffer.used() = 0;
    ccl::mqtt_conn_s::varint_enc(number,&buffer);
    cassert(ccl::mqtt_conn_s::varint_dec(buffer.data(),&end) == number);

  } while(++idx < 1000);
}/*}}}*/

void test_mqtt()
{/*{{{*/

  // TODO(devel): continue ...

}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

