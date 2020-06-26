
#ifndef TESTS_LIBTCP_CCL_TEST_SUBCOMPONENTS_TCP_SERVER_HEADER_FILES_MAIN_H
#define TESTS_LIBTCP_CCL_TEST_SUBCOMPONENTS_TCP_SERVER_HEADER_FILES_MAIN_H

#include "ccl_process.h"
#include "ccl_tcp.h"

namespace ccl {

// === definition of structure tcp_comm_s ======================================

struct tcp_comm_s
{/*{{{*/
private:
  pointer_s m_pointer;
  static bool m_terminate;

  tcp_server_s m_server;
  array<char> m_buffer;

public:
  ~tcp_comm_s() = default;
  tcp_comm_s(std::string a_address,uint16_t a_port);

  auto conn_event(uint32_t a_conn_idx,uint32_t a_event) -> void;
  auto conn_recv(uint32_t a_conn_idx,array<char> *a_message) -> void;
  auto conn_send(uint32_t a_conn_idx) -> void;

  static auto term_signal(int a_signal) -> void
  {/*{{{*/
    (void)a_signal;

    m_terminate = true;
  }/*}}}*/

  auto run() -> tcp_comm_s &;
};/*}}}*/

} // namespace ccl

#endif

