
#include "ccl_tcp.h"

namespace ccl {

// === methods of structure tcp_conn_s =========================================

tcp_conn_s::tcp_conn_s(const char *a_ip,uint16_t a_port,
    tcp_conn_event_callback_t a_conn_event_callback,
    tcp_conn_recv_callback_t a_conn_recv_callback,
    tcp_conn_send_callback_t a_conn_send_callback,
    uint32_t a_index) :
  m_epoll_fd(fd_s::sock(AF_INET,SOCK_STREAM)),
  m_connecting(true),m_connected(false),
  m_conn_event_callback(std::move(a_conn_event_callback)),
  m_conn_recv_callback(std::move(a_conn_recv_callback)),
  m_conn_send_callback(std::move(a_conn_send_callback)),
  m_index(a_index)
{/*{{{*/
  fd_s &sock = m_epoll_fd.fd();

  int nonblock_io = 1;
  int yes = 1;

  if (ioctl(sock,FIONBIO,&nonblock_io) != 0 ||
      setsockopt(sock,SOL_TCP,TCP_NODELAY,&yes,sizeof(int)) != 0)
  {
    throw_error(TCP_CONN_CONNECT_ERROR);
  }

  socket_address_s address(a_ip,a_port);
  if (connect(sock,address,sizeof(sockaddr_in)) != 0)
  {
    if (errno != EINPROGRESS)
    {
      throw_error(TCP_CONN_CONNECT_ERROR);
    }
  }
}/*}}}*/

auto tcp_conn_s::recv_msg() -> tcp_conn_s &
{/*{{{*/
  constexpr ssize_t c_buffer_add = 4096;
  unsigned msg_old_used = m_in_msg.used();

  ssize_t read_cnt;
  do
  {
    m_in_msg.reserve(c_buffer_add);

#ifdef CLIB_WITH_OPENSSL
    if (m_ssl != nullptr)
    {
      read_cnt = SSL_read(m_ssl,m_in_msg.data() + m_in_msg.used(),c_buffer_add);

      if (read_cnt <= 0)
      {
        switch (SSL_get_error(m_ssl,read_cnt))
        {
          case SSL_ERROR_WANT_READ:
            break;
          case SSL_ERROR_WANT_WRITE:
            m_ssl_action = SSL_ACTION_RECV_MSG;

            // - modify fd epoll events -
            m_epoll_fd.fd_update(EPOLLOUT);

            break;
          default:
            throw_error(TCP_CONN_READ_ERROR);
        }

        break;
      }
    }
    else
    {
#endif
      read_cnt = read(m_epoll_fd,m_in_msg.data() + m_in_msg.used(),c_buffer_add);

      if (read_cnt == -1)
      {
        throw_error(TCP_CONN_READ_ERROR);
      }
#ifdef CLIB_WITH_OPENSSL
    }
#endif

    m_in_msg.used() += read_cnt;

#ifdef CLIB_WITH_OPENSSL
    if (m_ssl == nullptr)
    {
#endif

      // - ERROR -
      int inq_cnt;
      if (ioctl(m_epoll_fd,TIOCINQ,&inq_cnt) == -1)
      {
        throw_error(TCP_CONN_READ_ERROR);
      }

      if (inq_cnt <= 0)
      {
        break;
      }
#ifdef CLIB_WITH_OPENSSL
    }
    else
    {
      if (read_cnt <= 0)
      {
        break;
      }
    }
#endif
  } while(true);

  if (m_in_msg.used() > msg_old_used)
  {
    m_conn_recv_callback(m_index,&m_in_msg);
  }
  else
  {
#ifdef CLIB_WITH_OPENSSL
    if (m_ssl == nullptr)
    {
#endif
      if (m_in_msg.used() == msg_old_used)
      {
        throw_error(TCP_CONN_READ_ERROR);
      }
#ifdef CLIB_WITH_OPENSSL
    }
#endif
  }

  return *this;
}/*}}}*/

auto tcp_conn_s::send_msg() -> tcp_conn_s &
{/*{{{*/
  if (m_out_msg_queue.used() > 0)
  {
    array<char> &message = *m_out_msg_queue.last();
    size_t write_cnt = message.used() - m_out_msg_offset;

    // - limit maximal write size -
    if (write_cnt > 4096)
    {
      write_cnt = 4096;
    }

    ssize_t cnt;

#ifdef CLIB_WITH_OPENSSL
    if (m_ssl != nullptr)
    {
      cnt = SSL_write(m_ssl,message.data() + m_out_msg_offset,write_cnt);

      if (cnt <= 0)
      {
        switch (SSL_get_error(m_ssl,cnt))
        {
          case SSL_ERROR_WANT_READ:
            m_ssl_action = SSL_ACTION_SEND_MSG;

            // - modify fd epoll events -
            m_epoll_fd.fd_update(EPOLLIN | EPOLLPRI);

            break;
          case SSL_ERROR_WANT_WRITE:
            break;
          default:
            throw_error(TCP_CONN_WRITE_ERROR);
        }

        cnt = 0;
      }
    }
    else
    {
#endif
      cnt = write(m_epoll_fd,message.data() + m_out_msg_offset,write_cnt);

      // - ERROR -
      if (cnt == -1)
      {
        throw_error(TCP_CONN_WRITE_ERROR);
      }
#ifdef CLIB_WITH_OPENSSL
    }
#endif

    // - whole message was send -
    if ((m_out_msg_offset += cnt) >= message.used())
    {
      // - remove message from queue -
      m_out_msg_queue.next() = nullptr;

      // - reset out message offset -
      m_out_msg_offset = 0;

      // - conn send callback is set -
      if (m_conn_send_callback != nullptr)
      {
        m_conn_send_callback(m_index);
      }
    }
  }
  else
  {
    // - modify fd epoll events: only input -
    m_epoll_fd.fd_update(EPOLLIN | EPOLLPRI);
  }

  return *this;
}/*}}}*/

auto tcp_conn_s::fd_event(const epoll_event &a_epoll_event) -> void
{/*{{{*/
  if (a_epoll_event.data.fd != m_epoll_fd)
  {
    throw_error(TCP_CONN_INVALID_FD);
  }

  if (m_connecting)
  {
    // - reset client connecting flag -
    m_connecting = false;

    int error;
    socklen_t length = sizeof(error);

    // - check connect result -
    if (getsockopt(m_epoll_fd,SOL_SOCKET,SO_ERROR,&error,&length) != 0)
    {
      error = 1;
    }

#ifdef CLIB_WITH_OPENSSL
    if (m_ssl == nullptr)
    {
#endif
      // - disable nonblocking io -
      int nonblock_io = 0;
      if (ioctl(m_epoll_fd,FIONBIO,&nonblock_io) != 0)
      {
        error = 1;
      }
#ifdef CLIB_WITH_OPENSSL
    }
#endif

    if (error != 0)
    {
      m_epoll_fd.fd_update(0);

      // - call event error callback -
      m_conn_event_callback(m_index,c_tcp_EVENT_ERROR);
    }
    else
    {
      // - set connected flag -
      m_connected = true;

      // - call event connected callback -
      m_conn_event_callback(m_index,c_tcp_EVENT_CONNECTED);

      // - modify fd epoll events -
      if (m_out_msg_queue.used() <= 0)
      {
        m_epoll_fd.fd_update(EPOLLIN | EPOLLPRI);
      }
    }
  }
  else
  {
    try
    {
#ifdef CLIB_WITH_OPENSSL
      if (m_ssl_action != SSL_ACTION_NONE)
      {
        // - reset ssl action -
        uint32_t ssl_action = m_ssl_action;
        m_ssl_action = SSL_ACTION_NONE;

        // - modify fd epoll events -
        m_epoll_fd.fd_update(EPOLLIN | EPOLLPRI | (m_out_msg_queue.used() > 0 ? EPOLLOUT : 0));

        switch (ssl_action)
        {
        case SSL_ACTION_SEND_MSG:
          send_msg();
          break;
        case SSL_ACTION_RECV_MSG:
          recv_msg();
          break;
        }
      }
      else
      {
#endif
        if ((a_epoll_event.events & EPOLLOUT) != 0)
        {
          send_msg();
        }

        if ((a_epoll_event.events & EPOLLIN) != 0)
        {
          recv_msg();
        }
#ifdef CLIB_WITH_OPENSSL
      }
#endif
    }
    catch (...)
    {
      m_epoll_fd.fd_update(0);

      // - reset connected flag -
      m_connected = false;

      // - call event droped callback -
      if (m_conn_event_callback != nullptr)
      {
        m_conn_event_callback(m_index,c_tcp_EVENT_DROPPED);
      }
      else
      {
        throw;
      }
    }
  }
}/*}}}*/

// === methods of structure tcp_server_s =======================================

auto tcp_server_s::fd_event(const epoll_event &a_epoll_event) -> void
{/*{{{*/
  if (a_epoll_event.data.fd != m_epoll_fd)
  {
    throw_error(TCP_SERVER_INVALID_FD);
  }

  uint32_t conn_idx = c_idx_not_exist;

  try
  {
    // - append cnnection to list -
    socket_address_s address;
    conn_idx = m_conn_list.append(tcp_conn_s(
      epoll_fd_s(m_epoll_fd.fd().sock_accept(&address)),
      m_conn_recv_callback,
      m_conn_send_callback));

    tcp_conn_s &conn = m_conn_list[conn_idx];

    conn.m_epoll_fd.fd_update(EPOLLIN | EPOLLPRI,true,
    [this,conn_idx](const epoll_event &a_epoll_event) -> void {
      conn_fd_event(conn_idx,a_epoll_event);
    });

    conn.m_index = conn_idx;

#ifdef CLIB_WITH_OPENSSL
    if (m_ssl_ctx != nullptr)
    {
      int nonblock_io = 1;
      if (ioctl(conn.m_epoll_fd,FIONBIO,&nonblock_io) != 0)
      {
        throw_error(TCP_SERVER_ACCEPT_ERROR);
      }

      conn.m_ssl = m_ssl_ctx.conn_accept(conn.m_epoll_fd);
    }
#endif

    // - call event accept callback -
    m_conn_event_callback(conn_idx,c_tcp_EVENT_ACCEPTED);
  }
  catch (...)
  {
    // - remove connection from list -
    if (conn_idx != c_idx_not_exist)
    {
      m_conn_list[conn_idx] = {};
      m_conn_list.remove(conn_idx);
    }

    throw_error(TCP_SERVER_ACCEPT_ERROR);
  }
}/*}}}*/

auto tcp_server_s::conn_fd_event(uint32_t a_conn_idx,const epoll_event &a_epoll_event) -> void
{/*{{{*/
  tcp_conn_s &conn = m_conn_list[a_conn_idx];

  try
  {
    conn.fd_event(a_epoll_event);
  }
  catch (...)
  {
    try
    {
      // - call event drop callback -
      m_conn_event_callback(a_conn_idx,c_tcp_EVENT_DROPPED);
    }
    catch (...)
    {
      m_conn_list[a_conn_idx] = {};
      m_conn_list.remove(a_conn_idx);

      throw_error(TCP_CONN_CALLBACK_ERROR);
    }

    m_conn_list[a_conn_idx] = {};
    m_conn_list.remove(a_conn_idx);
  }
}/*}}}*/

} // namespace ccl

