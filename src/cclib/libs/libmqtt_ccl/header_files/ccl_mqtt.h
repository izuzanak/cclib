
#ifndef LIBS_LIBMQTT_CCL_HEADER_FILES_CCL_MQTT_H
#define LIBS_LIBMQTT_CCL_HEADER_FILES_CCL_MQTT_H

#include "ccl_tcp.h"

namespace ccl {

constexpr int MQTT_INVALID_VARIABLE_BYTE_INTEGER = 1;
constexpr int MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE = 2;
constexpr int MQTT_UNSUPPORTED_QOS_LEVEL = 3;
constexpr int MQTT_UNKNOWN_CONTROL_PACKET_TYPE = 4;
constexpr int MQTT_INVALID_CONTROL_PACKET = 5;
constexpr int MQTT_INVALID_CONTROL_PACKET_PROPERTY = 6;
constexpr int MQTT_INVALID_CONNACK_PACKET = 7;
constexpr int MQTT_INVALID_PUBACK_PACKET = 8;
constexpr int MQTT_INVALID_SUBACK_PACKET = 9;
constexpr int MQTT_INVALID_UNSUBACK_PACKET = 10;
constexpr int MQTT_INVALID_PUBLISH_PACKET = 11;
constexpr int MQTT_INVALID_PUBLISH_REQUEST = 12;
constexpr int MQTT_INVALID_SUBSCRIBE_REQUEST = 13;
constexpr int MQTT_INVALID_UNSUBSCRIBE_REQUEST = 14;
constexpr int MQTT_PING_TIMEOUT = 15;

// === definition of structure mqtt_buff_s =====================================

struct mqtt_buff_s
{/*{{{*/
  uint32_t m_size;
  const char *m_data;
};/*}}}*/

// === definition of structure mqtt_conn_s =====================================

// - callback event -
enum
{/*{{{*/
  c_mqtt_EVENT_ERROR = 0,
  c_mqtt_EVENT_CONNECTED,
  c_mqtt_EVENT_DISCONNECTED,
  c_mqtt_EVENT_DROPPED,
};/*}}}*/

// - control packet type -
enum
{/*{{{*/
  c_mqtt_cpt_CONNECT     = 1,
  c_mqtt_cpt_CONNACK     = 2,
  c_mqtt_cpt_PUBLISH     = 3,
  c_mqtt_cpt_PUBACK      = 4,
  c_mqtt_cpt_PUBREC      = 5,
  c_mqtt_cpt_PUBREL      = 6,
  c_mqtt_cpt_PUBCOMP     = 7,
  c_mqtt_cpt_SUBSCRIBE   = 8,
  c_mqtt_cpt_SUBACK      = 9,
  c_mqtt_cpt_UNSUBSCRIBE = 10,
  c_mqtt_cpt_UNSUBACK    = 11,
  c_mqtt_cpt_PINGREQ     = 12,
  c_mqtt_cpt_PINGRESP    = 13,
  c_mqtt_cpt_DISCONNECT  = 14,
  c_mqtt_cpt_AUTH        = 15,

  // - not standard -
  c_mqtt_cpt_WILLPROPS   = 16,
};/*}}}*/

// - control packet data type -
enum
{/*{{{*/
  c_mqtt_type_BYTE = 0,
  c_mqtt_type_TWO_BYTE_INTEGER,
  c_mqtt_type_FOUR_BYTE_INTEGER,
  c_mqtt_type_VARIABLE_BYTE_INTEGER,
  c_mqtt_type_UTF_8_ENCODED_STRING,
  c_mqtt_type_UTF_8_STRING_PAIR,
  c_mqtt_type_BINARY_DATA,
};/*}}}*/

// - timer identifier -
enum
{/*{{{*/
  c_mqtt_TIMER_CONNECT = 0,
  c_mqtt_TIMER_PING,
};/*}}}*/

struct mqtt_prop_s
{/*{{{*/
  uint8_t m_code;
  const char *m_descr;
  uint8_t m_type;
  uint32_t m_cpt_mask;
};/*}}}*/

struct mqtt_publish_s
{/*{{{*/
  std::string m_topic;
  array<char> m_payload;
  uint8_t m_qos;
  bool m_retain;
};/*}}}*/

struct mqtt_subscribe_s
{/*{{{*/
  array<std::string> m_filters;
  uint8_t m_max_qos;
};/*}}}*/

using mqtt_conn_event_callback_t = std::function<void(uint32_t a_conn_idx,uint32_t a_event)>;
using mqtt_conn_recv_callback_t  = std::function<void(uint32_t a_conn_idx,
    const mqtt_buff_s &a_topic,const mqtt_buff_s &a_message)>;

struct mqtt_conn_s
{/*{{{*/
private:
  static constexpr uint32_t packet_prop_cnt = 43;
  static const std::array<mqtt_prop_s,packet_prop_cnt> packet_props;

  static constexpr uint32_t c_error_retry_sec = 5;
  static constexpr uint32_t c_ping_period_sec = 15;

  std::string m_client_id;
  std::string m_ip;
  uint16_t m_port;
  uint32_t m_index;

  tcp_conn_s m_conn;
  epoll_fd_s m_conn_timer;

  bool m_ping_resp;
  bool m_connected = false;
  bool m_disconnecting = false;

  list<mqtt_publish_s,uint16_t> m_published;
  list<mqtt_subscribe_s,uint16_t> m_subscribed;
  list<array<std::string>,uint16_t> m_unsubscribed;

  mqtt_conn_event_callback_t m_conn_event_callback;
  mqtt_conn_recv_callback_t m_conn_recv_callback;

  array<char> m_buffer;

  auto tcp_event(uint32_t a_conn_idx,uint32_t a_event) -> void;
  auto tcp_recv(uint32_t a_conn_idx,array<char> *a_message) -> void;
  auto tcp_send(uint32_t a_conn_idx) -> void;

  auto timer_event(const epoll_event &a_epoll_event,uint32_t a_timer_id) -> void;

  auto process_properties(uint8_t a_pkt_type,uint32_t a_size,const char *a_data) -> mqtt_conn_s &;
  auto process_packet(uint8_t a_pkt_type,uint32_t a_size,const char *a_data) -> mqtt_conn_s &;

  auto send_publish(const std::string &a_topic,const array<char> &a_payload,
      uint8_t a_qos,bool a_retain,bool a_dup,uint16_t a_packet_id) -> mqtt_conn_s &;
  auto send_subscribe(const array<std::string> &a_filters,uint8_t a_max_qos,uint16_t a_packet_id) -> mqtt_conn_s &;
  auto send_unsubscribe(const array<std::string> &a_filters,uint16_t a_packet_id) -> mqtt_conn_s &;

public:
  [[nodiscard]] constexpr auto connected() const noexcept -> bool { return m_connected; }
  auto conn() -> tcp_conn_s & { return m_conn; }

  ~mqtt_conn_s() = default;

  mqtt_conn_s() = default;
  mqtt_conn_s(const mqtt_conn_s &a_src) = delete;
  mqtt_conn_s(mqtt_conn_s &&a_src) = default;

  mqtt_conn_s(std::string a_client_id,
      std::string a_ip,uint16_t a_port,
      mqtt_conn_event_callback_t a_conn_event_callback,
      mqtt_conn_recv_callback_t a_conn_recv_callback,
      uint32_t a_index);

  auto operator=(const mqtt_conn_s &a_src) -> mqtt_conn_s & = delete;
  auto operator=(mqtt_conn_s &&a_src) -> mqtt_conn_s & = default;

#ifdef CLIB_WITH_OPENSSL
  auto init_ssl() -> mqtt_conn_s &
  {/*{{{*/
    m_conn.init_ssl();

    return *this;
  }/*}}}*/
#endif

  auto static twobyte_enc(uint16_t a_value,array<char> *a_trg) -> void
  {/*{{{*/
    uint16_t net_value = htons(a_value);
    a_trg->append(2,reinterpret_cast<char *>(&net_value));
  }/*}}}*/

  auto static twobyte_dec(const char *a_src) -> uint16_t
  {/*{{{*/
    uint16_t net_value;
    memcpy(&net_value,a_src,2);
    return ntohs(net_value);
  }/*}}}*/

  auto static varint_enc(uint32_t a_value,array<char> *a_trg) -> void;
  auto static varint_dec(const char *a_src,const char **a_end) -> uint32_t;

  auto publish(const std::string &a_topic,array<char> a_payload,
      uint8_t a_qos = 0,bool a_retain = false) -> mqtt_conn_s &
  {/*{{{*/
    if (m_disconnecting || a_qos >= 3)
    {
      throw_error(MQTT_INVALID_PUBLISH_REQUEST);
    }

    uint16_t packet_id = 0;
    array<char> *payload;

    if (a_qos > 0)
    {
      packet_id = m_published.append(mqtt_publish_s{a_topic,std::move(a_payload),a_qos,a_retain});
      payload = &m_published[packet_id].m_payload;
    }
    else
    {
      payload = &a_payload;
    }

    if (m_connected)
    {
      send_publish(a_topic,*payload,a_qos,a_retain,false,packet_id);
    }

    return *this;
  }/*}}}*/

  auto subscribe(array<std::string> a_filters,uint8_t a_max_qos = 0) -> mqtt_conn_s &
  {/*{{{*/
    if (m_disconnecting || a_filters.used() <= 0 || a_max_qos >= 3)
    {
      throw_error(MQTT_INVALID_SUBSCRIBE_REQUEST);
    }

    uint16_t packet_id = m_subscribed.append(mqtt_subscribe_s{std::move(a_filters),a_max_qos});
    send_subscribe(m_subscribed[packet_id].m_filters,a_max_qos,packet_id);

    return *this;
  }/*}}}*/

  auto unsubscribe(array<std::string> a_filters) -> mqtt_conn_s &
  {/*{{{*/
    if (m_disconnecting || a_filters.used() <= 0)
    {
      throw_error(MQTT_INVALID_UNSUBSCRIBE_REQUEST);
    }

    uint16_t packet_id = m_unsubscribed.append(std::move(a_filters));
    send_unsubscribe(m_unsubscribed[packet_id],packet_id);

    return *this;
  }/*}}}*/

  auto disconnect() -> mqtt_conn_s &
  {/*{{{*/
    if (m_connected)
    {
      m_buffer.used() = 0;
      m_buffer
        .push(0xe0)  // - DISCONNECT -
        .push(0x00); // - remaining length -

      m_conn.schedule_message(m_buffer);
    }

    // - reset ping timer -
    m_conn_timer.fd().timer_set({{0,0},{0,0}},0);

    // - reset connected flag -
    m_connected = false;

    // - set disconnecting flag -
    m_disconnecting = true;

    return *this;
  }/*}}}*/
};/*}}}*/

} // namespace ccl

#endif

