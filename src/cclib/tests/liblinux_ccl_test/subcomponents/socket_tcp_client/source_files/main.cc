
#include "main.h"

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::socket_address_s addr("127.0.0.1",8000);

  const std::string data_message = "Message from socket_tcp_client";
  ccl::array<char> message;

  unsigned idx = 0;
  do {
    auto sock = ccl::fd_s::sock(AF_INET,SOCK_STREAM);
    sock.sock_connect(addr);

    // - send message -
    sock.write(data_message.length(),data_message.data());

    // - receive response -
    message.reset();
    sock.read(&message);

    // - print response -
    message.push('\0');
    puts(message.data());

  } while(++idx < 10);

  return 0;
}/*}}}*/

