
#ifndef LIBS_LIBOPENSSL_CCL_HEADER_FILES_CCL_OPENSSL_H
#define LIBS_LIBOPENSSL_CCL_HEADER_FILES_CCL_OPENSSL_H

#include "ccl_base.h"

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <sys/ioctl.h>

namespace ccl {

constexpr int SSL_CONTEXT_CREATE_ERROR = 1;
constexpr int SSL_CONTEXT_CERTIFICATE_FILE_ERROR = 2;
constexpr int SSL_CONTEXT_PRIVATE_KEY_FILE_ERROR = 3;

constexpr int SSL_CONN_CREATE_ERROR = 1;
constexpr int SSL_CONN_WRITE_ERROR = 2;
constexpr int SSL_CONN_READ_ERROR = 3;

// - ssl repeated actions -
enum
{
  SSL_ACTION_NONE = 0,
  SSL_ACTION_SEND_MSG,
  SSL_ACTION_RECV_MSG
};

// === definition of structure openssl_init_s ==================================

struct openssl_init_s
{/*{{{*/
public:
  openssl_init_s() noexcept
  {/*{{{*/
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
  }/*}}}*/

  ~openssl_init_s() noexcept
  {/*{{{*/
    EVP_cleanup();
  }/*}}}*/
};/*}}}*/

// === definition of structure ssl_conn_s ======================================

struct ssl_context_s;

struct ssl_conn_s
{/*{{{*/
  friend ssl_context_s;

private:
  SSL *m_conn = nullptr;

public:
  ~ssl_conn_s() noexcept
  {/*{{{*/
    if (m_conn != nullptr)
    {
      SSL_free(m_conn);
    }
  }/*}}}*/

  ssl_conn_s() = default;
  ssl_conn_s(const ssl_conn_s &a_src) = delete;
  ssl_conn_s(ssl_conn_s &&a_src) noexcept : m_conn(a_src.m_conn)
  {/*{{{*/
    a_src.m_conn = nullptr;
  }/*}}}*/

  auto operator=(const ssl_conn_s &a_src) -> ssl_conn_s & = delete;
  auto operator=(ssl_conn_s &&a_src) noexcept -> ssl_conn_s &
  {/*{{{*/
    this->~ssl_conn_s();
    new (this) ssl_conn_s(std::move(a_src));

    return *this;
  }/*}}}*/

  operator SSL *() { return m_conn; }; // NOLINT(google-explicit-constructor)

  auto to_string(array<char> *a_trg) const noexcept -> array<char> &
  {/*{{{*/
    return a_trg->append_format("ssl_conn_s{%p}",m_conn);
  }/*}}}*/

  auto write(int a_size,const void *a_src) -> ssl_conn_s &;
  auto read(int a_fd,array<char> *a_trg) -> ssl_conn_s &;
};/*}}}*/

// === definition of structure ssl_context_s ===================================

struct ssl_context_s
{/*{{{*/
private:
  SSL_CTX *m_ctx = nullptr;

  auto conn(int a_fd) -> ssl_conn_s
  {/*{{{*/
    ssl_conn_s conn;

    if ((conn.m_conn = SSL_new(m_ctx)) == nullptr)
    {
      throw_error(SSL_CONN_CREATE_ERROR);
    }

    if (SSL_set_fd(conn.m_conn,a_fd) != 1)
    {
      throw_error(SSL_CONN_CREATE_ERROR);
    }

    return conn;
  }/*}}}*/

public:
  ~ssl_context_s() noexcept
  {/*{{{*/
    if (m_ctx != nullptr)
    {
      SSL_CTX_free(m_ctx);
    }
  }/*}}}*/

  ssl_context_s() = default;
  ssl_context_s(const ssl_context_s &a_src) = delete;
  ssl_context_s(ssl_context_s &&a_src) noexcept : m_ctx(a_src.m_ctx)
  {/*{{{*/
    a_src.m_ctx = nullptr;
  }/*}}}*/

  static auto server() -> ssl_context_s
  {/*{{{*/
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    const SSL_METHOD *method = TLS_server_method();
#else
    const SSL_METHOD *method = TLSv1_2_server_method();
#endif

    ssl_context_s context;
    if ((context.m_ctx = SSL_CTX_new(method)) == nullptr)
    {
      throw_error(SSL_CONTEXT_CREATE_ERROR);
    }

    return context;
  }/*}}}*/

  static auto client() -> ssl_context_s
  {/*{{{*/
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    const SSL_METHOD *method = TLS_client_method();
#else
    const SSL_METHOD *method = TLSv1_2_client_method();
#endif

    ssl_context_s context;
    if ((context.m_ctx = SSL_CTX_new(method)) == nullptr)
    {
      throw_error(SSL_CONTEXT_CREATE_ERROR);
    }

    return context;
  }/*}}}*/

  auto operator=(const ssl_context_s &a_src) -> ssl_context_s & = delete;
  auto operator=(ssl_context_s &&a_src) noexcept -> ssl_context_s &
  {/*{{{*/
    this->~ssl_context_s();
    new (this) ssl_context_s(std::move(a_src));

    return *this;
  }/*}}}*/

  operator SSL_CTX *() { return m_ctx; }; // NOLINT(google-explicit-constructor)

  auto to_string(array<char> *a_trg) const noexcept -> array<char> &
  {/*{{{*/
    return a_trg->append_format("ssl_context_s{%p}",m_ctx);
  }/*}}}*/

  auto use_certificate_file(const char *a_file_name,int a_file_type) -> ssl_context_s &
  {/*{{{*/
    debug_assert(m_ctx != nullptr);

    if (SSL_CTX_use_certificate_file(m_ctx,a_file_name,a_file_type) != 1)
    {
      throw_error(SSL_CONTEXT_CERTIFICATE_FILE_ERROR);
    }

    return *this;
  }/*}}}*/

  auto use_private_key_file(const char *a_file_name,int a_file_type) -> ssl_context_s &
  {/*{{{*/
    debug_assert(m_ctx != nullptr);

    if (SSL_CTX_use_PrivateKey_file(m_ctx,a_file_name,a_file_type) != 1)
    {
      throw_error(SSL_CONTEXT_PRIVATE_KEY_FILE_ERROR);
    }

    return *this;
  }/*}}}*/

  auto conn_accept(int a_fd) -> ssl_conn_s
  {/*{{{*/
    ssl_conn_s conn = this->conn(a_fd);
    SSL_set_accept_state(conn.m_conn);

    return conn;
  }/*}}}*/

  auto conn_connect(int a_fd) -> ssl_conn_s
  {/*{{{*/
    ssl_conn_s conn = this->conn(a_fd);
    SSL_set_connect_state(conn.m_conn);

    return conn;
  }/*}}}*/
};/*}}}*/

} // namespace ccl

#endif

