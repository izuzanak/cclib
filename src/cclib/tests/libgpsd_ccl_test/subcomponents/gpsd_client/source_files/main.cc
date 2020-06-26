
#include "main.h"

namespace ccl {

// === members of structure gpsd_comm_s ========================================

bool gpsd_comm_s::m_terminate = false;
epoll_s *g_epoll = nullptr;

// === methods of structure gpsd_comm_s ========================================

gpsd_comm_s::gpsd_comm_s(const char *a_ip,uint16_t a_port) :
  m_conn(a_ip,a_port,
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx,uint32_t a_type,var_s a_message) -> void {
      pointer_s::ptr<gpsd_comm_s>(ptr_index)->conn_recv(a_conn_idx,a_type,std::move(a_message)); },
      0)
{/*{{{*/
  process_s::simple_signal_handler(term_signal);
}/*}}}*/

auto gpsd_comm_s::run() -> gpsd_comm_s &
{/*{{{*/
  while(!m_terminate)
  {
    // - wait on events -
    g_epoll->wait(-1);
  }

  return *this;
}/*}}}*/

auto gpsd_comm_s::conn_recv(uint32_t a_conn_idx,uint32_t a_type,var_s a_message) -> void // NOLINT
{/*{{{*/
  (void)this;
  (void)a_conn_idx;
  (void)a_type;
  (void)a_message;

  fprintf(stderr,"gpsd_comm_s::conn_recv\n");
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

  ccl::gpsd_comm_s comm("10.2.1.163",2947);
  comm.run();

  return 0;
}/*}}}*/

