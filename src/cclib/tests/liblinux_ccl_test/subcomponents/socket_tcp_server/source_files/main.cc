
#include "main.h"

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  auto srv_sock = ccl::fd_s::sock(AF_INET,SOCK_STREAM);
  srv_sock.sock_listen({"127.0.0.1",8000},10);

  ccl::array<char> message;

  unsigned idx = 0;
  do {
    ccl::socket_address_s address;
    auto conn_sock = srv_sock.sock_accept(&address);

    // - receive message -
    message.reset();
    conn_sock.read(&message);

    // - print message -
    message.push('\0');
    puts(message.data());

    --message.used();
    message.append(11," - Response");

    // - send response -
    conn_sock.write(message.used(),message.data());

  } while(++idx < 10);

  return 0;
}/*}}}*/

