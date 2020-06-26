
#include "main.h"

namespace ccl {

// === members of structure mqtt_comm_s ========================================

bool mqtt_comm_s::m_terminate = false;
epoll_s *g_epoll = nullptr;

// === methods of structure mqtt_comm_s ========================================

mqtt_comm_s::mqtt_comm_s(std::string a_client_id,const char *a_ip,uint16_t a_port) :
  m_conn(std::move(a_client_id),a_ip,a_port,
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx,uint32_t a_event) -> void {
      pointer_s::ptr<mqtt_comm_s>(ptr_index)->conn_event(a_conn_idx,a_event); },
      [ptr_index = m_pointer.index()](uint32_t a_conn_idx,const mqtt_buff_s &a_topic,const mqtt_buff_s &a_message) -> void {
      pointer_s::ptr<mqtt_comm_s>(ptr_index)->conn_recv(a_conn_idx,a_topic,a_message); },
      0),
  m_timer(fd_s::timer())
{/*{{{*/
  process_s::simple_signal_handler(term_signal);

  m_timer.fd_update(EPOLLIN | EPOLLPRI,true,
      [ptr_index = m_pointer.index()](const epoll_event &a_epoll_event) -> void {
      pointer_s::ptr<mqtt_comm_s>(ptr_index)->timer_event(a_epoll_event); });

  //m_conn.init_ssl();
}/*}}}*/

auto mqtt_comm_s::run() -> mqtt_comm_s &
{/*{{{*/
  while(!m_terminate)
  {
    // - wait on events -
    g_epoll->wait(-1);
  }

  return *this;
}/*}}}*/

auto mqtt_comm_s::conn_event(uint32_t a_conn_idx,uint32_t a_event) -> void
{/*{{{*/
  (void)a_conn_idx;

  //fprintf(stderr,"mqtt_comm_s::conn_event: %u - %u\n",a_conn_idx,a_event);

  switch (a_event)
  {/*{{{*/
    case c_mqtt_EVENT_ERROR:
      break;
    case c_mqtt_EVENT_CONNECTED:
      {/*{{{*/
        m_conn.subscribe({"/test/pub","output/mod/ndb/config/gps"},1);
        m_conn.publish("/test/pub",array<char>("Hello MQTT world"),1);

        m_conn.unsubscribe({"output/mod/ndb/config/gps","output/mod/ndb/config/gps"});
        m_conn.publish("/test/pub",array<char>("Hello MQTT world"),1);

        m_timer.fd().timer_set({{0,100000000},{0,100000000}},0);
      }/*}}}*/
      break;
    case c_mqtt_EVENT_DISCONNECTED:
      m_terminate = true;
      break;
    case c_mqtt_EVENT_DROPPED:
      break;
  }/*}}}*/
}/*}}}*/

auto mqtt_comm_s::conn_recv(uint32_t a_conn_idx,
    const mqtt_buff_s &a_topic,const mqtt_buff_s &a_message) -> void
{/*{{{*/
  (void)this;
  (void)a_conn_idx;
  (void)a_message;

  fprintf(stderr,"mqtt_comm_s::conn_recv: %u - %.*s: %.*s\n",a_conn_idx,
      a_topic.m_size,a_topic.m_data,a_message.m_size,a_message.m_data);
}/*}}}*/

auto mqtt_comm_s::timer_event(const epoll_event &a_epoll_event) -> void
{/*{{{*/
  fd_s::timer_read(a_epoll_event.data.fd);

  if (m_conn.connected())
  {
    array<char> buffer;
    buffer.append(strlen("Hello MQTT timer"),"Hello MQTT timer");
    m_conn.publish("/test/pub",std::move(buffer),1);
  }
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

  //ccl::mqtt_comm_s comm("TestClient","10.2.1.151",1883);
  //ccl::mqtt_comm_s comm("TestClient","localhost",1883);
  ccl::mqtt_comm_s comm("TestClient","192.168.3.6",1883);
  comm.run();

  return 0;
}/*}}}*/

