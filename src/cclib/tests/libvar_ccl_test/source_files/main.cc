
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libvar_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "var_blank",
  "var_bool",
  "var_int",
  "var_float",
  "var_string",
  "var_array",
  "var_dict",
  "var_constr",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_var_blank,
  test_var_bool,
  test_var_int,
  test_var_float,
  test_var_string,
  test_var_array,
  test_var_dict,
  test_var_constr,
};/*}}}*/

// === test execution functions ================================================

void test_var_blank()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s blank_0;
  cassert(strcmp(buffer.debug(blank_0),"<blank>") == 0);

  ccl::var_s blank_1;
  cassert(blank_0 == blank_1 && blank_0.compare(blank_1) == 0);

  // - var to_json -
  buffer.reset().json(blank_0).push('\0');
  cassert(strcmp(buffer.data(),"null") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(blank_0,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"null") == 0);
}/*}}}*/

void test_var_bool()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s bool_0(false);
  cassert(strcmp(buffer.debug(bool_0),"<false>") == 0);

  ccl::var_s bool_1(true);
  cassert(strcmp(buffer.debug(bool_1),"<true>") == 0);

  cassert(bool_0 == ccl::var_s(false) && bool_1 == ccl::var_s(true));
  cassert(bool_0.to_bool() == false && bool_1.to_bool() == true);

  cassert(bool_0.compare(true) == -1);
  cassert(bool_0.compare(false) == 0);
  cassert(bool_1.compare(true) == 0);
  cassert(bool_1.compare(false) == 1);

  // - var to_json -
  buffer.reset().json(bool_0).push('\0');
  cassert(strcmp(buffer.data(),"false") == 0);

  buffer.reset().json(bool_1).push('\0');
  cassert(strcmp(buffer.data(),"true") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(bool_0,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"false") == 0);

  buffer.reset().json_nice(bool_1,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"true") == 0);
}/*}}}*/

void test_var_int()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s int_0(120);
  cassert(strcmp(buffer.debug(int_0),"120") == 0);

  cassert(int_0 == ccl::var_s(120));
  cassert(int_0.to_int() == 120);

  cassert(int_0.compare(ccl::var_s(119)) ==  1 &&
          int_0.compare(ccl::var_s(120)) ==  0 &&
          int_0.compare(ccl::var_s(121)) == -1);

  // - var to_json -
  buffer.reset().json(int_0).push('\0');
  cassert(strcmp(buffer.data(),"120") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(int_0,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"120") == 0);
}/*}}}*/

void test_var_float()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s float_0(120.123);
  cassert(strcmp(buffer.debug(float_0),"120.123000") == 0);

  cassert(float_0 == ccl::var_s(120.123));
  cassert(float_0.to_float() == 120.123);

  cassert(float_0.compare(ccl::var_s(119.123)) ==  1 &&
          float_0.compare(ccl::var_s(120.123)) ==  0 &&
          float_0.compare(ccl::var_s(121.123)) == -1);

  // - var to_json -
  buffer.reset().json(float_0).push('\0');
  cassert(strcmp(buffer.data(),"120.123000") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(float_0,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"120.123000") == 0);
}/*}}}*/

void test_var_string()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s string_0("Hello world!!!");
  cassert(strcmp(buffer.debug(string_0),"Hello world!!!") == 0);

  std::string str_1("Hello world!!!");
  ccl::var_s string_1(std::string{str_1});

  cassert(string_0 == string_1);
  cassert(string_0 == ccl::var_s("Hello world!!!"));
  cassert(string_0.to_str() == "Hello world!!!");

  cassert(string_0.compare(ccl::var_s("Hello world!!")) ==  1 &&
          string_0.compare(ccl::var_s("Hello world!!!")) ==  0 &&
          string_0.compare(ccl::var_s("Hello world!!!!")) == -1);

  cassert(string_0.length() == 14);

  // - var to_json -
  buffer.reset().json(string_0).push('\0');
  cassert(strcmp(buffer.data(),"\"Hello world!!!\"") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(string_0,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"\"Hello world!!!\"") == 0);
}/*}}}*/

void test_var_array()
{/*{{{*/
  ccl::array<char> buffer;

  // - var new_array -
  ccl::var_s array_0 = ccl::var_s::new_array();
  cassert(strcmp(buffer.debug(array_0),"[]") == 0);

  // - var push -
  uint32_t idx = 0;
  do {
    array_0.push(ccl::var_s(idx));
  } while(++idx < 10);

  cassert(strcmp(buffer.debug(array_0),"[0,1,2,3,4,5,6,7,8,9]") == 0);

  ccl::var_s array_1(ccl::array<ccl::var_s>(array_0.to_array()));
  array_1.push(ccl::var_s("Hello world!"));

  // - var operator== -
  cassert(!(array_0 == array_1));
  cassert(strcmp(buffer.debug(array_1),"[0,1,2,3,4,5,6,7,8,9,Hello world!]") == 0);

  ccl::var_s array_2(ccl::array<ccl::var_s>(array_0.to_array()));
  cassert(array_0 == array_2);

  // - var compare -
  cassert(array_1.compare(array_0) == 1 &&
          array_0.compare(array_2) == 0 &&
          array_0.compare(array_1) == -1);

  // - var length -
  cassert(array_0.length() == 10 &&
          array_1.length() == 11);

  // - var pop -
  array_1.to_array().used() = 0;
  cassert(strcmp(buffer.debug(array_0),"[0,1,2,3,4,5,6,7,8,9]") == 0);
  cassert(strcmp(buffer.debug(array_1),"[]") == 0);

  while (array_0.length() > 0)
  {
    array_1.push(array_0.pop());
  }

  cassert(strcmp(buffer.debug(array_0),"[]") == 0);
  cassert(strcmp(buffer.debug(array_1),"[9,8,7,6,5,4,3,2,1,0]") == 0);

  // - var operator [] -
  ccl::var_s array_3 = ccl::var_s::new_array();

  idx = 0;
  do {
    array_3.push(ccl::var_s(string_format("%lld-%lld",
          array_1[idx].to_int(),array_1[ccl::var_s(idx)].to_int())));
  } while(++idx < array_1.length());

  cassert(strcmp(buffer.debug(array_3),"[9-9,8-8,7-7,6-6,5-5,4-4,3-3,2-2,1-1,0-0]") == 0);

  // - var loop -
  ccl::var_s items = ccl::var_s::new_array();
  for (ccl::var_s &var : array_3)
  {
    items.push(var);
  }
  cassert(strcmp(buffer.debug(items),"[9-9,8-8,7-7,6-6,5-5,4-4,3-3,2-2,1-1,0-0]") == 0);

  // - var to_json -
  buffer.reset().json(ccl::var_s::new_array()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(array_3).push('\0');
  cassert(strcmp(buffer.data(),
    R"'(["9-9","8-8","7-7","6-6","5-5","4-4","3-3","2-2","1-1","0-0"])'") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::var_s::new_array(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(array_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>"9-9",
-->>"8-8",
-->>"7-7",
-->>"6-6",
-->>"5-5",
-->>"4-4",
-->>"3-3",
-->>"2-2",
-->>"1-1",
-->>"0-0"
--])'") == 0);
}/*}}}*/

void test_var_dict()
{/*{{{*/
  ccl::array<char> buffer;

  // - var new_dict -
  ccl::var_s dict_0 = ccl::var_s::new_dict();
  cassert(strcmp(buffer.debug(dict_0),"[]") == 0);

  // - var operator[] -
  uint32_t idx = 0;
  do {
    dict_0[ccl::var_s(string_format("idx(%u)",idx))] = ccl::var_s(idx);
  } while(++idx < 5);

  cassert(dict_0[ccl::var_s("idx(2)")].to_int() == 2);
  cassert(dict_0[ccl::var_s("idx(3)")].to_int() == 3);
  cassert(strcmp(buffer.debug(dict_0),
        "[idx(0):0,idx(1):1,idx(2):2,idx(3):3,idx(4):4]") == 0);

  ccl::var_s dict_1(ccl::tree<ccl::map_s>(dict_0.to_dict()));
  dict_1[ccl::var_s("Hello")] = ccl::var_s(120);

  // - var operator== -
  cassert(!(dict_0 == dict_1));
  cassert(strcmp(buffer.debug(dict_1),
        "[Hello:120,idx(0):0,idx(1):1,idx(2):2,idx(3):3,idx(4):4]") == 0);

  ccl::var_s dict_2(ccl::tree<ccl::map_s>(dict_0.to_dict()));
  cassert(dict_0 == dict_2);

  // - var compare -
  cassert(dict_1.compare(dict_0) == 1 &&
          dict_0.compare(dict_2) == 0 &&
          dict_0.compare(dict_1) == -1);

  // - var length -
  cassert(dict_0.length() == 5 &&
          dict_1.length() == 6);

  // - var remove_key -
  dict_1.remove_key(ccl::var_s("idx(1)"));
  dict_1.remove_key(ccl::var_s("idx(3)"));

  cassert(strcmp(buffer.debug(dict_1),"[Hello:120,idx(0):0,idx(2):2,idx(4):4]") == 0);

  // - var has_key -
  // - var remove_if_key -
  ccl::var_s dict_3 = ccl::var_s::new_dict();

  idx = 0;
  do {
    ccl::var_s key(string_format("idx(%u)",idx));

    dict_3[key] = ccl::var_s(static_cast<int64_t>(dict_1.has_key(key)));
    dict_1.remove_if_key(key);
  } while(++idx < 5);

  cassert(strcmp(buffer.debug(dict_3),"[idx(0):1,idx(1):0,idx(2):1,idx(3):0,idx(4):1]") == 0);
  cassert(strcmp(buffer.debug(dict_1),"[Hello:120]") == 0);

  // - var loop -
  ccl::var_s keys = ccl::var_s::new_array();
  for (ccl::var_s &key : dict_3)
  {
    keys.push(key);
  }
  cassert(strcmp(buffer.debug(keys),"[idx(0),idx(1),idx(2),idx(3),idx(4)]") == 0);

  // - var to_json -
  buffer.reset().json(ccl::var_s::new_dict()).push('\0');
  cassert(strcmp(buffer.data(),"{}") == 0);

  buffer.reset().json(dict_3).push('\0');
  cassert(strcmp(buffer.data(),
    R"'({"idx(0)":1,"idx(1)":0,"idx(2)":1,"idx(3)":0,"idx(4)":1})'") == 0);

  // - var to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::var_s::new_dict(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"{}") == 0);

  buffer.reset().json_nice(dict_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'({
-->>"idx(0)": 1,
-->>"idx(1)": 0,
-->>"idx(2)": 1,
-->>"idx(3)": 0,
-->>"idx(4)": 1
--})'") == 0);
}/*}}}*/

void test_var_constr()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::var_s blank_0{};
  cassert(strcmp(buffer.debug(blank_0),"<blank>") == 0);

  ccl::var_s int_0(10);
  cassert(strcmp(buffer.debug(int_0),"10") == 0);

  ccl::var_s float_0(10.0);
  cassert(strcmp(buffer.debug(float_0),"10.000000") == 0);

  ccl::var_s string_0("Hello world!!!");
  cassert(strcmp(buffer.debug(string_0),"Hello world!!!") == 0);

  ccl::var_s array_0{1,2,3,4,5};
  cassert(strcmp(buffer.debug(array_0),"[1,2,3,4,5]") == 0);

  ccl::var_s dict_0{ccl::map_s
    {"dict",{ccl::map_s
      {"one",1},
      {"two",2},
      {"three",3},
    }},
    {"array",{1,2,3,4,5}},
    {"string","Hello world!!!"},
    {"float",12.345},
    {"int",12},
    {"blank",{}},
  };
  cassert(strcmp(buffer.debug(dict_0),
        "[array:[1,2,3,4,5],blank:<blank>,dict:[one:1,three:3,two:2],float:12.345000,int:12,string:Hello world!!!]") == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

