
#ifndef TESTS_LIBGPSD_CCL_TEST_SUBCOMPONENTS_GPSD_CLIENT_HEADER_FILES_MAIN_H
#define TESTS_LIBGPSD_CCL_TEST_SUBCOMPONENTS_GPSD_CLIENT_HEADER_FILES_MAIN_H

#include "ccl_gpsd.h"
#include "ccl_process.h"

namespace ccl {

// === definition of structure gpsd_comm_s =====================================

struct gpsd_comm_s
{/*{{{*/
private:
  pointer_s m_pointer;
  static bool m_terminate;

  gpsd_conn_s m_conn;

public:
  ~gpsd_comm_s() = default;
  gpsd_comm_s(const char *a_ip,uint16_t a_port);

  auto conn_recv(uint32_t a_conn_idx,uint32_t a_type,var_s a_message) -> void;

  static auto term_signal(int a_signal) -> void
  {/*{{{*/
    (void)a_signal;

    m_terminate = true;
  }/*}}}*/

  auto run() -> gpsd_comm_s &;
};/*}}}*/

} // namespace ccl

#endif

