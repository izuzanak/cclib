
#ifndef LIBS_LIBTCP_CCL_HEADER_FILES_CCL_TCP_H
#define LIBS_LIBTCP_CCL_HEADER_FILES_CCL_TCP_H

#include "ccl_linux.h"
#include "ccl_openssl.h"
#include "ccl_var.h"

#include <netinet/tcp.h>

namespace ccl {

constexpr int TCP_CONN_READ_ERROR = 1;
constexpr int TCP_CONN_WRITE_ERROR = 2;
constexpr int TCP_CONN_INVALID_FD = 3;
constexpr int TCP_CONN_CONNECT_ERROR = 4;
constexpr int TCP_CONN_CALLBACK_ERROR = 5;

constexpr int TCP_SERVER_SOCKOPT_ERROR = 1;
constexpr int TCP_SERVER_INVALID_FD = 2;
constexpr int TCP_SERVER_ACCEPT_ERROR = 3;

using tcp_conn_event_callback_t = std::function<void(uint32_t a_conn_idx,uint32_t a_event)>;
using tcp_conn_recv_callback_t  = std::function<void(uint32_t a_conn_idx,array<char> *a_message)>;
using tcp_conn_send_callback_t  = std::function<void(uint32_t a_conn_idx)>;

// === definition of structure tcp_conn_s ======================================

struct tcp_server_s;

enum
{/*{{{*/
  c_tcp_EVENT_ERROR = 0,
  c_tcp_EVENT_CONNECTED,
  c_tcp_EVENT_ACCEPTED,
  c_tcp_EVENT_DROPPED,
};/*}}}*/

struct tcp_conn_s
{/*{{{*/
  friend tcp_server_s;

private:
#ifdef CLIB_WITH_OPENSSL
  ssl_conn_s m_ssl;
  uint32_t m_ssl_action = SSL_ACTION_NONE;
#endif
  epoll_fd_s m_epoll_fd; 
  bool m_connecting;
  bool m_connected;

  tcp_conn_event_callback_t m_conn_event_callback;
  tcp_conn_recv_callback_t m_conn_recv_callback;
  tcp_conn_send_callback_t m_conn_send_callback;
  uint32_t m_index = 0;

  array<char> m_in_msg;

  queue<std::shared_ptr<array<char>>> m_out_msg_queue;
  uint32_t m_out_msg_offset = 0;

public:
  ~tcp_conn_s() = default;

  tcp_conn_s() = default;
  tcp_conn_s(const tcp_conn_s &a_src) = delete;
  tcp_conn_s(tcp_conn_s &&a_src) = default;

  tcp_conn_s(epoll_fd_s a_epoll_fd,
      tcp_conn_recv_callback_t a_conn_recv_callback,
      tcp_conn_send_callback_t a_conn_send_callback) :
    m_epoll_fd(std::move(a_epoll_fd)),
    m_connecting(false),m_connected(true),
    m_conn_event_callback(nullptr),
    m_conn_recv_callback(std::move(a_conn_recv_callback)),
    m_conn_send_callback(std::move(a_conn_send_callback)) {}

  tcp_conn_s(const char *a_ip,uint16_t a_port,
      tcp_conn_event_callback_t a_conn_event_callback,
      tcp_conn_recv_callback_t a_conn_recv_callback,
      tcp_conn_send_callback_t a_conn_send_callback,
      uint32_t a_index);

  auto operator=(const tcp_conn_s &a_src) -> tcp_conn_s & = delete;
  auto operator=(tcp_conn_s &&a_src) -> tcp_conn_s & = default;

  auto init_ssl() -> tcp_conn_s &
  {/*{{{*/
    m_ssl = ssl_context_s::client().conn_connect(m_epoll_fd);

    return *this;
  }/*}}}*/

  auto fd_update(uint32_t a_evts,bool a_update_cb = false,fd_callback_t &&a_callback = nullptr) -> tcp_conn_s &
  {/*{{{*/
    m_epoll_fd.fd_update(a_evts,a_update_cb,std::move(a_callback));

    return *this;
  }/*}}}*/

  auto recv_msg() -> tcp_conn_s &;
  auto send_msg() -> tcp_conn_s &;
  auto fd_event(const epoll_event &a_epoll_event) -> void;

  auto schedule_message(std::shared_ptr<array<char>> a_message) -> tcp_conn_s &
  {/*{{{*/
    m_out_msg_queue.insert(std::move(a_message));
    m_epoll_fd.fd_update(EPOLLIN | EPOLLOUT | EPOLLPRI);

    return *this;
  }/*}}}*/

  auto schedule_message(array<char> a_message) -> tcp_conn_s &
  {/*{{{*/
    return schedule_message(std::make_shared<array<char>>(std::move(a_message)));
  }/*}}}*/
};/*}}}*/

// === definition of structure tcp_server_s ====================================

struct tcp_server_s
{/*{{{*/
private:
  std::string m_ip;
  uint16_t m_port = 0;

  tcp_conn_event_callback_t m_conn_event_callback;
  tcp_conn_recv_callback_t m_conn_recv_callback;
  tcp_conn_send_callback_t m_conn_send_callback;

  ssl_context_s m_ssl_ctx;
  epoll_fd_s m_epoll_fd;
  list<tcp_conn_s> m_conn_list;

public:
  ~tcp_server_s() = default;

  tcp_server_s() = default;
  tcp_server_s(const tcp_server_s &a_src) = delete;
  tcp_server_s(tcp_server_s &&a_src) = default;

  tcp_server_s(std::string a_ip,uint16_t a_port,
      tcp_conn_event_callback_t a_conn_event_callback,
      tcp_conn_recv_callback_t a_conn_recv_callback,
      tcp_conn_send_callback_t a_conn_send_callback) :
    m_ip(std::move(a_ip)), m_port(a_port),
    m_conn_event_callback(std::move(a_conn_event_callback)),
    m_conn_recv_callback(std::move(a_conn_recv_callback)),
    m_conn_send_callback(std::move(a_conn_send_callback)),
    m_epoll_fd(fd_s::sock(AF_INET,SOCK_STREAM))
  {/*{{{*/
    m_epoll_fd.fd().sock_listen({m_ip.data(),m_port},256);

    int yes = 1;
    if (setsockopt(m_epoll_fd,SOL_TCP,TCP_NODELAY,&yes,sizeof(int)) != 0)
    {
      throw_error(TCP_SERVER_SOCKOPT_ERROR);
    }
  }/*}}}*/

  auto operator=(const tcp_server_s &a_src) -> tcp_server_s & = delete;
  auto operator=(tcp_server_s &&a_src) -> tcp_server_s & = default;

  auto init_ssl(const char *a_cert_file,const char *a_pkey_file) -> tcp_server_s &
  {/*{{{*/
    m_ssl_ctx = ssl_context_s::server();
    m_ssl_ctx
      .use_certificate_file(a_cert_file,SSL_FILETYPE_PEM)
      .use_private_key_file(a_pkey_file,SSL_FILETYPE_PEM);

    return *this;
  }/*}}}*/

  auto fd_update(uint32_t a_evts,bool a_update_cb = false,fd_callback_t &&a_callback = nullptr) -> tcp_server_s &
  {/*{{{*/
    m_epoll_fd.fd_update(a_evts,a_update_cb,std::move(a_callback));

    return *this;
  }/*}}}*/

  auto conn(uint32_t a_conn_idx) -> tcp_conn_s & { return m_conn_list[a_conn_idx]; }

  auto fd_event(const epoll_event &a_epoll_event) -> void;
  auto conn_fd_event(uint32_t a_conn_idx,const epoll_event &a_epoll_event) -> void;
};/*}}}*/

} // namespace ccl

#endif

