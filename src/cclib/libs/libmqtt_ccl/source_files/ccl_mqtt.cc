
#include "ccl_mqtt.h"

namespace ccl {

const std::array<mqtt_prop_s,mqtt_conn_s::packet_prop_cnt> mqtt_conn_s::packet_props =
{/*{{{*/
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x01,"Payload Format Indicator"         ,c_mqtt_type_BYTE                 ,0x010008},
  mqtt_prop_s{0x02,"Message Expiry Interval"          ,c_mqtt_type_FOUR_BYTE_INTEGER    ,0x010008},
  mqtt_prop_s{0x03,"Content Type"                     ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x010008},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x08,"Response Topic"                   ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x010008},
  mqtt_prop_s{0x09,"Correlation Data"                 ,c_mqtt_type_BINARY_DATA          ,0x010008},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x0b,"Subscription Identifier"          ,c_mqtt_type_VARIABLE_BYTE_INTEGER,0x000108},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x11,"Session Expiry Interval"          ,c_mqtt_type_FOUR_BYTE_INTEGER    ,0x004006},
  mqtt_prop_s{0x12,"Assigned Client Identifier"       ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x000004},
  mqtt_prop_s{0x13,"Server Keep Alive"                ,c_mqtt_type_TWO_BYTE_INTEGER     ,0x000004},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x15,"Authentication Method"            ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x008006},
  mqtt_prop_s{0x16,"Authentication Data"              ,c_mqtt_type_BINARY_DATA          ,0x008006},
  mqtt_prop_s{0x17,"Request Problem Information"      ,c_mqtt_type_BYTE                 ,0x000002},
  mqtt_prop_s{0x18,"Will Delay Interval"              ,c_mqtt_type_FOUR_BYTE_INTEGER    ,0x010000},
  mqtt_prop_s{0x19,"Request Response Information"     ,c_mqtt_type_BYTE                 ,0x000002},
  mqtt_prop_s{0x1a,"Response Information"             ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x000004},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x1c,"Server Reference"                 ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x004004},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x1f,"Reason String"                    ,c_mqtt_type_UTF_8_ENCODED_STRING ,0x00caf4},
  mqtt_prop_s{0xff,""                                 ,0                                ,0x000000},
  mqtt_prop_s{0x21,"Receive Maximum"                  ,c_mqtt_type_TWO_BYTE_INTEGER     ,0x000006},
  mqtt_prop_s{0x22,"Topic Alias Maximum"              ,c_mqtt_type_TWO_BYTE_INTEGER     ,0x000006},
  mqtt_prop_s{0x23,"Topic Alias"                      ,c_mqtt_type_TWO_BYTE_INTEGER     ,0x000008},
  mqtt_prop_s{0x24,"Maximum QoS"                      ,c_mqtt_type_BYTE                 ,0x000004},
  mqtt_prop_s{0x25,"Retain Available"                 ,c_mqtt_type_BYTE                 ,0x000004},
  mqtt_prop_s{0x26,"User Property"                    ,c_mqtt_type_UTF_8_STRING_PAIR    ,0x01cffe},
  mqtt_prop_s{0x27,"Maximum Packet Size"              ,c_mqtt_type_FOUR_BYTE_INTEGER    ,0x000006},
  mqtt_prop_s{0x28,"Wildcard Subscription Available"  ,c_mqtt_type_BYTE                 ,0x000004},
  mqtt_prop_s{0x29,"Subscription Identifier Available",c_mqtt_type_BYTE                 ,0x000004},
  mqtt_prop_s{0x2a,"Shared Subscription Available"    ,c_mqtt_type_BYTE                 ,0x000004}
};/*}}}*/

// === methods of structure mqtt_conn_s ========================================

mqtt_conn_s::mqtt_conn_s(std::string a_client_id,
    std::string a_ip,uint16_t a_port,
    mqtt_conn_event_callback_t a_conn_event_callback,
    mqtt_conn_recv_callback_t a_conn_recv_callback,
    uint32_t a_index) :
  m_client_id(std::move(a_client_id)),
  m_ip(std::move(a_ip)),
  m_port(a_port),
  m_index(a_index),
  m_conn_timer(fd_s::timer()),
  m_conn_event_callback(std::move(a_conn_event_callback)),
  m_conn_recv_callback(std::move(a_conn_recv_callback))
{/*{{{*/

  // - set connect timer -
  m_conn_timer.fd().timer_set({{0,0},{0,1}},0);
  m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,[this](const epoll_event &a_epoll_event) -> void {
      timer_event(a_epoll_event,c_mqtt_TIMER_CONNECT); });

  m_published.append({});
  m_subscribed.append({});
  m_unsubscribed.append({});
}/*}}}*/

auto mqtt_conn_s::tcp_event(uint32_t a_conn_idx,uint32_t a_event) -> void
{/*{{{*/
  (void)this;
  (void)a_conn_idx;

  //fprintf(stderr,"mqtt_conn_s::tcp_event: %u - %u\n",a_conn_idx,a_event);

  switch (a_event)
  {
    case c_tcp_EVENT_ERROR:
      {/*{{{*/
        m_conn_event_callback(a_conn_idx,c_mqtt_EVENT_ERROR);

        // - set connect timer -
        m_conn_timer.fd().timer_set({{0,0},{c_error_retry_sec,0}},0);
        m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,[this](const epoll_event &a_epoll_event) -> void {
            timer_event(a_epoll_event,c_mqtt_TIMER_CONNECT); });
      }/*}}}*/
      break;
    case c_tcp_EVENT_CONNECTED:
      {/*{{{*/
        m_buffer.used() = 0;
        m_buffer.push(0x10); // - CONNECT -
        varint_enc(
          10 +                   // - variable header -
          1 +                    // - properties -
          2 + m_client_id.size() // - client id -
          ,&m_buffer); // - remaining length -
        m_buffer
          .append(7,"\x00\x04MQTT\x05")

          // - connect flags -
          .push(
            0 << 7 |     // - user name flag -
            0 << 6 |     // - password flag -
            0 << 5 |     // - will retain -
            0x00 << 3 |  // - will qos (2 bits) -
            0 << 2 |     // - will flag -
            1 << 1 |     // - clean start -
            0 << 0 )     // - reserved -

          // - keep alive -
          .append(2,"\x00\x00")

          // - properties length -
          .push('\x00');

        // - client id -
        twobyte_enc(m_client_id.size(),&m_buffer);
        m_buffer.append(m_client_id.size(),m_client_id.data());

        m_conn.schedule_message(m_buffer);
      }/*}}}*/
      break;
    case c_tcp_EVENT_DROPPED:
      {/*{{{*/
        if (m_disconnecting)
        {
          m_conn_event_callback(a_conn_idx,c_mqtt_EVENT_DISCONNECTED);
        }
        else
        {
          m_conn_event_callback(a_conn_idx,c_mqtt_EVENT_DROPPED);

          // - set connect timer -
          m_conn_timer.fd().timer_set({{0,0},{0,1}},0);
          m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,[this](const epoll_event &a_epoll_event) -> void {
              timer_event(a_epoll_event,c_mqtt_TIMER_CONNECT); });
        }
      }/*}}}*/
      break;
  }
}/*}}}*/

auto mqtt_conn_s::tcp_recv(uint32_t a_conn_idx,array<char> *a_message) -> void
{/*{{{*/
  (void)a_conn_idx;

  //fprintf(stderr,"mqtt_conn_s::tcp_recv: %u\n",a_conn_idx);

  do
  {
    uint32_t msg_used = a_message->used();

    const char *remain;
    uint32_t remain_length;
    uint32_t pkt_size;

    if (msg_used >= 5)
    {
      remain_length = varint_dec(a_message->data() + 1,&remain);
      pkt_size = remain_length + (remain - a_message->data());
    }
    else
    {
      if (msg_used >= 2 && msg_used == 2U + static_cast<uint8_t>(a_message->data()[1]))
      {
        remain = a_message->data() + 2;
        remain_length = msg_used - 2;
        pkt_size = msg_used;
      }
      else
      {
        break;
      }
    }

    if (msg_used >= pkt_size)
    {
      process_packet(a_message->data()[0],remain_length,remain);

      // - remove packet from receive buffer -
      a_message->tail(msg_used - pkt_size);
    }
    else
    {
      break;
    }

  } while(true);
}/*}}}*/

auto mqtt_conn_s::tcp_send(uint32_t a_conn_idx) -> void // NOLINT(readability-convert-member-functions-to-static)
{/*{{{*/
  //fprintf(stderr,"mqtt_conn_s::tcp_send: %u\n",a_conn_idx);
}/*}}}*/

auto mqtt_conn_s::timer_event(const epoll_event &a_epoll_event,uint32_t a_timer_id) -> void
{/*{{{*/
  fd_s::timer_read(a_epoll_event.data.fd);

  switch (a_timer_id)
  {
    case c_mqtt_TIMER_CONNECT:
      {/*{{{*/
        m_conn = tcp_conn_s(m_ip.data(),m_port,
            [this](uint32_t a_conn_idx,uint32_t a_event) -> void { tcp_event(a_conn_idx,a_event); },
            [this](uint32_t a_conn_idx,array<char> *a_message) -> void { tcp_recv(a_conn_idx,a_message); },
            [this](uint32_t a_conn_idx) -> void { tcp_send(a_conn_idx); },
            m_index);

        m_conn.fd_update(EPOLLIN | EPOLLOUT | EPOLLPRI,true,
            [this](const epoll_event &a_epoll_event) -> void {
            m_conn.fd_event(a_epoll_event);
            });
      }/*}}}*/
      break;
    case c_mqtt_TIMER_PING:
      {/*{{{*/
        
        // - previous ping response not received -
        if (!m_ping_resp)
        {
          throw_error(MQTT_PING_TIMEOUT);
        }

        // - send ping request -
        m_buffer.used() = 0;
        m_buffer
          .push(0xc0)  // - PINGREQ -
          .push(0x00); // - remaining length -

        m_conn.schedule_message(m_buffer);

        // - reset ping response flag -
        m_ping_resp = false;
      }/*}}}*/
      break;
  }
}/*}}}*/

auto mqtt_conn_s::varint_enc(uint32_t a_value,array<char> *a_trg) -> void
{/*{{{*/
  if (a_value < 0x4000)
  {
    if (a_value < 0x80)
    {
      a_trg->push(a_value);
    }
    else
    {
      a_trg->push(0x80 | (a_value & 0x7f));
      a_trg->push(a_value >> 7);
    }
  }
  else
  {
    if (a_value < 0x200000)
    {
      a_trg->push(0x80 | (a_value & 0x7f));
      a_trg->push(0x80 | ((a_value >>= 7) & 0x7f));
      a_trg->push(a_value >> 7);
    }
    else
    {
      if (a_value > 268435455)
      {
        throw_error(MQTT_INVALID_VARIABLE_BYTE_INTEGER);
      }

      a_trg->push(0x80 | (a_value & 0x7f));
      a_trg->push(0x80 | ((a_value >>= 7) & 0x7f));
      a_trg->push(0x80 | ((a_value >>= 7) & 0x7f));
      a_trg->push(a_value >> 7);
    }
  }
}/*}}}*/

auto mqtt_conn_s::varint_dec(const char *a_src,const char **a_end) -> uint32_t
{/*{{{*/
  uint32_t value = 0;
  uint32_t shift = 0;

  do {
    value += (*a_src & 0x7f) << shift;

    if ((shift += 7) > 28)
    {
      throw_error(MQTT_INVALID_VARIABLE_BYTE_INTEGER);
    }
  } while ((*a_src++ & 0x80) != 0);

  *a_end = a_src;

  return value;
}/*}}}*/

auto mqtt_conn_s::process_properties(uint8_t a_pkt_type,uint32_t a_size,const char *a_data) -> mqtt_conn_s &
{/*{{{*/
  const char *data_end = a_data + a_size;
  do {
    uint8_t code = a_data[0];

    const mqtt_prop_s *property;
    if (code >= packet_prop_cnt ||
        (property = &packet_props[code])->m_code != code ||
        (property->m_cpt_mask & (1 << (a_pkt_type >> 4))) == 0)

    {
      throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
    }

    switch (property->m_type)
    {
      case c_mqtt_type_BYTE:
        a_data += 1 + 1;
        break;
      case c_mqtt_type_TWO_BYTE_INTEGER:
        a_data += 1 + 2;
        break;
      case c_mqtt_type_FOUR_BYTE_INTEGER:
        a_data += 1 + 4;
        break;
      case c_mqtt_type_VARIABLE_BYTE_INTEGER:
        varint_dec(a_data + 1,&a_data);
        break;
      case c_mqtt_type_UTF_8_ENCODED_STRING:
        {
          uint16_t net_length;
          memcpy(&net_length,a_data + 1,2);
          uint16_t length = ntohs(net_length);
          a_data += 1 + 2 + length;
        }
        break;
      case c_mqtt_type_UTF_8_STRING_PAIR:
        {
          uint16_t net_length;
          uint16_t length;

          memcpy(&net_length,a_data + 1,2);
          length = ntohs(net_length);
          a_data += 1 + 2 + length;

          memcpy(&net_length,a_data,2);
          length = ntohs(net_length);
          a_data += 2 + length;
        }
        break;
      case c_mqtt_type_BINARY_DATA:
        {
          uint16_t net_length;
          memcpy(&net_length,a_data + 1,2);
          uint16_t length = ntohs(net_length);
          a_data += 1 + 2 + length;
        }
        break;
      default:
        throw_error();
    }

  } while(a_data < data_end);

  return *this;
}/*}}}*/

auto mqtt_conn_s::process_packet(uint8_t a_pkt_type,uint32_t a_size,const char *a_data) -> mqtt_conn_s &
{/*{{{*/
  const char *data_end = a_data + a_size;

  switch (a_pkt_type >> 4)
  {
    case c_mqtt_cpt_CONNECT:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case c_mqtt_cpt_CONNACK:
      {/*{{{*/
        if ((a_pkt_type & 0x0f) != 0)
        {
          throw_error(MQTT_INVALID_CONNACK_PACKET);
        }

        if ((a_data[0] & 0xfe) != 0 || // - connect acknowledge flags -
            a_data[1] != 0x00)         // - connect reason code -
        {
          throw_error(MQTT_INVALID_CONNACK_PACKET);
        }

        a_data += 2;

        // - properties -
        uint32_t props_length = varint_dec(a_data,&a_data);

        if (props_length > 0)
        {
          process_properties(a_pkt_type,props_length,a_data);
        }

        // - resend all unacknowledged published packets -
        if (m_published.count() > 1)
        {
          // - skip first index -
          uint32_t mp_idx = m_published.next_idx(m_published.first_idx());
          do {
            const mqtt_publish_s &publish = m_published[mp_idx];
            send_publish(publish.m_topic,publish.m_payload,publish.m_qos,publish.m_retain,true,mp_idx);

            mp_idx = m_published.next_idx(mp_idx);
          } while(mp_idx != decltype(m_published)::idx_not_exist);
        }

        m_conn_event_callback(m_index,c_mqtt_EVENT_CONNECTED);

        // - schedule ping timer -
        m_ping_resp = true;
        m_conn_timer.fd().timer_set({{c_ping_period_sec,0},{c_ping_period_sec,0}},0);
        m_conn_timer.fd_update(EPOLLIN | EPOLLPRI,true,[this](const epoll_event &a_epoll_event) -> void {
            timer_event(a_epoll_event,c_mqtt_TIMER_PING); });
      }/*}}}*/
      break;
    case c_mqtt_cpt_PUBLISH:
      {/*{{{*/
        uint8_t dup = (a_pkt_type & 0x04) >> 3;
        uint8_t qos = (a_pkt_type & 0x06) >> 1;
        uint8_t retain = a_pkt_type & 0x01;

        (void)dup;
        (void)retain;

        uint32_t topic_length = twobyte_dec(a_data);
        mqtt_buff_s topic{topic_length,a_data + 2};
        a_data += 2 + topic_length;

        uint16_t packet_id = 0;
        if (qos > 0)
        {
          packet_id = twobyte_dec(a_data);
          a_data += 2;
        }

        // - properties -
        uint32_t props_length = varint_dec(a_data,&a_data);

        if (props_length > 0)
        {
          process_properties(a_pkt_type,props_length,a_data);
          a_data += props_length;
        }

        uint32_t payload_length = data_end - a_data;
        m_conn_recv_callback(m_index,topic,{payload_length,a_data});

        switch (qos)
        {
        case 0:
          break;
        case 1:
          {/*{{{*/
            m_buffer.used() = 0;
            m_buffer
              .push(0x40)  // - PUBACK -
              .push(0x02); // - remaining length -

            // - packet identifier -
            twobyte_enc(packet_id,&m_buffer);

            m_conn.schedule_message(m_buffer);
          }/*}}}*/
          break;
        case 2:
          throw_error(MQTT_UNSUPPORTED_QOS_LEVEL);
        }
      }/*}}}*/
      break;
    case c_mqtt_cpt_PUBACK:
      {/*{{{*/
        if ((a_pkt_type & 0x0f) != 0)
        {
          throw_error(MQTT_INVALID_PUBACK_PACKET);
        }
        
        // - packet identifier -
        uint16_t packet_id = twobyte_dec(a_data);

        if (!m_published.is_valid(packet_id) || m_published[packet_id].m_qos != 1)
        {
          throw_error(MQTT_INVALID_PUBACK_PACKET);
        }

        m_published.remove(packet_id);
      }/*}}}*/
      break;
    case c_mqtt_cpt_PUBREC:
    case c_mqtt_cpt_PUBREL:
    case c_mqtt_cpt_PUBCOMP:
    case c_mqtt_cpt_SUBSCRIBE:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case c_mqtt_cpt_SUBACK:
      {/*{{{*/
        if ((a_pkt_type & 0x0f) != 0)
        {
          throw_error(MQTT_INVALID_SUBACK_PACKET);
        }

        // - packet identifier -
        uint16_t packet_id = twobyte_dec(a_data);
        a_data += 2;

        if (!m_subscribed.is_valid(packet_id))
        {
          throw_error(MQTT_INVALID_SUBACK_PACKET);
        }

        mqtt_subscribe_s &subscribe = m_subscribed[packet_id];

        // - properties -
        uint32_t props_length = varint_dec(a_data,&a_data);

        if (props_length > 0)
        {
          process_properties(a_pkt_type,props_length,a_data);
          a_data += props_length;
        }

        // - check reason code count -
        if (static_cast<size_t>(data_end - a_data) != subscribe.m_filters.used())
        {
          throw_error(MQTT_INVALID_SUBACK_PACKET);
        }

        // - check reason codes -
        do {
          if (*a_data != subscribe.m_max_qos)
          {
            throw_error(MQTT_INVALID_SUBACK_PACKET);
          }
        } while(++a_data < data_end);

        m_subscribed.remove(packet_id);
      }/*}}}*/
      break;
    case c_mqtt_cpt_UNSUBSCRIBE:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case c_mqtt_cpt_UNSUBACK:
      {/*{{{*/
        if ((a_pkt_type & 0x0f) != 0)
        {
          throw_error(MQTT_INVALID_UNSUBACK_PACKET);
        }

        // - packet identifier -
        uint16_t packet_id = twobyte_dec(a_data);
        a_data += 2;

        if (!m_unsubscribed.is_valid(packet_id))
        {
          throw_error(MQTT_INVALID_UNSUBACK_PACKET);
        }

        array<std::string> &filters = m_unsubscribed[packet_id];

        // - properties -
        uint32_t props_length = varint_dec(a_data,&a_data);

        if (props_length > 0)
        {
          process_properties(a_pkt_type,props_length,a_data);
          a_data += props_length;
        }

        // - check reason code count -
        if (static_cast<size_t>(data_end - a_data) != filters.used())
        {
          throw_error(MQTT_INVALID_UNSUBACK_PACKET);
        }

        // - check reason codes -
        do {
          if (static_cast<uint8_t>(*a_data) >= 0x80)
          {
            throw_error(MQTT_INVALID_UNSUBACK_PACKET);
          }
        } while(++a_data < data_end);

        m_unsubscribed.remove(packet_id);
      }/*}}}*/
      break;
    case c_mqtt_cpt_PINGREQ:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case c_mqtt_cpt_PINGRESP:
      {/*{{{*/
        
        // - set ping response flag -
        m_ping_resp = true;
      }/*}}}*/
      break;
    case c_mqtt_cpt_DISCONNECT:
    case c_mqtt_cpt_AUTH:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    default:
      throw_error(MQTT_UNKNOWN_CONTROL_PACKET_TYPE);
  }

  return *this;
}/*}}}*/

auto mqtt_conn_s::send_publish(const std::string &a_topic,const array<char> &a_payload,
    uint8_t a_qos,bool a_retain,bool a_dup,uint16_t a_packet_id) -> mqtt_conn_s &
{/*{{{*/
  m_buffer.used() = 0;
  m_buffer.push(
    0x30 |               // - PUBLISH -
    (a_dup ? 8 : 0) |    // - dup -
    a_qos << 1 |         // - qos level -
    (a_retain ? 1 : 0)); // - retain -
  varint_enc(
    2 + a_topic.size() +         // - topic length -
    (a_packet_id != 0 ? 2 : 0) + // - packet identifier length -
    1 +              // - properties length -
    a_payload.used() // - payload length -
    ,&m_buffer);     // - remaining length -

  // - topic name -
  twobyte_enc(a_topic.size(),&m_buffer);
  m_buffer.append(a_topic.size(),a_topic.data());

  if (a_packet_id != 0)
  {
    // - packet identifier -
    twobyte_enc(a_packet_id,&m_buffer);
  }

  // - properties length -
  m_buffer.push('\x00');

  // - payload -
  m_buffer.append(a_payload.used(),a_payload.data());

  m_conn.schedule_message(m_buffer);

  return *this;
}/*}}}*/

auto mqtt_conn_s::send_subscribe(const array<std::string> &a_filters,
    uint8_t a_max_qos,uint16_t a_packet_id) -> mqtt_conn_s &
{/*{{{*/

  // - retrieve topic filters length -
  const std::string *tf_ptr = a_filters.data();
  const std::string *tf_ptr_end = tf_ptr + a_filters.used();
  uint32_t filters_length = 0;
  do
  {
    filters_length += 2 + 1 + tf_ptr->size();
  } while(++tf_ptr < tf_ptr_end);

  m_buffer.used() = 0;
  m_buffer.push(0x82); // - SUBSCRIBE -
  varint_enc(
    2 +                // - packet identifier -
    1 +                // - properties length -
    filters_length     // - topic filters -
    ,&m_buffer);       // - remaining length -

  // - packet identifier -
  twobyte_enc(a_packet_id,&m_buffer);

  // - properties length -
  m_buffer.push('\x00');

  // - process topic filters -
  tf_ptr = a_filters.data();
  tf_ptr_end = tf_ptr + a_filters.used();
  do
  {
    // - topic filter -
    twobyte_enc(tf_ptr->size(),&m_buffer);
    m_buffer.append(tf_ptr->size(),tf_ptr->data());

    // - subscription options -
    m_buffer.push(
      0 << 4 | // - retain handling (send retained messages at the time of the subscribe) -
      1 << 3 | // - retain as published -
      0 << 2 | // - no local (off) -
      a_max_qos);

  } while(++tf_ptr < tf_ptr_end);

  m_conn.schedule_message(m_buffer);

  return *this;
}/*}}}*/

auto mqtt_conn_s::send_unsubscribe(const array<std::string> &a_filters,
    uint16_t a_packet_id) -> mqtt_conn_s &
{/*{{{*/

  // - retrieve topic filters length -
  const std::string *tf_ptr = a_filters.data();
  const std::string *tf_ptr_end = tf_ptr + a_filters.used();
  uint32_t filters_length = 0;
  do
  {
    filters_length += 2 + tf_ptr->size();
  } while(++tf_ptr < tf_ptr_end);

  m_buffer.used() = 0;
  m_buffer.push(0xa2); // - UNSUBSCRIBE -
  varint_enc(
    2 +                // - packet identifier -
    1 +                // - properties length -
    filters_length     // - topic filters -
    ,&m_buffer);       // - remaining length -

  // - packet identifier -
  twobyte_enc(a_packet_id,&m_buffer);

  // - properties length -
  m_buffer.push('\x00');

  // - process topic filters -
  tf_ptr = a_filters.data();
  tf_ptr_end = tf_ptr + a_filters.used();
  do
  {
    // - topic filter -
    twobyte_enc(tf_ptr->size(),&m_buffer);
    m_buffer.append(tf_ptr->size(),tf_ptr->data());
  } while(++tf_ptr < tf_ptr_end);

  m_conn.schedule_message(m_buffer);

  return *this;
}/*}}}*/

} // namespace ccl

