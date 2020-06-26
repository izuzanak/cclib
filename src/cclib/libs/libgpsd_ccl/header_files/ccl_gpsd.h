
#ifndef LIBS_LIBGPSD_CCL_HEADER_FILES_CCL_GPSD_H
#define LIBS_LIBGPSD_CCL_HEADER_FILES_CCL_GPSD_H

#include "ccl_json.h"
#include "ccl_tcp.h"

namespace ccl {

constexpr int GPSD_DUMMY_ERROR = 1;

enum
{/*{{{*/
  c_gpsd_type_VERSION = 0,
  c_gpsd_type_DEVICES,
  c_gpsd_type_DEVICE,
  c_gpsd_type_SKY,
  c_gpsd_type_WATCH,
  c_gpsd_type_TPV,
};/*}}}*/

// - timer identifier -
enum
{/*{{{*/
  c_gpsd_TIMER_CONNECT = 0,
  c_gpsd_TIMER_CHECK,
};/*}}}*/

// === definition of structure gpsd_conn_s =====================================

using gpsd_conn_recv_callback_t = std::function<void(uint32_t a_conn_idx,
    uint32_t a_type,var_s a_message)>;

struct gpsd_conn_s
{/*{{{*/
private:
  pointer_s m_pointer;

  static constexpr uint32_t c_error_retry_sec = 5;
  static constexpr uint32_t c_check_period_sec = 15;

  var_s m_class_TPV_var = "TPV";

  std::string m_ip;
  uint16_t m_port;
  uint32_t m_index;

  tcp_conn_s m_conn;
  epoll_fd_s m_conn_timer;

  json_parser_s m_json_parser;
  bool m_tpv_received = true;

  gpsd_conn_recv_callback_t m_conn_recv_callback;

  auto static retrieve_line(const array<char> &a_message,uint32_t *a_input_idx) -> uint32_t;

  auto tcp_event(uint32_t a_conn_idx,uint32_t a_event) -> void;
  auto tcp_recv(uint32_t a_conn_idx,array<char> *a_message) -> void;

  auto timer_event(const epoll_event &a_epoll_event,uint32_t a_timer_id) -> void;

public:
  ~gpsd_conn_s() = default;

  gpsd_conn_s() = default;
  gpsd_conn_s(const gpsd_conn_s &a_src) = delete;
  gpsd_conn_s(gpsd_conn_s &&a_src) = default;

  gpsd_conn_s(std::string a_ip,uint16_t a_port,
      gpsd_conn_recv_callback_t a_conn_recv_callback,
      uint32_t a_index);

  auto operator=(const gpsd_conn_s &a_src) -> gpsd_conn_s & = delete;
  auto operator=(gpsd_conn_s &&a_src) -> gpsd_conn_s & = default;
};/*}}}*/

} // namespace ccl

#endif

