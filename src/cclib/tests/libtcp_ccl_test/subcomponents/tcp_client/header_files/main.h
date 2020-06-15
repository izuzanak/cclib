
#ifndef TESTS_LIBTCP_CCL_TEST_SUBCOMPONENTS_TCP_CLIENT_HEADER_FILES_MAIN_H
#define TESTS_LIBTCP_CCL_TEST_SUBCOMPONENTS_TCP_CLIENT_HEADER_FILES_MAIN_H

#include "ccl_process.h"
#include "ccl_tcp.h"

namespace ccl {

// === definition of structure tcp_client_s ====================================

struct tcp_client_s
{/*{{{*/
private:
  std::string m_ip;
  uint16_t m_port;
  tcp_conn_s m_conn;
  epoll_fd_s m_timer;

public:
  auto conn() -> tcp_conn_s & { return m_conn; }
  auto timer() -> epoll_fd_s & { return m_timer; }

  ~tcp_client_s() = default;

  tcp_client_s() = default;
  tcp_client_s(const tcp_client_s &a_src) = delete;
  tcp_client_s(tcp_client_s &&a_src) = default;

  tcp_client_s(std::string a_ip,uint16_t a_port,
      tcp_conn_event_callback_t a_conn_event_callback,
      tcp_conn_recv_callback_t a_conn_recv_callback,
      tcp_conn_send_callback_t a_conn_send_callback,
      uint32_t a_index) :
    m_ip(std::move(a_ip)),m_port(a_port),
    m_conn(m_ip.data(),m_port,
    std::move(a_conn_event_callback),
    std::move(a_conn_recv_callback),
    std::move(a_conn_send_callback),
    a_index),
    m_timer(fd_s::timer())
  {}

  auto operator=(const tcp_client_s &a_src) -> tcp_client_s & = delete;
  auto operator=(tcp_client_s &&a_src) -> tcp_client_s & = default;

  auto timer_set(const itimerspec &a_itimerspec,int a_flags) -> tcp_client_s &
  {/*{{{*/
    m_timer.fd().timer_set(a_itimerspec,a_flags);

    return *this;
  }/*}}}*/
};/*}}}*/

// === definition of structure tcp_comm_s ======================================

struct tcp_comm_s
{/*{{{*/
private:
  static bool m_terminate;

  tcp_client_s m_client;
  array<char> m_buffer;

public:
  ~tcp_comm_s() = default;
  tcp_comm_s(std::string a_ip,uint16_t a_port);

  auto conn_event(uint32_t a_conn_idx,uint32_t a_event) -> void;
  auto conn_recv(uint32_t a_conn_idx,array<char> *a_message) -> void;
  auto conn_send(uint32_t a_conn_idx) -> void;

  auto timer_event(const epoll_event &a_epoll_event) -> void;

  static auto term_signal(int a_signal) -> void
  {/*{{{*/
    (void)a_signal;

    m_terminate = true;
  }/*}}}*/

  auto run() -> tcp_comm_s &;
};/*}}}*/

} // namespace ccl

#endif

