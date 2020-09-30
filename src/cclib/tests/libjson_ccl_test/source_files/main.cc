
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libjson_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "json",
  "string",
  "comments",
  "load",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_json,
  test_string,
  test_comments,
  test_load,
};/*}}}*/

// === test execution functions ================================================

void test_json()
{/*{{{*/
  ccl::array<char> buffer;

  const std::string data =
R"'({
  "null": null,
  "true": true,
  "false": false,
  "integer": 123,
  "float": 123.400000,
  "string": "Hello world!",
  "array": [0,1,2,3,4,5,6,7,8,9],
  "object": {
    "first": 0,
    "second": 1,
    "third": 2
  }
})'";

  ccl::var_s value = ccl::json_parser_s().parse(data.length(),data.data());
  cassert(strcmp(buffer.debug(value),
        "[array:[0,1,2,3,4,5,6,7,8,9],false:<false>,float:123.400000,integer:123,null:<blank>,object:[first:0,second:1,third:2],string:Hello world!,true:<true>]") == 0);

  ccl::json_parser_s parser;
  cassert(strcmp(buffer.debug(parser.parse(4,"null")),"<blank>") == 0);
  cassert(strcmp(buffer.debug(parser.parse(4,"true")),"<true>") == 0);
  cassert(strcmp(buffer.debug(parser.parse(5,"false")),"<false>") == 0);
  cassert(strcmp(buffer.debug(parser.parse(3,"123")),"123") == 0);
  cassert(strcmp(buffer.debug(parser.parse(5,"123.4")),"123.400000") == 0);
  cassert(strcmp(buffer.debug(parser.parse(14,"\"Hello world!\"")),"Hello world!") == 0);
  cassert(strcmp(buffer.debug(parser.parse(9,"[1,2,3,4]")),"[1,2,3,4]") == 0);
}/*}}}*/

void test_string()
{/*{{{*/
  ccl::array<char> buffer;

  buffer.reset().json(std::string("escapes: \b\f\n\r\t\\\"")).push('\0');
  cassert(strcmp(buffer.data(),R"'("escapes: \b\f\n\r\t\\\"")'") == 0);

  ccl::var_s var_0 = ccl::json_parser_s().parse(buffer.used(),buffer.data());
  cassert(strcmp(var_0.to_str().data(),"escapes: \b\f\n\r\t\\\"") == 0);

  const std::string uchar_1 = R"'("\u03b1")'";
  ccl::var_s var_1 = ccl::json_parser_s().parse(uchar_1.length(),uchar_1.data());
  cassert(strcmp(var_1.to_str().data(),"α") == 0);

  const std::string uchar_2 = R"'("\u03B2")'";
  ccl::var_s var_2 = ccl::json_parser_s().parse(uchar_2.length(),uchar_2.data());
  cassert(strcmp(var_2.to_str().data(),"β") == 0);

  const std::string uchar_3 = R"'("\u25e6")'";
  ccl::var_s var_3 = ccl::json_parser_s().parse(uchar_3.length(),uchar_3.data());
  cassert(strcmp(var_3.to_str().data(),"◦") == 0);

  const std::string uchar_4 = R"'("\u0041\u0042\u0043")'";
  ccl::var_s var_4 = ccl::json_parser_s().parse(uchar_4.length(),uchar_4.data());
  cassert(strcmp(var_4.to_str().data(),"ABC") == 0);
}/*}}}*/

void test_comments()
{/*{{{*/
  const std::string data =
R"'(
  # first comment type
  // second comment type
  /*
   third // comment // type
   */
  {}
)'";

  cassert(ccl::json_parser_s().parse(data.length(),data.data()) == ccl::var_s::new_dict() );
}/*}}}*/

void test_load()
{/*{{{*/
  //ccl::array<char> buffer;
  //ccl::file_s("/home/jirka/folder/work/programming/git/uclang/uclang_build/macs/generated/phase.json","rb")
  //  .read(&buffer).close();

  //ccl::json_parser_s parser;
  //ccl::var_s data = parser.parse(buffer.used(),buffer.data());

  //uint32_t idx = 0;
  //do {
  //  data[0] = data[1];
  //} while(++idx < 1000000);

  //data[0]["_id"] = "Identifier";

  //puts(buffer.debug(data[0]));
  //puts(buffer.debug(data[1]));

  //new (&data) ccl::var_s;
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

