
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "liblinux_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "fd",
  "socket_address",
  "socket",
  "socket_udp",
  "socket_tcp",
  "pid",
  "poll",
  "epoll",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_fd,
  test_socket_address,
  test_socket,
  test_socket_udp,
  test_socket_tcp,
  test_pid,
  test_poll,
  test_epoll,
};/*}}}*/

// === test execution functions ================================================

void test_fd()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::fd_s fd_0(open("tests/liblinux_ccl_test/fd/test.file",O_CREAT | O_RDWR,0666));

  ccl::regmatch_s match_0;
  cassert(ccl::regex_s("^fd_s{[0-9]\\+}$").match(buffer.clear().debug(fd_0),&match_0));

  ccl::array<ccl::fd_s> fd_array_0;

  uint32_t idx = 0;
  do {
    fd_array_0.push_blank();
  } while(++idx < 5);

  cassert(strcmp(buffer.debug(fd_array_0),"[fd_s{-1},fd_s{-1},fd_s{-1},fd_s{-1},fd_s{-1}]") == 0);

  // - fd writev -
  ccl::array<std::string> strings = {"Some","vector","of","text","data","written","by","vector","write"};
  ccl::array<iovec> iov;

  idx = 0;
  size_t size = 0;
  do {
    std::string &string = strings[idx];

    iov.push(iovec{const_cast<char *>(string.data()),string.length()});
    size += string.length();
  } while(++idx < strings.used());

  fd_0.writev(iov.data(),iov.used(),size);

  // - fd read_cnt -
  cassert(lseek(fd_0,0,SEEK_SET) == 0);

  buffer.reset();
  fd_0.read_cnt(size,&buffer);
  buffer.push('\0');
  cassert(strcmp(buffer.data(),"Somevectoroftextdatawrittenbyvectorwrite") == 0);
}/*}}}*/

void test_socket_address()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::socket_address_s address("127.0.0.1",8000);
  cassert(strcmp(buffer.debug(address),"socket_address_s{}") == 0);

  buffer.reset();
  cassert(strcmp(address.name(&buffer).push('\0').data(),"127.0.0.1") == 0);
  cassert(address.port() == 8000);
}/*}}}*/

void test_socket()
{/*{{{*/
  ccl::array<char> buffer;

  auto sock = ccl::fd_s::sock(AF_INET,SOCK_STREAM);
  puts(buffer.debug(sock));
}/*}}}*/

void test_socket_udp()
{/*{{{*/
  auto recv_pid = ccl::pid_s({"./liblinux_ccl_test_socket_udp_recvfrom"});
  usleep(100000);

  auto send_pid = ccl::pid_s({"./liblinux_ccl_test_socket_udp_sendto"});

  int status;
  cassert(waitpid(send_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(recv_pid,&status,0) != -1 && status == 0);
}/*}}}*/

void test_socket_tcp()
{/*{{{*/
  auto server_pid = ccl::pid_s({"./liblinux_ccl_test_socket_tcp_server"});
  usleep(100000);

  auto client_pid = ccl::pid_s({"./liblinux_ccl_test_socket_tcp_client"});

  int status;
  cassert(waitpid(client_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(server_pid,&status,0) != -1 && status == 0);
}/*}}}*/

void test_pid()
{/*{{{*/
  ccl::array<char> buffer;

  auto pid_0 = ccl::pid_s({"ls"});
  puts(buffer.debug(pid_0));

  int status;
  waitpid(pid_0,&status,0);
}/*}}}*/

#include <map>
#include <poll.h>
#include <vector>

std::map<int,std::pair<int16_t,ccl::fd_callback_t>> g_poll_map;
std::vector<pollfd> g_poll_fds;

auto poll_fd_update(int a_fd,unsigned a_evts,bool a_update_cb,ccl::fd_callback_t &&a_callback) -> void
{/*{{{*/
  auto &ref = g_poll_map[a_fd];

  ref.first = a_evts;

  if (a_update_cb)
  {
    ref.second = std::move(a_callback);
  }

  g_poll_fds.resize(0);
  
  for (auto &evt : g_poll_map)
  {
    g_poll_fds.push_back({evt.first,evt.second.first,0});
  }
}/*}}}*/

auto poll_fd_run(int a_timeout) -> void
{/*{{{*/
  cassert(poll(g_poll_fds.data(),g_poll_fds.size(),a_timeout) >= 0);
  for (auto &pollfd : g_poll_fds)
  {
    if (pollfd.revents != 0)
    {
      auto &callback = g_poll_map[pollfd.fd].second;

      if (callback != nullptr)
      {
        epoll_event event;

        event.events = pollfd.revents;
        event.data.fd = pollfd.fd;

        callback(event);
      }
    }
  }
}/*}}}*/

void test_poll()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::g_fd_update = poll_fd_update;

  ccl::epoll_fd_s fd_0(ccl::fd_s(dup(1)));

  auto callback_0 = [&buffer](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    buffer.push('A');
  };/*}}}*/

  fd_0.fd_update(EPOLLOUT,true,callback_0);
  poll_fd_run(-1);
  poll_fd_run(-1);

  ccl::array<ccl::epoll_fd_s> fd_array_0;
  fd_array_0.push(std::move(fd_0));

  uint32_t idx = 0;
  do {
    fd_array_0.push_blank();
  } while(++idx < 100);

  ccl::epoll_fd_s &fd = fd_array_0[0];
  auto callback_1 = [&buffer,&fd](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    fd.fd_update(EPOLLOUT,true,[&buffer](const epoll_event &a_epoll_event) -> void
    {
      (void)a_epoll_event;

      buffer.push('C');
    });

    buffer.push('B');
  };/*}}}*/

  fd_array_0[0].fd_update(EPOLLOUT,true,callback_1);

  poll_fd_run(-1);
  poll_fd_run(-1);
  poll_fd_run(-1);

  fd_0 = std::move(fd_array_0[0]);
  fd_0.fd_update(EPOLLOUT,true,nullptr);
  poll_fd_run(-1);
  poll_fd_run(-1);

  fd_0.fd_update(EPOLLOUT,true,callback_0);
  poll_fd_run(-1);
  poll_fd_run(-1);

  fd_0.fd_update(EPOLLIN,true,nullptr);
  fd_0.fd_update(0,true,nullptr);

  int counter = 0;
  auto callback_2 = [&buffer,&counter](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    ccl::fd_s::timer_read(a_epoll_event.data.fd);

    buffer.push('T');
    ++counter;
  };/*}}}*/

  ccl::epoll_fd_s timer_0(ccl::fd_s::timer({{0,1000000},{0,1000000}},0));
  timer_0.fd_update(EPOLLIN,true,callback_2);

  do {
    poll_fd_run(-1);
  } while(counter < 10);

  cassert(strcmp(buffer.push('\0').data(),"AABCCAATTTTTTTTTT") == 0);
}/*}}}*/

ccl::epoll_s *g_epoll = nullptr;

auto epoll_fd_update(int a_fd,unsigned a_evts,bool a_update_cb,ccl::fd_callback_t &&a_callback) -> void
{/*{{{*/
  g_epoll->__fd_update(a_fd,a_evts,a_update_cb,std::move(a_callback));
}/*}}}*/

void test_epoll()
{/*{{{*/
  ccl::array<char> buffer;

  ccl::epoll_s epoll_0(0);

  ccl::g_fd_update = epoll_fd_update;
  g_epoll = &epoll_0;

  ccl::epoll_fd_s fd_0(ccl::fd_s(dup(1)));

  auto callback_0 = [&buffer](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    buffer.push('A');
  };/*}}}*/

  fd_0.fd_update(EPOLLOUT,true,callback_0);
  epoll_0.wait(-1);
  epoll_0.wait(-1);

  ccl::array<ccl::epoll_fd_s> fd_array_0;
  fd_array_0.push(std::move(fd_0));

  uint32_t idx = 0;
  do {
    fd_array_0.push_blank();
  } while(++idx < 100);

  ccl::epoll_fd_s &fd = fd_array_0[0];
  auto callback_1 = [&buffer,&fd](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    fd.fd_update(EPOLLOUT,true,[&buffer](const epoll_event &a_epoll_event) -> void
    {
      (void)a_epoll_event;

      buffer.push('C');
    });

    buffer.push('B');
  };/*}}}*/

  fd_array_0[0].fd_update(EPOLLOUT,true,callback_1);

  epoll_0.wait(-1);
  epoll_0.wait(-1);
  epoll_0.wait(-1);

  fd_0 = std::move(fd_array_0[0]);
  fd_0.fd_update(EPOLLOUT,true,nullptr);
  epoll_0.wait(-1);
  epoll_0.wait(-1);

  fd_0.fd_update(EPOLLOUT,true,callback_0);
  epoll_0.wait(-1);
  epoll_0.wait(-1);

  fd_0.fd_update(EPOLLIN,true,nullptr);
  fd_0.fd_update(0,true,nullptr);

  int counter = 0;
  auto callback_2 = [&buffer,&counter](const epoll_event &a_epoll_event) -> void
  {/*{{{*/
    (void)a_epoll_event;

    ccl::fd_s::timer_read(a_epoll_event.data.fd);

    buffer.push('T');
    ++counter;
  };/*}}}*/

  ccl::epoll_fd_s timer_0(ccl::fd_s::timer({{0,1000000},{0,1000000}},0));
  timer_0.fd_update(EPOLLIN,true,callback_2);

  do {
    epoll_0.wait(-1);
  } while(counter < 10);

  cassert(strcmp(buffer.push('\0').data(),"AABCCAATTTTTTTTTT") == 0);
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

