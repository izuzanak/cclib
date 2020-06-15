
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "performance_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "array",
  "tree",
  "var",
  "json",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_array,
  test_tree,
  test_var,
  test_json,
};/*}}}*/

// === test execution functions ================================================

#include <vector>

void test_array()
{/*{{{*/
  ccl::time_s time_start;

#if OPTION_PERFORMANCE_TEST == ENABLED
  constexpr unsigned create_array_uint_size   = 10000000;
  constexpr unsigned create_array_string_size = 100000;
  constexpr unsigned create_array_array_size  = 1000000;
#else
  constexpr unsigned create_array_uint_size   = 100;
  constexpr unsigned create_array_string_size = 100;
  constexpr unsigned create_array_array_size  = 100;
#endif

  ccl::array<uint32_t> array_0;

  {
    time_start = ccl::time_s::now();

    uint32_t idx = 0;
    do {
      array_0.push(idx);
    } while(++idx < create_array_uint_size);

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::array<uint32_t>",array_0.used(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  std::vector<uint32_t> vector_0;

  {
    time_start = ccl::time_s::now();

    uint32_t idx = 0;
    do {
      vector_0.push_back(idx);
    } while(++idx < create_array_uint_size);

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::vector<uint32_t>",vector_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  {
    time_start = ccl::time_s::now();

    uint64_t sum = 0;
    uint32_t idx = 0;
    do {
      sum += array_0[idx];
    } while(++idx < create_array_uint_size);

    fprintf(stdout,"SUM     %-26.26s %14.llu in %10.3f msec ...\n",
        "ccl::array<uint32_t>",sum,(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    time_start = ccl::time_s::now();

    uint64_t sum = 0;
    uint32_t idx = 0;
    do {
      sum += vector_0[idx];
    } while(++idx < create_array_uint_size);

    fprintf(stdout,"SUM     %-26.26s %14.llu in %10.3f msec ...\n",
        "std::vector<uint32_t>",sum,(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  ccl::array<std::string> array_1;
  std::vector<std::string> vector_1;

  {
    time_start = ccl::time_s::now();

    uint32_t idx = 0;
    do {
      array_1.push(string_format("str(%u)",idx));
    } while(++idx < create_array_string_size);

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::array<std::string>",array_1.used(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    time_start = ccl::time_s::now();

    uint32_t idx = 0;
    do {
      vector_1.push_back(string_format("str(%u)",idx));
    } while(++idx < create_array_string_size);

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::vector<std::string>",vector_1.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  {
    time_start = ccl::time_s::now();

    ccl::array<std::string> array_2;
    while (array_1.used() > 0)
    {
      array_2.push(std::move(array_1.pop()));
    }

    fprintf(stdout,"MOVED   %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::array<std::string>",array_2.used(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    time_start = ccl::time_s::now();

    std::vector<std::string> vector_2;
    while (!vector_1.empty())
    {
      vector_2.push_back(std::move(vector_1.back()));
      vector_1.pop_back();
    }

    fprintf(stdout,"MOVED   %-30.30s %10.zu in %10.3f msec ...\n",
        "std::vector<std::string>",vector_2.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  ccl::array<uint32_t> array{1,2,3,4,5,6,7,8,9,10};
  ccl::array<ccl::array<uint32_t>> array_3;

  {
    time_start = ccl::time_s::now();
    uint32_t idx = 0;
    do {
      array_3.push(array);
    } while(++idx < create_array_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::array<ccl::array<uint32_t>>",array_3.used(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  std::vector<ccl::array<uint32_t>> vector_3;
  {
    time_start = ccl::time_s::now();
    uint32_t idx = 0;
    do {
      vector_3.push_back(array);
    } while(++idx < create_array_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::vector<ccl::array<uint32_t>>",vector_3.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  time_start = ccl::time_s::now();
  ccl::array<ccl::array<uint32_t>> array_4 = array_3;
  fprintf(stdout,"COPIED  %-30.30s %10.u in %10.3f msec ...\n",
      "ccl::array<ccl::array<uint32_t>>",array_4.used(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);

  time_start = ccl::time_s::now();
  std::vector<ccl::array<uint32_t>> vector_4 = vector_3;
  fprintf(stdout,"COPIED  %-30.30s %10.zu in %10.3f msec ...\n",
      "std::vector<ccl::array<uint32_t>>",vector_4.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);

  putc('\n',stdout);
}/*}}}*/

#include <set>
#include <unordered_set>

void test_tree()
{/*{{{*/
  ccl::time_s time_start;

#if OPTION_PERFORMANCE_TEST == ENABLED
  constexpr unsigned create_tree_uint_size  = 1000000;
  constexpr unsigned create_tree_array_size = 500000;
#else
  constexpr unsigned create_tree_uint_size  = 100;
  constexpr unsigned create_tree_array_size = 100;
#endif

  ccl::array<uint32_t> rand_array;
  uint32_t idx = 0;
  do {
    rand_array.push(rand() % 10000); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  } while(++idx < create_tree_uint_size);

  {
    ccl::tree<uint32_t> tree_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      tree_0.unique_insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::tree<uint32_t>",tree_0.count(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    std::set<uint32_t> set_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      set_0.insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::set<uint32_t>",set_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    std::unordered_set<uint32_t> set_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      set_0.insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::unordered_set<uint32_t>",set_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  {
    ccl::tree<uint32_t> tree_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      tree_0.insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::tree<uint32_t>",tree_0.count(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    std::multiset<uint32_t> multiset_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      multiset_0.insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::multiset<uint32_t>",multiset_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    std::unordered_multiset<uint32_t> multiset_0;

    time_start = ccl::time_s::now();
    idx = 0;
    do {
      multiset_0.insert(rand_array[idx]);
    } while(++idx < rand_array.used());

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::unordered_multiset<uint32_t>",multiset_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);

  ccl::array<uint32_t> array{1,2,3,4,5,6,7,8,9,10};

  ccl::tree<ccl::array<uint32_t>> tree_0;
  std::multiset<ccl::array<uint32_t>> multiset_0;

  {
    time_start = ccl::time_s::now();
    idx = 0;
    do {
      tree_0.insert(array);
    } while(++idx < create_tree_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.u in %10.3f msec ...\n",
        "ccl::tree<array>",tree_0.count(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }
  
  {
    time_start = ccl::time_s::now();
    idx = 0;
    do {
      multiset_0.insert(array);
    } while(++idx < create_tree_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.zu in %10.3f msec ...\n",
        "std::multiset<array>",multiset_0.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);
  
  time_start = ccl::time_s::now();
  ccl::tree<ccl::array<uint32_t>> tree_1 = tree_0;
  fprintf(stdout,"COPIED  %-30.30s %10.u in %10.3f msec ...\n",
      "ccl::tree<array>",tree_1.count(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);

  time_start = ccl::time_s::now();
  std::multiset<ccl::array<uint32_t>> multiset_1 = multiset_0;
    fprintf(stdout,"COPIED  %-30.30s %10.zu in %10.3f msec ...\n",
        "std::multiset<array>",multiset_1.size(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);

  putc('\n',stdout);
}/*}}}*/

void test_var()
{/*{{{*/
  ccl::time_s time_start;

#if OPTION_PERFORMANCE_TEST == ENABLED
  constexpr unsigned create_array_size = 500000;
#else
  constexpr unsigned create_array_size = 100;
#endif

  {
    time_start = ccl::time_s::now();

    ccl::var_s array_0 = ccl::var_s::new_array();
    uint32_t idx = 0;
    do {
      array_0.push(idx);
    } while(++idx < create_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.lld in %10.3f msec ...\n",
        "ccl::var_s array_0",array_0.length(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  {
    time_start = ccl::time_s::now();

    ccl::var_s dict_0 = ccl::var_s::new_dict();
    uint32_t idx = 0;
    do {
      ccl::var_s idx_var = idx;
      dict_0[idx_var] = std::move(idx_var);
    } while(++idx < create_array_size);

    fprintf(stdout,"CREATED %-30.30s %10.lld in %10.3f msec ...\n",
        "ccl::var_s dict_0",dict_0.length(),(ccl::time_s::now().micro() - time_start.micro())/1000.0);
  }

  putc('\n',stdout);
}/*}}}*/

void test_json()
{/*{{{*/
  ccl::time_s time_start;

#if OPTION_PERFORMANCE_TEST == ENABLED
  constexpr const char *c_json_filename = "/home/devel/folder/download/resources/citylots.json";
#else
  constexpr const char *c_json_filename = "compile_commands.json";
#endif

  struct stat st;
  if (stat(c_json_filename,&st) == 0)
  {
    ccl::array<char> buffer(st.st_size);
    ccl::file_s(c_json_filename,"r").read_cnt(&buffer,st.st_size);

    time_start = ccl::time_s::now();
    ccl::var_s obj = ccl::json_parser_s().parse(buffer.used(),buffer.data());
    fprintf(stdout,"PARSED in %llu msec ...\n",ccl::time_s::now().milli() - time_start.milli());

    time_start = ccl::time_s::now();
    buffer.reset().json(obj).push('\0');
    fprintf(stdout,"JSON %u in %llu msec ...\n",
        buffer.used(),ccl::time_s::now().milli() - time_start.milli());

    time_start = ccl::time_s::now();
    ccl::json_nice_s json_nice("  ","",nullptr);
    buffer.reset().json_nice(obj,&json_nice).push('\0');
    fprintf(stdout,"JSON_NICE %u in %llu msec ...\n",
        buffer.used(),ccl::time_s::now().milli() - time_start.milli());
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

