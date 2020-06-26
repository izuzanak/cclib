
#include "main.h"

namespace ccl {

// === members of structure tcp_comm_s =========================================

bool tcp_comm_s::m_terminate = false;
epoll_s *g_epoll = nullptr;

// === methods of structure tcp_comm_s =========================================

tcp_comm_s::tcp_comm_s(std::string a_address,uint16_t a_port) :
  m_server(std::move(a_address),a_port,
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx,uint32_t a_event) -> void {
      pointer_s::ptr<tcp_comm_s>(ptr_index)->conn_event(a_conn_idx,a_event); },
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx,array<char> *a_message) -> void {
      pointer_s::ptr<tcp_comm_s>(ptr_index)->conn_recv(a_conn_idx,a_message); },
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx) -> void {
      pointer_s::ptr<tcp_comm_s>(ptr_index)->conn_send(a_conn_idx); })
{/*{{{*/
  process_s::simple_signal_handler(term_signal);

  m_server.fd_update(EPOLLIN | EPOLLPRI,true,
  [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
    pointer_s::ptr<tcp_comm_s>(ptr_index)->m_server.fd_event(a_epoll_event);
  });

  //m_server.init_ssl(
  //  "tests/libtcp_ccl_test/resources/mycert.pem",
  //  "tests/libtcp_ccl_test/resources/mycert.pem");
}/*}}}*/

auto tcp_comm_s::run() -> tcp_comm_s &
{/*{{{*/
  while(!m_terminate)
  {
    // - wait on events -
    g_epoll->wait(-1);
  }

  return *this;
}/*}}}*/

auto tcp_comm_s::conn_event(uint32_t a_conn_idx,uint32_t a_event) -> void
{/*{{{*/
  (void)this;
  (void)a_conn_idx;

  fprintf(stderr,"tcp_comm_s::conn_event: %u - %u\n",a_conn_idx,a_event);
}/*}}}*/

auto tcp_comm_s::conn_recv(uint32_t a_conn_idx,array<char> *a_message) -> void
{/*{{{*/
  fprintf(stderr,"tcp_comm_s::conn_recv: %u - %.*s\n",a_conn_idx,a_message->used(),a_message->data());

  m_buffer
    .reset()
    .append("RESPONSE: ")
    .append(a_message->used(),a_message->data());

  // - clear message buffer -
  a_message->used() = 0;

  m_server.conn(a_conn_idx).schedule_message(std::move(m_buffer));
}/*}}}*/

auto tcp_comm_s::conn_send(uint32_t a_conn_idx) -> void
{/*{{{*/
  (void)this;
  (void)a_conn_idx;

  fprintf(stderr,"tcp_comm_s::conn_send: %u\n",a_conn_idx);
}/*}}}*/

// === global functions ========================================================

auto fd_update(int a_fd,unsigned a_evts,bool a_update_cb,fd_callback_t &&a_callback) -> void
{/*{{{*/
  g_epoll->__fd_update(a_fd,a_evts,a_update_cb,std::move(a_callback));
}/*}}}*/

} // namespace ccl

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::epoll_s epoll(0);

  ccl::g_fd_update = ccl::fd_update;
  ccl::g_epoll = &epoll;

  ccl::tcp_comm_s comm("127.0.0.1",8001);
  comm.run();

  return 0;
}/*}}}*/

