
#ifndef TESTS_LIBMQTT_CCL_TEST_SUBCOMPONENTS_MQTT_CLIENT_HEADER_FILES_MAIN_H
#define TESTS_LIBMQTT_CCL_TEST_SUBCOMPONENTS_MQTT_CLIENT_HEADER_FILES_MAIN_H

#include "ccl_mqtt.h"
#include "ccl_process.h"

namespace ccl {

// === definition of structure mqtt_comm_s =====================================

struct mqtt_comm_s
{/*{{{*/
private:
  static bool m_terminate;

  bool m_connected = false;
  mqtt_conn_s m_conn;
  epoll_fd_s m_timer;

public:
  ~mqtt_comm_s() = default;
  mqtt_comm_s(std::string a_client_id,const char *a_ip,uint16_t a_port);

  auto conn_event(uint32_t a_conn_idx,uint32_t a_event) -> void;
  auto conn_recv(uint32_t a_conn_idx,
      const mqtt_buff_s &a_topic,const mqtt_buff_s &a_message) -> void;

  auto timer_event(const epoll_event &a_epoll_event) -> void;

  static auto term_signal(int a_signal) -> void
  {/*{{{*/
    (void)a_signal;

    m_terminate = true;
  }/*}}}*/

  auto run() -> mqtt_comm_s &;
};/*}}}*/

} // namespace ccl

#endif

