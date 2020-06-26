
#include "ccl_gpsd.h"

namespace ccl {

// === methods of structure gpsd_conn_s ========================================

gpsd_conn_s::gpsd_conn_s(std::string a_ip,uint16_t a_port,
    gpsd_conn_recv_callback_t a_conn_recv_callback,
    uint32_t a_index) :
  m_ip(std::move(a_ip)),
  m_port(a_port),
  m_index(a_index),
  m_conn_timer(fd_s::timer()),
  m_conn_recv_callback(std::move(a_conn_recv_callback))
{/*{{{*/

  // - set connect timer -
  m_conn_timer.fd().timer_set({{0,0},{0,1}},0);
  m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,
      [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
      pointer_s::ptr<gpsd_conn_s>(ptr_index)->timer_event(a_epoll_event,c_gpsd_TIMER_CONNECT); });
}/*}}}*/

auto gpsd_conn_s::retrieve_line(const array<char> &a_message,uint32_t *a_input_idx) -> uint32_t
{/*{{{*/
#define GPSD_GET_NEXT_CHAR() \
{\
   if (*a_input_idx < a_message.used()) {\
      in_char = (uint8_t)a_message.data()[*a_input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define GPSD_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (*a_input_idx >= a_message.used()) {\
      return RET_TERM_IDX;\
   }\
\
   (*a_input_idx)++;\
}

   uint8_t in_char;
   goto fa_start_label;

// - STATE 0 -
state_0_label:
   GPSD_CLOSE_CHAR(c_idx_not_exist);

fa_start_label:
   GPSD_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_0_label;
   }
   if (in_char == 13) {
      goto state_1_label;
   }
   if (in_char >= 14) {
      goto state_0_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   GPSD_CLOSE_CHAR(c_idx_not_exist);
   GPSD_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   GPSD_CLOSE_CHAR(0);
   return 0;

}/*}}}*/

auto gpsd_conn_s::tcp_event(uint32_t a_conn_idx,uint32_t a_event) -> void
{/*{{{*/
  (void)a_conn_idx;

  switch (a_event)
  {
    case c_tcp_EVENT_ERROR:
      {/*{{{*/

        // - set connect timer -
        m_conn_timer.fd().timer_set({{0,0},{c_error_retry_sec,0}},0);
        m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,
            [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->timer_event(a_epoll_event,c_gpsd_TIMER_CONNECT); });
      }/*}}}*/
      break;
    case c_tcp_EVENT_CONNECTED:
      {/*{{{*/

        // - set tpv received flag -
        m_tpv_received = true;

        // - set check timer -
        m_conn_timer.fd().timer_set({{c_check_period_sec,0},{0,1}},0);
        m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,
            [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->timer_event(a_epoll_event,c_gpsd_TIMER_CHECK); });
      }/*}}}*/
      break;
    case c_tcp_EVENT_DROPPED:
      {/*{{{*/
        
        // - set connect timer -
        m_conn_timer.fd().timer_set({{0,0},{c_error_retry_sec,0}},0);
        m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,
            [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->timer_event(a_epoll_event,c_gpsd_TIMER_CONNECT); });
      }/*}}}*/
      break;
  }
}/*}}}*/

auto gpsd_conn_s::tcp_recv(uint32_t a_conn_idx,array<char> *a_message) -> void
{/*{{{*/
  uint32_t input_idx = 0;
  do
  {
    // - line not ready -
    uint32_t old_input_idx = input_idx;
    if (retrieve_line(*a_message,&input_idx) != 0)
    {
      a_message->tail(a_message->used() - old_input_idx);
      break;
    }

    uint32_t message_length = input_idx - old_input_idx - 2;
    var_s msg_var = m_json_parser.parse(message_length,a_message->data() + old_input_idx);

    if (msg_var["class"] == m_class_TPV_var)
    {
      // - set tpv received flag -
      m_tpv_received = true;

      // - call receive callback -
      m_conn_recv_callback(a_conn_idx,c_gpsd_type_TPV,msg_var);
    }
  } while(true);
}/*}}}*/

auto gpsd_conn_s::timer_event(const epoll_event &a_epoll_event,uint32_t a_timer_id) -> void
{/*{{{*/
  fd_s::timer_read(a_epoll_event.data.fd);

  switch (a_timer_id)
  {
    case c_gpsd_TIMER_CONNECT:
      {/*{{{*/
        m_conn = tcp_conn_s(m_ip.data(),m_port,
            [ptr_index = m_pointer.index()](uint32_t a_conn_idx,uint32_t a_event) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->tcp_event(a_conn_idx,a_event); },
            [ptr_index = m_pointer.index()](uint32_t a_conn_idx,array<char> *a_message) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->tcp_recv(a_conn_idx,a_message); },
            nullptr,m_index);

        m_conn.fd_update(EPOLLIN | EPOLLOUT | EPOLLPRI,true,
            [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
            pointer_s::ptr<gpsd_conn_s>(ptr_index)->m_conn.fd_event(a_epoll_event); });
      }/*}}}*/
      break;
    case c_gpsd_TIMER_CHECK:
      {/*{{{*/
        if (m_tpv_received)
        {
          // - periodic watch request -
          m_conn.schedule_message(ccl::array<char>(R"'(?WATCH={"class":"WATCH","json":true})'"));

          // - reset tpv received flag -
          m_tpv_received = false;
        }
        else
        {
          m_conn = tcp_conn_s();

          // - set connect timer -
          m_conn_timer.fd().timer_set({{0,0},{0,1}},0);
          m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,
              [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
              pointer_s::ptr<gpsd_conn_s>(ptr_index)->timer_event(a_epoll_event,c_gpsd_TIMER_CONNECT); });
        }
      }/*}}}*/
      break;
  }
}/*}}}*/

} // namespace ccl

