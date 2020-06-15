
#include "ccl_openssl.h"

namespace ccl {

openssl_init_s g_openssl;

// === methods of structure ssl_conn_s =========================================

auto ssl_conn_s::write(int a_size,const void *a_src) -> ssl_conn_s &
{/*{{{*/
  debug_assert(m_conn != nullptr);

  if (a_size > 0)
  {
    int cnt;
    ssize_t writed = 0;

    do
    {
      if ((cnt = SSL_write(m_conn,static_cast<const char *>(a_src) + writed,a_size - writed)) <= 0)
      {
        throw_error(SSL_CONN_WRITE_ERROR);
      }
    }
    while((writed += cnt) < a_size);
  }

  return *this;
}/*}}}*/

auto ssl_conn_s::read(int a_fd,array<char> *a_trg) -> ssl_conn_s &
{/*{{{*/
  debug_assert(m_conn != nullptr);

  const int c_buffer_add = 1024;

  int inq_cnt;
  int read_cnt;
  do
  {
    a_trg->reserve(c_buffer_add);
    read_cnt = SSL_read(m_conn,a_trg->data() + a_trg->used(),c_buffer_add);

    // - ERROR -
    if (read_cnt <= 0)
    {
      throw_error(SSL_CONN_READ_ERROR);
    }

    a_trg->used() += read_cnt;

    // - ERROR -
    if (ioctl(a_fd,TIOCINQ,&inq_cnt) == -1)
    {
      throw_error(SSL_CONN_READ_ERROR);
    }
  }
  while(inq_cnt > 0);

  return *this;
}/*}}}*/

} // namespace ccl

