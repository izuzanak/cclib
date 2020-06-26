
#include "ccl_linux.h"

namespace ccl {

// === constants and definitions ===============================================

fd_update_t *g_fd_update = nullptr;
list<void *> pointer_s::g_list;

// === methods of structure fd_s ===============================================

auto fd_s::write(size_t a_size,const void *a_src) -> fd_s &
{/*{{{*/
  debug_assert(m_fd != -1);

  if (a_size > 0)
  {
    ssize_t cnt;
    size_t writed = 0;

    do
    {
      // - ERROR -
      if ((cnt = ::write(m_fd,static_cast<const char *>(a_src) + writed,a_size - writed)) == -1)
      {
        throw_error(FD_WRITE_ERROR);
      }
    }
    while((writed += cnt) < a_size);
  }

  return *this;
}/*}}}*/

auto fd_s::writev(iovec *a_iov,int a_iovcnt,size_t a_size) -> fd_s &
{/*{{{*/
  debug_assert(m_fd != -1);

  ssize_t cnt;
  size_t writed = 0;

  do {
    if ((cnt = ::writev(m_fd,a_iov,a_iovcnt)) == -1)
    {
      throw_error(FD_WRITE_ERROR);
    }

    // - all data writed -
    if ((writed += cnt) >= a_size)
    {
      break;
    }

    // - remove completely writed buffers -
    while (cnt >= static_cast<ssize_t>(a_iov->iov_len))
    {
      cnt -= a_iov->iov_len;

      ++a_iov;
      --a_iovcnt;
    }

    // - adjust partially writed buffer -
    a_iov->iov_base = static_cast<char *>(a_iov->iov_base) + cnt;
    a_iov->iov_len -= cnt;

  } while(true);

  return *this;
}/*}}}*/

auto fd_s::read(array<char> *a_trg) -> fd_s &
{/*{{{*/
  debug_assert(m_fd != -1);

  const size_t c_buffer_add = 1024;

  int inq_cnt;
  ssize_t read_cnt;
  do
  {
    a_trg->reserve(c_buffer_add);
    read_cnt = ::read(m_fd,a_trg->data() + a_trg->used(),c_buffer_add);

    if (read_cnt == -1)
    {
      throw_error(FD_READ_ERROR);
    }

    a_trg->used() += read_cnt;

    if (ioctl(m_fd,TIOCINQ,&inq_cnt) == -1)
    {
      throw_error(FD_READ_ERROR);
    }
  }
  while(inq_cnt > 0);

  return *this;
}/*}}}*/

auto fd_s::read_cnt(size_t a_count,array<char> *a_trg) -> fd_s &
{/*{{{*/
  debug_assert(m_fd != -1);

  size_t old_used = a_trg->used();
  a_trg->push_blanks(a_count);

  size_t readed = 0;
  ssize_t read_cnt;

  do
  {
    read_cnt = ::read(m_fd,a_trg->data() + old_used + readed,a_count - readed);

    // - ERROR -
    if (read_cnt == -1)
    {
      throw_error(FD_READ_ERROR);
    }

    readed += read_cnt;
  }
  while(readed < a_count && read_cnt != 0);

  // - ERROR -
  if (readed < a_count)
  {
    throw_error(FD_READ_ERROR);
  }

  return *this;
}/*}}}*/

// === methods of structure pid_s ==============================================

pid_s::pid_s(array<const char *> &&a_arguments)
{/*{{{*/
  if (a_arguments.used() <= 0)
  {
    throw_error(PID_MISSING_PROGRAM_NAME);
  }

  pid_t pid = fork();

  if (pid == -1)
  {
    throw_error(PID_CANNOT_CREATE_NEW_PROCESS);
  }

  // - process is child -
  if (pid == 0)
  {
    // - add terminating argument -
    a_arguments.push(nullptr);

    // - execute target process -
    if (execvp(a_arguments[0],const_cast<char* const*>(a_arguments.data())) == -1)
    {
      exit(127);
    }
  }

  m_pid = pid;
}/*}}}*/

// === methods of structure epoll_s ============================================

auto epoll_s::__fd_update(int a_fd,uint32_t a_evts,bool a_update_cb,fd_callback_t &&a_callback) -> epoll_s &
{/*{{{*/
  debug_assert(a_fd >= 0);

  // - resize fd events if needed -
  if (m_fd_events.used() <= a_fd)
  {
    m_fd_events.push_blanks(a_fd + 1 - m_fd_events.used());
  }

  fd_event &fd_event = m_fd_events[a_fd];

  // - update callback if requested -
  if (a_update_cb)
  {
    fd_event.m_callback = std::move(a_callback);
  }

  // - adjust epoll fd events -
  if (fd_event.m_evts != a_evts)
  {
    epoll_event epoll_event = {};
    epoll_event.events = a_evts;
    epoll_event.data.fd = a_fd;

    if (fd_event.m_evts == 0)
    {
      if (epoll_ctl(m_fd,EPOLL_CTL_ADD,a_fd,&epoll_event) == -1)
      {
        throw_error(EPOLL_CONTROL_ADD_ERROR);
      }
    }
    else if (a_evts == 0)
    {
      if (epoll_ctl(m_fd,EPOLL_CTL_DEL,a_fd,&epoll_event) == -1)
      {
        // - closed fd, not problem -
        if (errno != EBADF)
        {
          throw_error(EPOLL_CONTROL_DELETE_ERROR);
        }
      }
    }
    else
    {
      if (epoll_ctl(m_fd,EPOLL_CTL_MOD,a_fd,&epoll_event) == -1)
      {
        // - reopened fd, not problem -
        if (errno != ENOENT)
        {
          throw_error(EPOLL_CONTROL_MODIFY_ERROR);
        }

        if (epoll_ctl(m_fd,EPOLL_CTL_ADD,a_fd,&epoll_event) == -1)
        {
          throw_error(EPOLL_CONTROL_ADD_ERROR);
        }
      }
    }

    // - update fd events -
    fd_event.m_evts = a_evts;
  }

  return *this;
}/*}}}*/

auto epoll_s::wait(int a_timeout) -> epoll_s &
{/*{{{*/
  epoll_event event;

  // - call epoll_wait -
  int count = epoll_wait(m_fd,&event,1,a_timeout);

  // - ERROR -
  if (count == -1)
  {
    // - interrupted by signal -
    if (errno == EINTR)
    {
      return *this;
    }

    throw_error(EPOLL_WAIT_ERROR);
  }

  // - process fd events -
  if (count > 0)
  {
    // - check event callback -
    fd_callback_t &callback = m_fd_events[event.data.fd].m_callback;

    if (callback != nullptr)
    {
      // - call event callback -
      callback(event);
    }
  }

  return *this;
}/*}}}*/

} // namespace ccl

