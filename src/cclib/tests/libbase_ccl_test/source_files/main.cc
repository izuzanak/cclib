
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libbase_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "string",
  "basic_type_array",
  "static_type_array",
  "dynamic_type_array",
  "basic_type_list",
  "basic_type_queue",
  "basic_type_tree",
  "static_type_tree",
  "dynamic_type_tree",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_string,
  test_basic_type_array,
  test_static_type_array,
  test_dynamic_type_array,
  test_basic_type_list,
  test_basic_type_queue,
  test_basic_type_tree,
  test_static_type_tree,
  test_dynamic_type_tree,
};/*}}}*/

// === test execution functions ================================================

void test_string()
{/*{{{*/

  // - string_format -
  std::string string_0 = string_format(
      "Longer than %d bytes ================================================================================================================================",
      128);

  cassert(strcmp(string_0.data(),
        "Longer than 128 bytes ================================================================================================================================") == 0);
}/*}}}*/

void test_basic_type_array()
{/*{{{*/
  ccl::array<char> buffer;

  // - array init -
  ccl::array<uint32_t> array_0;
  cassert(array_0.size() == 0 && array_0.used() == 0 && array_0.data() == nullptr);

  // - array init_size -
  ccl::array<uint32_t> array_1(100);
  cassert(array_1.size() == 100 && array_1.used() == 0 && array_1.data() != nullptr);

  // - array set -
  std::array<uint32_t,10> data_0 = {1,2,3,4,5,6,7,8,9,10};
  array_0.set(data_0.size(),data_0.data());

  buffer.debug(array_0);
  cassert(array_0.used() == 10 && array_0.size() == 10 &&
    strcmp(buffer.data(),"[1,2,3,4,5,6,7,8,9,10]") == 0);

  // - array reset -
  array_0.reset();
  cassert(array_0.used() == 0);
  array_0.set(data_0.size(),data_0.data());

  // - array push -
  array_0.push(11);
  buffer.debug(array_0);
  cassert(array_0.used() == 11 && array_0.size() >= 11 &&
    strcmp(buffer.data(),"[1,2,3,4,5,6,7,8,9,10,11]") == 0);

  // - array flush -
  array_0.flush();
  cassert(array_0.used() == 11 && array_0.size() == 11);

  // - array reserve -
  array_0.reserve(10);
  cassert(array_0.used() == 11 && array_0.size() >= 21);

  // - array pop -
  array_1.used() = 0;
  uint32_t cnt = 0;
  do {
    array_1.push(array_0.pop());
  } while(++cnt < 5);

  buffer.debug(array_1);
  cassert(strncmp(buffer.data(),"[11,10,9,8,7]",buffer.used()) == 0);

  // - array last -
  cassert(array_0.last() == 6);

  // - array copy_resize -
  array_0.copy_resize(10);
  buffer.debug(array_0);
  cassert(array_0.used() == 6 && array_0.size() == 10 &&
    strcmp(buffer.data(),"[1,2,3,4,5,6]") == 0);

  // - array fill -
  array_0.fill(7);
  buffer.debug(array_0);
  cassert(array_0.used() == 10 && array_0.size() == 10 &&
    strcmp(buffer.data(),"[7,7,7,7,7,7,7,7,7,7]") == 0);

  // - array at -
  array_0[7] = 125;
  buffer.debug(array_0);
  cassert(strcmp(buffer.data(),"[7,7,7,7,7,7,7,125,7,7]") == 0);

  // - array get_idx -
  cassert(array_0.get_idx(0) == c_idx_not_exist);
  cassert(array_0.get_idx(125) == 7);

  // - array operator== -
  cassert(array_0 == array_0);
  cassert(!(array_0 == array_1));

  // - array compare -
  auto array_2 = array_0;
  cassert(array_0.compare(array_1) ==  1);
  cassert(array_2.compare(array_0) ==  0);
  cassert(array_1.compare(array_0) == -1);

  // - array operator= -
  array_0 = array_1;
  buffer.debug(array_0);
  cassert(array_0 == array_1 && strcmp(buffer.data(),"[11,10,9,8,7]") == 0);

  // - array initializer_list -
  ccl::array<uint32_t> array_3{1,2,3,4,5};
  cassert(strcmp(buffer.debug(array_3),"[1,2,3,4,5]") == 0);

  // - array to_json -
  buffer.reset().json(ccl::array<uint32_t>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(array_3).push('\0');
  cassert(strcmp(buffer.data(),"[1,2,3,4,5]") == 0);

  // - array to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::array<uint32_t>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(array_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>1,
-->>2,
-->>3,
-->>4,
-->>5
--])'") == 0);
}/*}}}*/

void test_static_type_array()
{/*{{{*/
  ccl::array<char> buffer;

  // - array init -
  ccl::array<static_s> array_0;
  cassert(array_0.size() == 0 && array_0.used() == 0);

  // - array set -
  const std::array<static_s,3> static_data = {static_s{1,2,3},static_s{4,5,6},static_s{7,8,9}};
  array_0.set(static_data.size(),static_data.data());
  buffer.debug(array_0);
  cassert(strcmp(buffer.data(),"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - array reset -
  array_0.reset();
  cassert(array_0.used() == 0);
  array_0.set(static_data.size(),static_data.data());

  // - array copy_resize -
  array_0.copy_resize(10);
  buffer.debug(array_0);
  cassert(array_0.size() == 10 && array_0.used() == 3 &&
    strcmp(buffer.data(),"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - array flush -
  array_0.flush();
  buffer.debug(array_0);
  cassert(array_0.size() == 3 && array_0.used() == 3 &&
    strcmp(buffer.data(),"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - array swap -
  ccl::array<static_s> array_1;
  array_0.swap(array_1);
  buffer.debug(array_1);
  cassert(array_0.size() == 0 && array_0.used() == 0 && array_1.size() == 3 && array_1.used() == 3 &&
    strcmp(buffer.data(),"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - array at -
  buffer.debug(array_1[2]);
  cassert(strcmp(buffer.data(),"{first:7,second:8,third:9}") == 0);

  // - array push -
  // - array pop -
  while (array_1.used() != 0)
  {
    array_0.push(array_1.pop());
  }
  buffer.debug(array_0);
  cassert(array_0.used() == 3 && array_1.used() == 0 &&
    strcmp(buffer.data(),"[{first:7,second:8,third:9},{first:4,second:5,third:6},{first:1,second:2,third:3}]") == 0);

  // - array last -
  buffer.debug(array_0.last());
  cassert(strcmp(buffer.data(),"{first:1,second:2,third:3}") == 0);

  // - array fill -
  array_0.used() = 0;
  array_0.copy_resize(3);
  static_s element{4,5,6};
  array_0.fill(element);
  buffer.debug(array_0);
  cassert(array_0.used() == 3 && array_0.size() == 3 &&
    strcmp(buffer.data(),"[{first:4,second:5,third:6},{first:4,second:5,third:6},{first:4,second:5,third:6}]") == 0);

  // - array push_blank -
  uint32_t idx = 0;
  do {
    array_1.push_blank();
    array_1.last() = {idx,static_cast<uint16_t>(idx),idx};
  } while(++idx < 3);
  buffer.debug(array_1);
  cassert(strcmp(buffer.data(),"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

  // - array get_idx -
  cassert(array_1.get_idx(array_1[1]) == 1);

  // - array operator= -
  // - array operator== -
  cassert(!(array_0 == array_1));
  array_0 = array_1;
  buffer.debug(array_0);
  cassert(array_0 == array_1 &&
    strcmp(buffer.data(),"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

  // - array initializer_list -
  ccl::array<static_s> array_3{
    {1,1,1},
    {2,2,2},
    {3,3,3},
  };
  cassert(strcmp(buffer.debug(array_3),"[{first:1,second:1,third:1},{first:2,second:2,third:2},{first:3,second:3,third:3}]") == 0);

  // - array to_json -
  buffer.reset().json(ccl::array<static_s>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(array_3).push('\0');
  cassert(strcmp(buffer.data(),"[{\"first\":1,\"second\":1,\"third\":1},{\"first\":2,\"second\":2,\"third\":2},{\"first\":3,\"second\":3,\"third\":3}]") == 0);

  // - array to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::array<static_s>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(array_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>{
-->>>>"first":1,
-->>>>"second":1,
-->>>>"third":1
-->>},
-->>{
-->>>>"first":2,
-->>>>"second":2,
-->>>>"third":2
-->>},
-->>{
-->>>>"first":3,
-->>>>"second":3,
-->>>>"third":3
-->>}
--])'") == 0);
}/*}}}*/

void test_dynamic_type_array()
{/*{{{*/
  ccl::array<char> buffer;

  // - array init -
  ccl::array<std::string> array_0;
  cassert(array_0.size() == 0 && array_0.used() == 0);

  // - array push -
  const std::array<const char *,4> data = {"One","Two","Three","Four"};
  do {
    array_0.push(data[array_0.used()]);
  } while(array_0.used() < sizeof(data)/sizeof(data[0]));
  buffer.debug(array_0);
  cassert(array_0.used() == 4 && strcmp(buffer.data(),"[One,Two,Three,Four]") == 0);

  // - array push_blank -
  ccl::array<std::string> array_1;
  do {
    array_1.push_blank();
    array_1.last() = data[4 - array_1.used()];
  } while(array_1.used() < sizeof(data)/sizeof(data[0]));
  buffer.debug(array_1);
  cassert(array_1.used() == 4 && strcmp(buffer.data(),"[Four,Three,Two,One]") == 0);

  // - array copy_resize -
  array_0.copy_resize(10);
  buffer.debug(array_0);
  cassert(array_0.size() == 10 && array_0.used() == 4 &&
    strcmp(buffer.data(),"[One,Two,Three,Four]") == 0);

  // - array flush -
  array_0.flush();
  buffer.debug(array_0);
  cassert(array_0.size() == 4 && array_0.used() == 4 &&
    strcmp(buffer.data(),"[One,Two,Three,Four]") == 0);

  // - array clear -
  // - array swap -
  array_1.clear();
  buffer.debug(array_1);
  cassert(array_0.size() == 4 && array_0.used() == 4 && array_1.used() == 0 &&
    strcmp(buffer.data(),"[]") == 0);
  array_0.swap(array_1);
  buffer.debug(array_1);
  cassert(array_0.used() == 0 && array_1.size() == 4 && array_1.used() == 4 &&
    strcmp(buffer.data(),"[One,Two,Three,Four]") == 0);

  // - array at -
  cassert(strcmp(array_1[2].data(),"Three") == 0);

  // - array push -
  // - array pop -
  while (array_1.used() != 0)
  {
    array_0.push(array_1.pop());
  }
  buffer.debug(array_0);
  cassert(array_0.used() == 4 && array_1.used() == 0 &&
    strcmp(buffer.data(),"[Four,Three,Two,One]") == 0);

  // - array last -
  cassert(strcmp(array_0.last().data(),"One") == 0);

  // - array fill -
  array_0.used() = 0;
  array_0.copy_resize(3);
  array_0.fill("Hello world!");
  buffer.debug(array_0);
  cassert(array_0.used() == 3 && array_0.size() == 3 &&
    strcmp(buffer.data(),"[Hello world!,Hello world!,Hello world!]") == 0);

  // - array push_blank -
  uint32_t idx = 0;
  do {
    array_1.push_blank();
    array_1.last() = string_format("idx: %u",idx);
  } while(++idx < 3);
  buffer.debug(array_1);
  cassert(strcmp(buffer.data(),"[idx: 0,idx: 1,idx: 2]") == 0);

  // - array get_idx -
  cassert(array_1.get_idx(array_1[1]) == 1);

  // - array operator= -
  // - array operator== -
  cassert(!(array_0 == array_1));
  array_0 = array_1;
  buffer.debug(array_0);
  cassert(array_0 == array_1 &&
    strcmp(buffer.data(),"[idx: 0,idx: 1,idx: 2]") == 0);

  // - array initializer_list -
  ccl::array<std::string> array_3{"Hello","there","world!"};
  cassert(strcmp(buffer.debug(array_3),"[Hello,there,world!]") == 0);

  // - array to_json -
  buffer.reset().json(ccl::array<std::string>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(array_3).push('\0');
  cassert(strcmp(buffer.data(),R"'(["Hello","there","world!"])'") == 0);

  // - array to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::array<std::string>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(array_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>"Hello",
-->>"there",
-->>"world!"
--])'") == 0);
}/*}}}*/

void test_basic_type_list()
{/*{{{*/
  ccl::array<char> buffer;

  // - list init -
  ccl::list<uint32_t> list_0;
  cassert(list_0.size() == 0 && list_0.used() == 0 && list_0.data() == nullptr);

  // - list init_size -
  ccl::list<uint32_t> list_1(100);
  cassert(list_1.size() == 100 && list_1.used() == 0 && list_1.data() != nullptr);

  // - list prepend -
  uint32_t idx = 0;
  do {
    list_0.prepend(idx);
  } while(++idx < 10);

  buffer.debug(list_0);
  cassert(strcmp(buffer.data(),"[9,8,7,6,5,4,3,2,1,0]") == 0);

  // - list reset -
  list_1 = list_0;
  cassert(list_1.count() == 10);
  list_1.reset();
  cassert(list_1.count() == 0);

  // - list reserve -
  list_0.reserve(100);
  cassert(list_0.count() == 10 && list_0.size() >= 100);

  // - list append -
  idx = 0;
  do {
    list_1.append(idx);
  } while(++idx < 10);

  buffer.debug(list_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,5,6,7,8,9]") == 0);

  // - list insert_before -
  idx = 0;
  do {
    list_1.insert_before(5,10 + idx);
  } while(++idx < 3);

  buffer.debug(list_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,10,11,12,5,6,7,8,9]") == 0);

  // - list insert_after -
  idx = 0;
  do {
    list_1.insert_after(5,20 + idx);
  } while(++idx < 3);

  buffer.debug(list_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,10,11,12,5,22,21,20,6,7,8,9]") == 0);

  // - list get_idx -
  // - list remove -
  uint32_t remove_idx = list_1.get_idx(21);
  idx = 0;
  do {
    uint32_t next_idx = list_1.next_idx(remove_idx);
    list_1.remove(remove_idx);

    remove_idx = next_idx;
  } while(++idx < 3);

  buffer.debug(list_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,10,11,12,5,22,7,8,9]") == 0);

  // - list copy_resize -
  list_0.copy_resize(15);
  buffer.debug(list_0);
  cassert(list_0.count() == 10 && list_0.size() == 15 &&
      strcmp(buffer.data(),"[9,8,7,6,5,4,3,2,1,0]") == 0);

  // - list compare -
  cassert(list_0.compare(list_1) == -1);
  cassert(list_1.compare(list_0) == 1);

  ccl::list<uint32_t> list_2(list_1);
  cassert(list_1.compare(list_2) == 0);

  list_2[list_2.last_idx()] = 10;
  cassert(list_1.compare(list_2) == -1);
  cassert(list_2.compare(list_1) == 1);
  cassert(strcmp(buffer.debug(list_2),"[0,1,2,3,4,10,11,12,5,22,7,8,10]") == 0);

  // - list operator== -
  // - list operator= -
  cassert(!(list_0 == list_1));
  list_0 = list_1;
  cassert(list_0 == list_1);

  // - list initializer_list -
  ccl::list<uint32_t> list_3{1,2,3,4,5};
  cassert(strcmp(buffer.debug(list_3),"[1,2,3,4,5]") == 0);

  // - list to_json -
  buffer.reset().json(ccl::list<uint32_t>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(list_3).push('\0');
  cassert(strcmp(buffer.data(),"[1,2,3,4,5]") == 0);

  // - list to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::list<uint32_t>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(list_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>1,
-->>2,
-->>3,
-->>4,
-->>5
--])'") == 0);
}/*}}}*/

void test_basic_type_queue()
{/*{{{*/
  ccl::array<char> buffer;

  // - queue init -
  ccl::queue<uint32_t> queue_0;
  cassert(queue_0.size() == 0 && queue_0.used() == 0 && queue_0.data() == nullptr);

  // - queue insert -
  uint32_t idx = 0;
  do {
    queue_0.insert(idx);
  } while(++idx < 10);

  buffer.debug(queue_0);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,5,6,7,8,9]") == 0);

  // - queue reset -
  ccl::queue<uint32_t> queue_1;
  queue_1 = queue_0;
  cassert(queue_1.count() == 10);
  queue_1.reset();
  cassert(queue_1.count() == 0);

  // - queue reserve -
  queue_0.reserve(100);
  cassert(queue_0.used() == 10 && queue_0.size() >= 100);

  // - queue next -
  idx = 0;
  do {
    queue_1.insert(queue_0.next());
  } while(++idx < 5);

  buffer.debug(queue_0);
  cassert(strcmp(buffer.data(),"[5,6,7,8,9]") == 0);

  buffer.debug(queue_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4]") == 0);

  // - queue operator== -
  // - queue compare -
  queue_0.copy_resize(29);
  cassert(!(queue_0 == queue_1));

  ccl::queue<uint32_t> queue_2(queue_0);
  cassert(queue_0.compare(queue_2) == 0);
  cassert(queue_0.compare(queue_1) == 1);
  cassert(queue_1.compare(queue_0) == -1);

  idx = 0;
  do {
    queue_0.next();
    queue_0.insert(idx);
  } while(++idx < 5);

  idx = 0;
  do {
    queue_0.insert(idx);
    queue_1.insert(idx);
  } while(++idx < 24);

  cassert(queue_0 == queue_0);
  cassert(queue_0 == queue_1);
  cassert(queue_1 == queue_0);

  cassert(queue_0.compare(queue_0) == 0);
  cassert(queue_0.compare(queue_1) == 0);
  cassert(queue_1.compare(queue_0) == 0);

  buffer.debug(queue_0);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23]") == 0);

  buffer.debug(queue_1);
  cassert(strcmp(buffer.data(),"[0,1,2,3,4,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23]") == 0);

  // - queue initializer_list -
  ccl::queue<uint32_t> queue_3{1,2,3,4,5};
  cassert(strcmp(buffer.debug(queue_3),"[1,2,3,4,5]") == 0);

  // - queue to_json -
  buffer.reset().json(ccl::queue<uint32_t>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(queue_3).push('\0');
  cassert(strcmp(buffer.data(),"[1,2,3,4,5]") == 0);

  // - queue to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::queue<uint32_t>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(queue_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>1,
-->>2,
-->>3,
-->>4,
-->>5
--])'") == 0);
}/*}}}*/

void test_basic_type_tree()
{/*{{{*/
  ccl::array<char> buffer;

  // - tree init -
  ccl::tree<uint32_t> tree_0;
  cassert(tree_0.size() == 0 && tree_0.used() == 0 && tree_0.data() == nullptr);

  ccl::tree<uint32_t> tree_1;

  // - tree reserve -
  tree_1.reserve(100);
  cassert(tree_1.used() == 0 && tree_1.count() == 0 && tree_1.size() >= 101);

  uint32_t tree_size = tree_1.size();

  uint32_t idx = 0;
  uint32_t idx_end = idx + tree_1.size() - 1;
  do {
    cassert(tree_1.insert(idx) == idx + 1);
  } while(++idx < idx_end);

  cassert(tree_1.size() == tree_size && tree_1.count() == tree_size - 1);

  // - tree reset -
  tree_1.reset();
  cassert(tree_1.count() == 0 && tree_1.size() == tree_size);

  // - tree insert -
  srand(13); // NOLINT(cert-msc32-c,cert-msc51-cpp)

  idx = 0;
  do {
    tree_0.insert(rand()%1000); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < 10);

  buffer.debug(tree_0);
  cassert(strcmp(buffer.data(),"[24,51,279,329,381,432,475,764,890,991]") == 0);

  // - tree traverse min to max -
  buffer.used() = 0;

  idx = tree_0.min_idx();
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.next_idx(idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"24,51,279,329,381,432,475,764,890,991,") == 0);

  // - tree traverse stack min to max -
  buffer.used() = 0;

  auto stack = tree_0.descent_stack();
  idx = tree_0.stack_min_idx(&stack);
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.stack_next_idx(&stack,idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"24,51,279,329,381,432,475,764,890,991,") == 0);

  // - tree traverse data -
  buffer.used() = 0;

  idx = 0;
  do {
    auto node = tree_0.data()[idx];
    if (node.is_valid())
    {
      buffer.string(node.object()).push(',');
    }
  } while(++idx < tree_0.used());

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"890,381,279,991,764,24,432,475,329,51,") == 0);

  // - tree unique_insert -
  idx = 0;
  do {
    tree_1.unique_insert(rand() % 100); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < 100);

  cassert(tree_1.count() == 67);

  // - tree remove -
  idx = 1;
  do {
    tree_1.remove(idx);
  } while(++idx < 50);

  buffer.debug(tree_1);
  cassert(strcmp(buffer.data(),"[1,10,17,23,24,30,44,47,53,59,61,66,70,72,75,83,89,90]") == 0);

  // - tree get_idx -
  tree_1.remove(tree_1.get_idx(10));
  tree_1.remove(tree_1.get_idx(44));
  tree_1.remove(tree_1.get_idx(75));

  buffer.debug(tree_1);
  cassert(strcmp(buffer.data(),"[1,17,23,24,30,47,53,59,61,66,70,72,83,89,90]") == 0);

  // - tree get_idx_left -
  tree_1.insert(66);
  tree_1.insert(66);
  tree_1.insert(66);
  tree_1.insert(66);

  idx = tree_1.get_idx_left(66);
  uint32_t count = 0;
  do {
    cassert(tree_1[idx] == 66);
    idx = tree_1.next_idx(idx);
  } while(++count < 5);

  cassert(tree_1[idx] != 66);

  // - tree get_gre_idx -
  cassert(tree_1[tree_1.get_gre_idx(23)] == 23);
  cassert(tree_1[tree_1.get_gre_idx(62)] == 66);
  cassert(tree_1[tree_1.get_gre_idx(82)] == 83);

  // - tree get_lee_idx -
  cassert(tree_1[tree_1.get_lee_idx(23)] == 23);
  cassert(tree_1[tree_1.get_lee_idx(62)] == 61);
  cassert(tree_1[tree_1.get_lee_idx(82)] == 72);

  // - tree get_idxs -
  ccl::array<uint32_t> idxs;
  tree_1.get_idxs(66,&idxs);
  buffer.debug(idxs);
  cassert(strcmp(buffer.data(),"[56,64,54,49,65]") == 0);

  // - tree operator== -
  // - tree operator= -
  cassert(!(tree_0 == tree_1));
  cassert(strcmp(buffer.debug(tree_0),
        "[24,51,279,329,381,432,475,764,890,991]") == 0);
  tree_0 = tree_1;
  cassert(tree_0 == tree_1);
  cassert(strcmp(buffer.debug(tree_0),
        "[1,17,23,24,30,47,53,59,61,66,66,66,66,66,70,72,83,89,90]") == 0);

  // - tree index type -
  ccl::tree<uint32_t,uint16_t> tree_2;

  idx = 0;
  do {
    tree_2.insert(idx);
  } while(++idx < 10);

  cassert(tree_2.get_idx(5) != decltype(tree_2)::idx_not_exist);
  cassert(tree_2.get_idx(100) == decltype(tree_2)::idx_not_exist);

  // - tree initializer_list -
  ccl::tree<uint32_t> tree_3{1,2,3,4,5};
  cassert(strcmp(buffer.debug(tree_3),"[1,2,3,4,5]") == 0);

  // - tree to_json -
  buffer.reset().json(ccl::tree<uint32_t>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(tree_3).push('\0');
  cassert(strcmp(buffer.data(),"[1,2,3,4,5]") == 0);

  // - tree to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::tree<uint32_t>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(tree_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>1,
-->>2,
-->>3,
-->>4,
-->>5
--])'") == 0);
}/*}}}*/

void test_static_type_tree()
{/*{{{*/
  ccl::array<char> buffer;

  // - tree init -
  ccl::tree<static_s> tree_0;
  cassert(tree_0.size() == 0 && tree_0.used() == 0 && tree_0.data() == nullptr);

  // - tree insert -
  srand(13); // NOLINT(cert-msc32-c,cert-msc51-cpp)

  uint32_t idx = 0;
  do {
    tree_0.insert({
      static_cast<uint32_t>(rand()%1000), // NOLINT(cert-msc30-c,cert-msc50-cpp)
      static_cast<uint16_t>(rand()%1000), // NOLINT(cert-msc30-c,cert-msc50-cpp)
      static_cast<uint32_t>(rand()%1000)}); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < 4);

  buffer.debug(tree_0);
  cassert(strcmp(buffer.data(),"[{first:51,second:126,third:379},{first:432,second:475,third:329},{first:890,second:381,third:279},{first:991,second:764,third:24}]") == 0);

  // - tree traverse min to max -
  buffer.used() = 0;

  idx = tree_0.min_idx();
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.next_idx(idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"{first:51,second:126,third:379},{first:432,second:475,third:329},{first:890,second:381,third:279},{first:991,second:764,third:24},") == 0);

  // - tree traverse stack min to max -
  buffer.used() = 0;

  auto stack = tree_0.descent_stack();
  idx = tree_0.stack_min_idx(&stack);
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.stack_next_idx(&stack,idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"{first:51,second:126,third:379},{first:432,second:475,third:329},{first:890,second:381,third:279},{first:991,second:764,third:24},") == 0);

  // - tree traverse data -
  buffer.used() = 0;

  idx = 0;
  do {
    auto node = tree_0.data()[idx];
    if (node.is_valid())
    {
      buffer.string(node.object()).push(',');
    }
  } while(++idx < tree_0.used());

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"{first:890,second:381,third:279},{first:991,second:764,third:24},{first:432,second:475,third:329},{first:51,second:126,third:379},") == 0);

  // - tree unique_insert -
  ccl::tree<static_s> tree_1;

  idx = 0;
  do {
    tree_1.unique_insert({
      static_cast<uint32_t>(rand()%1000), // NOLINT(cert-msc30-c,cert-msc50-cpp)
      static_cast<uint16_t>(rand()%1000), // NOLINT(cert-msc30-c,cert-msc50-cpp)
      static_cast<uint32_t>(rand()%1000)}); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < 1000);

  cassert(tree_1.count() == 624);

  // - tree initializer_list -
  ccl::tree<static_s> tree_3{
    {1,1,1},
    {2,2,2},
    {3,3,3},
  };
  cassert(strcmp(buffer.debug(tree_3),"[{first:1,second:1,third:1},{first:2,second:2,third:2},{first:3,second:3,third:3}]") == 0);

  // - tree to_json -
  buffer.reset().json(ccl::tree<static_s>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(tree_3).push('\0');
  cassert(strcmp(buffer.data(),
R"'([{"first":1,"second":1,"third":1},{"first":2,"second":2,"third":2},{"first":3,"second":3,"third":3}])'") == 0);

  // - tree to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::tree<static_s>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(tree_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>{
-->>>>"first":1,
-->>>>"second":1,
-->>>>"third":1
-->>},
-->>{
-->>>>"first":2,
-->>>>"second":2,
-->>>>"third":2
-->>},
-->>{
-->>>>"first":3,
-->>>>"second":3,
-->>>>"third":3
-->>}
--])'") == 0);
}/*}}}*/

void test_dynamic_type_tree()
{/*{{{*/
  ccl::array<char> buffer;

  // - tree init -
  ccl::tree<std::string> tree_0;
  cassert(tree_0.size() == 0 && tree_0.used() == 0 && tree_0.data() == nullptr);

  tree_0.insert("one");
  tree_0.insert("two");
  tree_0.insert("three");
  tree_0.insert("four");
  tree_0.insert("five");

  buffer.debug(tree_0);
  cassert(strcmp(buffer.data(),"[five,four,one,three,two]") == 0);

  // - tree traverse min to max -
  buffer.used() = 0;

  uint32_t idx = tree_0.min_idx();
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.next_idx(idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"five,four,one,three,two,") == 0);

  // - tree traverse stack min to max -
  buffer.used() = 0;

  auto stack = tree_0.descent_stack();
  idx = tree_0.stack_min_idx(&stack);
  do {
    buffer.string(tree_0[idx]).push(',');

    idx = tree_0.stack_next_idx(&stack,idx);
  } while(idx != c_idx_not_exist);

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"five,four,one,three,two,") == 0);

  // - tree traverse data -
  buffer.used() = 0;

  idx = 0;
  do {
    auto node = tree_0.data()[idx];
    if (node.is_valid())
    {
      buffer.string(node.object()).push(',');
    }
  } while(++idx < tree_0.used());

  buffer.push('\0');
  cassert(strcmp(buffer.data(),"one,two,three,four,five,") == 0);

  // - tree unique_insert -
  srand(13); // NOLINT(cert-msc32-c,cert-msc51-cpp)

  ccl::tree<std::string> tree_1;

  idx = 0;
  do {
    tree_1.unique_insert(string_format("num(%u)",rand() % 100)); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < 100);

  cassert(tree_1.count() == 65);

  // - tree initializer_list -
  ccl::tree<std::string> tree_3{"Hello","there","my","friend!"};
  cassert(strcmp(buffer.debug(tree_3),"[Hello,friend!,my,there]") == 0);

  // - tree to_json -
  buffer.reset().json(ccl::tree<std::string>()).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json(tree_3).push('\0');
  cassert(strcmp(buffer.data(),R"'(["Hello","friend!","my","there"])'") == 0);

  // - tree to_json_nice -
  ccl::json_nice_s json_nice(">>","--",nullptr);
  buffer.reset().json_nice(ccl::tree<std::string>(),&json_nice).push('\0');
  cassert(strcmp(buffer.data(),"[]") == 0);

  buffer.reset().json_nice(tree_3,&json_nice).push('\0');
  cassert(strcmp(buffer.data(),
R"'([
-->>"Hello",
-->>"friend!",
-->>"my",
-->>"there"
--])'") == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

