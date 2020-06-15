
#include "ccl_sys.h"

namespace ccl {

// === methods of structure file_s =============================================

auto file_s::read(array<char> *a_trg) -> file_s &
{/*{{{*/
  debug_assert(m_file != nullptr);

  const uint32_t c_buffer_add = 1024;

  uint32_t read_cnt;
  do
  {
    uint32_t old_used = a_trg->used();
    a_trg->push_blanks(c_buffer_add);
    read_cnt = fread(a_trg->data() + old_used,1,c_buffer_add,m_file);
  }
  while(read_cnt >= c_buffer_add);

  if (ferror(m_file) != 0)
  {
    throw_error(FILE_READ_ERROR);
  }

  a_trg->used() = (a_trg->used() - c_buffer_add) + read_cnt;

  return *this;
}/*}}}*/

auto file_s::read_cnt(array<char> *a_trg,size_t a_size) -> file_s &
{/*{{{*/
  debug_assert(m_file != nullptr);

  a_trg->reserve(a_size);

  if (fread(a_trg->data() + a_trg->used(),1,a_size,m_file) != a_size)
  {
    throw_error(FILE_READ_ERROR);
  }

  a_trg->used() += a_size;

  return *this;
}/*}}}*/

} // namespace ccl

