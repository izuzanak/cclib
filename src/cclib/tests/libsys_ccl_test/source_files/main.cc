
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libsys_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "file",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_file,
};/*}}}*/

// === test execution functions ================================================

void test_file()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::array<ccl::file_s> file_array;
  file_array.push_blanks(3);
  cassert(strcmp(buffer.debug(file_array),"[file_s{(nil)},file_s{(nil)},file_s{(nil)}]") == 0);

  file_array[0] = ccl::file_s("tests/libsys_ccl_test/file/write.txt","wb");

  // - file write -
  const std::string data("Hello world!");
  file_array[0].write(data.length(),data.data());

  // - file flush -
  file_array[0].flush();

  // - file read -
  // - file close -
  buffer.used() = 0;
  ccl::file_s("tests/libsys_ccl_test/file/write.txt","rb").read(&buffer).close();
  buffer.push('\0');
  cassert(strcmp(buffer.data(),data.data()) == 0);

  // - file read_cnt -
  // - file close -
  buffer.used() = 0;
  ccl::file_s("tests/libsys_ccl_test/file/write.txt","rb").read_cnt(&buffer,5).close();
  buffer.push('\0');
  cassert(strcmp(buffer.data(),"Hello") == 0);

  // - file tell -
  // - file seek -
  ccl::file_s &file = file_array[1] = ccl::file_s("tests/libsys_ccl_test/file/write.txt","rb");
  cassert(file.tell() == 0);

  buffer.used() = 0;
  file.read(&buffer);
  cassert(buffer.used() == data.length() && strncmp(buffer.data(),data.data(),data.length()) == 0);

  file.seek(strlen("Hello "),SEEK_SET);
  buffer.used() = 0;
  file.read(&buffer);
  cassert(buffer.used() == 6 && strncmp(buffer.data(),"world!",6) == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

