
#include "main.h"

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  auto sock = ccl::fd_s::sock(AF_INET,SOCK_DGRAM);
  sock.sock_bind({"127.0.0.1",8001});

  ccl::array<char> message;
  ccl::socket_address_s src_addr;

  do
  {
    // - receive message -
    message.reset();
    sock.sock_recvfrom(&message,&src_addr);
    message.push('\0');

    // - print message -
    puts(message.data());

    // - if end message was received -
    if (strcmp(message.data(),"END") == 0)
    {
      break;
    }
  } while(true);

  return 0;
}/*}}}*/

