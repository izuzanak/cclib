
#ifndef LIBS_LIBLINUX_CCL_HEADER_FILES_CCL_LINUX_H
#define LIBS_LIBLINUX_CCL_HEADER_FILES_CCL_LINUX_H

#include "ccl_time.h"

#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>

namespace ccl {

constexpr int SOCKET_ADDRESS_RESOLVE_ERROR = 1;
constexpr int SOCKET_ADDRESS_NAME_ERROR = 2;

constexpr int FD_WRITE_ERROR = 1;
constexpr int FD_READ_ERROR = 2;

constexpr int SOCKET_CREATE_ERROR = 1;
constexpr int SOCKET_BIND_ERROR = 2;
constexpr int SOCKET_LISTEN_ERROR = 3;
constexpr int SOCKET_ACCEPT_ERROR = 4;
constexpr int SOCKET_ADDRESS_UNKNOWN_FORMAT = 5;
constexpr int SOCKET_CONNECT_ERROR = 6;
constexpr int SOCKET_SENDTO_ERROR = 7;
constexpr int SOCKET_RECVFROM_ERROR = 8;
constexpr int SOCKET_UDP_MAX_MSG_SIZE_EXCEEDED = 9;
constexpr int SOCKET_GETSOCKNAME_ERROR = 10;

constexpr int TIMER_CREATE_ERROR = 6;
constexpr int TIMER_SETTIME_ERROR = 7;
constexpr int TIMER_READ_ERROR = 8;

constexpr int PID_MISSING_PROGRAM_NAME = 1;
constexpr int PID_CANNOT_CREATE_NEW_PROCESS = 2;

constexpr int EPOLL_CREATE_ERROR = 1;
constexpr int EPOLL_CONTROL_ADD_ERROR = 2;
constexpr int EPOLL_CONTROL_DELETE_ERROR = 3;
constexpr int EPOLL_CONTROL_MODIFY_ERROR = 4;
constexpr int EPOLL_WAIT_ERROR = 5;

// === constants and definitions ===============================================

using fd_callback_t = std::function<void(const epoll_event &a_epoll_event)>;
using fd_update_t = auto (int a_fd,unsigned a_evts,bool a_update_cb,fd_callback_t &&a_callback) -> void;

extern fd_update_t *g_fd_update;

// === definition of structure socket_address_s ================================

struct socket_address_s
{/*{{{*/
private:
  struct sockaddr_in m_sockaddr;

public:
  ~socket_address_s() = default;
  socket_address_s() = default;
  socket_address_s(const socket_address_s &a_src) = default;
  socket_address_s(socket_address_s &&a_src) = default;

  socket_address_s(const char *a_address,uint16_t a_port)
  {/*{{{*/

    // - retrieve host by name address -
    struct hostent *host = gethostbyname(a_address);

    if (host == nullptr)
    {
      throw_error(SOCKET_ADDRESS_RESOLVE_ERROR);
    }

    memcpy(&m_sockaddr.sin_addr.s_addr,host->h_addr,host->h_length);
    m_sockaddr.sin_port = htons(a_port);
    m_sockaddr.sin_family = AF_INET;
  }/*}}}*/

  auto operator=(const socket_address_s &a_src) -> socket_address_s & = default;
  auto operator=(socket_address_s &&a_src) -> socket_address_s & = default;

  operator sockaddr *() { return reinterpret_cast<sockaddr *>(&m_sockaddr); } // NOLINT(google-explicit-constructor)
  operator const sockaddr *() const { return reinterpret_cast<const sockaddr *>(&m_sockaddr); } // NOLINT(google-explicit-constructor)

  auto static to_string(array<char> *a_trg) noexcept -> array<char> &
  {/*{{{*/
    return a_trg->append(18,"socket_address_s{}");
  }/*}}}*/

  auto name(array<char> *a_trg) -> array<char> &
  {/*{{{*/
    a_trg->reserve(256);

    char *trg_data_end = a_trg->data() + a_trg->used();

    // - ERROR -
    if (getnameinfo(*this,sizeof(sockaddr_in),
          trg_data_end,256,nullptr,0,NI_NUMERICHOST | NI_NUMERICSERV) != 0)
    {
      throw_error(SOCKET_ADDRESS_NAME_ERROR);
    }

    a_trg->used() += strlen(trg_data_end);

    return *a_trg;
  }/*}}}*/

  [[nodiscard]] auto port() const noexcept -> uint16_t
  {/*{{{*/
    return ntohs(m_sockaddr.sin_port);
  }/*}}}*/
};/*}}}*/

// === definition of structure fd_s ============================================

struct fd_s
{/*{{{*/
private:
  int m_fd = -1;

public:
  ~fd_s()
  {/*{{{*/
    if (m_fd != -1)
    {
      close(m_fd);
    }
  }/*}}}*/

  fd_s() = default;
  fd_s(const fd_s &a_src) = delete;
  fd_s(fd_s &&a_src) noexcept
  {/*{{{*/
    m_fd = a_src.m_fd;
    a_src.m_fd = -1;
  }/*}}}*/

  explicit fd_s(int a_fd) noexcept : m_fd(a_fd) {}

  auto operator=(const fd_s &a_src) -> fd_s & = delete;
  auto operator=(fd_s &&a_src) noexcept -> fd_s &
  {/*{{{*/
    this->~fd_s();
    new (this) fd_s(std::move(a_src));

    return *this;
  }/*}}}*/

  auto operator=(int a_fd) noexcept -> fd_s &
  {/*{{{*/
    this->~fd_s();
    new (this) fd_s(a_fd);

    return *this;
  }/*}}}*/

  operator int () const noexcept { return m_fd; } // NOLINT(google-explicit-constructor)

  auto operator==(const fd_s &a_second) const noexcept -> bool
  {/*{{{*/
    return m_fd == a_second.m_fd;
  }/*}}}*/

  [[nodiscard]] auto compare(const fd_s &a_second) const noexcept -> int
  {/*{{{*/
    return m_fd < a_second.m_fd ? -1 : m_fd > a_second.m_fd ? 1 : 0;
  }/*}}}*/

  auto to_string(array<char> *a_trg) const noexcept -> array<char> &
  {/*{{{*/
    return a_trg->append_format("fd_s{%d}",m_fd);
  }/*}}}*/

  auto write(size_t a_size,const void *a_src) -> fd_s &;
  auto writev(iovec *a_iov,int a_iovcnt,size_t a_size) -> fd_s &;
  auto read(array<char> *a_trg) -> fd_s &;
  auto read_cnt(size_t a_count,array<char> *a_trg) -> fd_s &;

  static auto sock(int a_domain,int a_type) -> fd_s
  {/*{{{*/
    fd_s sock(socket(a_domain,a_type,0));

    // - create socket -
    if (sock == -1)
    {
      throw_error(SOCKET_CREATE_ERROR);
    }

    return sock;
  }/*}}}*/

  auto sock_bind(const socket_address_s &a_addr) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1);

    if (bind(m_fd,a_addr,sizeof(sockaddr_in)) != 0)
    {
      throw_error(SOCKET_BIND_ERROR);
    }

    return *this;
  }/*}}}*/

  auto sock_listen(const socket_address_s &a_addr,int a_backlog) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1);

    // - set server socket as reusable -
    int yes = 1;
    setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

    if (bind(m_fd,a_addr,sizeof(sockaddr_in)) != 0)
    {
      throw_error(SOCKET_BIND_ERROR);
    }

    if (listen(m_fd,a_backlog) != 0)
    {
      throw_error(SOCKET_LISTEN_ERROR);
    }

    return *this;
  }/*}}}*/

  auto sock_accept(socket_address_s *a_addr) const -> fd_s
  {/*{{{*/
    debug_assert(*this != -1);

    socklen_t addr_len = sizeof(sockaddr_in);

    fd_s sock(accept(m_fd,*a_addr,&addr_len));

    if (sock < 0)
    {
      throw_error(SOCKET_ACCEPT_ERROR);
    }

    if (addr_len != sizeof(sockaddr_in))
    {
      throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
    }

    return sock;
  }/*}}}*/

  auto sock_connect(const socket_address_s &a_addr) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1);

    if (connect(m_fd,a_addr,sizeof(sockaddr_in)) != 0)
    {
      throw_error(SOCKET_CONNECT_ERROR);
    }

    return *this;
  }/*}}}*/

  auto sock_sendto(const socket_address_s &a_addr,ssize_t a_size,const void *a_src) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1 && a_size >= 0);

    if (sendto(m_fd,a_src,a_size,0,a_addr,sizeof(sockaddr_in)) != a_size)
    {
      throw_error(SOCKET_SENDTO_ERROR);
    }

    return *this;
  }/*}}}*/

  auto sock_recvfrom(array<char> *a_trg,socket_address_s *a_addr) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1);

    // - max count of udp message bytes -
    const ssize_t c_udp_max_cnt = 4096;

    // - prepare target buffer -
    a_trg->reserve(c_udp_max_cnt);

    // - source address length -
    socklen_t addr_len = sizeof(sockaddr_in);

    ssize_t cnt;
    if ((cnt = recvfrom(m_fd,a_trg->data() + a_trg->used(),c_udp_max_cnt,0,*a_addr,&addr_len)) == -1)
    {
      throw_error(SOCKET_RECVFROM_ERROR);
    }

    if (cnt >= c_udp_max_cnt)
    {
      throw_error(SOCKET_UDP_MAX_MSG_SIZE_EXCEEDED);
    }

    if (addr_len != sizeof(sockaddr_in))
    {
      throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
    }

    // - update target buffer -
    a_trg->used() += cnt;

    return *this;
  }/*}}}*/

  [[nodiscard]] auto sock_address() const -> socket_address_s
  {/*{{{*/
    debug_assert(m_fd != -1);

    socket_address_s addr;
    socklen_t addr_len = sizeof(sockaddr_in);

    if (getsockname(m_fd,addr,&addr_len) != 0)
    {
      throw_error(SOCKET_GETSOCKNAME_ERROR);
    }

    if (addr_len != sizeof(sockaddr_in))
    {
      throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
    }

    return addr;
  }/*}}}*/

  static auto timer(const itimerspec &a_itimerspec = {{0,0},{0,0}},int a_flags = 0) -> fd_s
  {/*{{{*/
    fd_s timer(timerfd_create(CLOCK_MONOTONIC,0));

    if (timer == -1)
    {
      throw_error(TIMER_CREATE_ERROR);
    }

    timer.timer_set(a_itimerspec,a_flags);

    return timer;
  }/*}}}*/

  auto timer_set(const itimerspec &a_itimerspec,int a_flags) -> fd_s &
  {/*{{{*/
    debug_assert(m_fd != -1);

    // - set timer -
    if (timerfd_settime(m_fd,a_flags,&a_itimerspec,nullptr) != 0)
    {
      throw_error(TIMER_SETTIME_ERROR);
    }

    return *this;
  }/*}}}*/

  static auto timer_read(int a_fd) -> void
  {/*{{{*/
    uint64_t timer_exps;
    if (::read(a_fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
    {
      throw_error(TIMER_READ_ERROR);
    }
  }/*}}}*/
};/*}}}*/

// === definition of structure pid_s ===========================================

struct pid_s
{/*{{{*/
private:
  pid_t m_pid = -1;

public:
  ~pid_s()
  {/*{{{*/
    if (m_pid != -1)
    {
      int status;

      // - check if process was terminated -
      if (waitpid(m_pid,&status,WNOHANG) <= 0)
      {
        // - send terminate signal to process -
        if (kill(m_pid,SIGTERM) == 0)
        {
          // - wait on process to terminate -
          waitpid(m_pid,&status,0);
        }
      }
    }
  }/*}}}*/

  pid_s(const pid_s &a_src) = delete;
  pid_s(pid_s &&a_src) noexcept : m_pid(a_src.m_pid)
  {/*{{{*/
    a_src.m_pid = -1;
  }/*}}}*/
  explicit pid_s(array<const char *> &&a_arguments);

  auto operator=(const pid_s &a_src) -> pid_s & = delete;
  auto operator=(pid_s &&a_src) noexcept -> pid_s &
  {/*{{{*/
    this->~pid_s();
    new (this) pid_s(std::move(a_src));

    return *this;
  }/*}}}*/

  operator pid_t () const { return m_pid; } // NOLINT(google-explicit-constructor)

  auto to_string(array<char> *a_trg) const noexcept -> array<char> &
  {/*{{{*/
    return a_trg->append_format("pid_s{%d}",m_pid);
  }/*}}}*/
};/*}}}*/

// === definition of structure epoll_s =========================================

struct epoll_s
{/*{{{*/
  struct fd_event
  {/*{{{*/
    friend epoll_s;

  private:
    uint32_t m_evts = 0;
    fd_callback_t m_callback = nullptr;
  };/*}}}*/

private:
  fd_s m_fd;
  array<fd_event,int> m_fd_events;

public:
  ~epoll_s() = default;

  epoll_s() = default;
  epoll_s(const epoll_s &a_src) = delete;
  epoll_s(epoll_s &&a_src) = delete;

  explicit epoll_s(int a_flags) : m_fd(epoll_create1(a_flags))
  {/*{{{*/
    if (m_fd == -1)
    {
      throw_error(EPOLL_CREATE_ERROR);
    }
  }/*}}}*/

  auto operator=(const epoll_s &a_src) -> epoll_s & = delete;
  auto operator=(epoll_s &&a_src) -> epoll_s & = delete;

  auto __fd_update(int a_fd,uint32_t a_evts,bool a_update_cb = false,fd_callback_t &&a_callback = nullptr) -> epoll_s &;
  auto wait(int a_timeout) -> epoll_s &;
};/*}}}*/

// === definition of structure epoll_fd_s ======================================

struct epoll_fd_s
{/*{{{*/
  friend epoll_s;

private:
  fd_s m_fd;

public:
  auto fd() -> fd_s & { return m_fd; }

  ~epoll_fd_s() noexcept
  {/*{{{*/
    if (m_fd != -1)
    {
      g_fd_update(m_fd,0,true,nullptr);
    }
  }/*}}}*/

  epoll_fd_s() = default;
  epoll_fd_s(const epoll_fd_s &a_src) = delete;
  epoll_fd_s(epoll_fd_s &&a_src) noexcept : m_fd(std::move(a_src.m_fd)) {}
  explicit epoll_fd_s(fd_s &&a_fd) noexcept : m_fd(std::move(a_fd)) {}

  auto operator=(const epoll_fd_s &a_src) -> epoll_fd_s & = delete;
  auto operator=(epoll_fd_s &&a_src) noexcept -> epoll_fd_s &
  {/*{{{*/
    this->~epoll_fd_s();
    new (this) epoll_fd_s(std::move(a_src));

    return *this;
  }/*}}}*/

  operator int () const noexcept { return m_fd; } // NOLINT(google-explicit-constructor)

  auto fd_update(uint32_t a_evts,bool a_update_cb = false,fd_callback_t &&a_callback = nullptr) -> epoll_fd_s &
  {/*{{{*/
    g_fd_update(m_fd,a_evts,a_update_cb,std::move(a_callback));

    return *this;
  }/*}}}*/
};/*}}}*/

// === definition of structure pointer_s =======================================

struct pointer_s
{/*{{{*/
private:
  static list<void *> g_list;
  uint32_t m_index;

public:
  [[nodiscard]] constexpr auto index() const noexcept -> uint32_t { return m_index; }

  template<class TYPE>
  [[nodiscard]] static auto ptr(uint32_t a_index) noexcept -> TYPE *
  { return reinterpret_cast<TYPE *>(g_list[a_index]); }

  ~pointer_s() noexcept
  {/*{{{*/
    if (m_index != c_idx_not_exist)
    {
      g_list.remove(m_index);
    }
  }/*}}}*/

  pointer_s() noexcept
  {/*{{{*/
    m_index = g_list.append(this);
  }/*}}}*/

  pointer_s(const pointer_s &a_src) = delete;

  pointer_s(pointer_s &&a_src) noexcept
  {/*{{{*/
    m_index = a_src.m_index;
    g_list[m_index] = this;

    a_src.m_index = c_idx_not_exist;
  }/*}}}*/

  auto operator=(const pointer_s &a_src) -> pointer_s & = delete;

  auto operator=(pointer_s &&a_src) noexcept -> pointer_s &
  {/*{{{*/
    this->~pointer_s();
    new (this) pointer_s(std::move(a_src));

    return *this;
  }/*}}}*/
};/*}}}*/

} // namespace ccl

#endif

