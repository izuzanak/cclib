
#include "main.h"

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  auto sock = ccl::fd_s::sock(AF_INET,SOCK_DGRAM);
  sock.sock_bind({"127.0.0.1",8000});

  ccl::socket_address_s trg_addr("127.0.0.1",8001);

  const std::string data_message = "Message from socket_udp_sendto";
  const std::string end_message = "END";

  unsigned idx = 0;
  do {

    // - send data message -
    sock.sock_sendto(trg_addr,data_message.length(),data_message.data());
  } while(++idx < 10);

  // - send end message -
  sock.sock_sendto(trg_addr,end_message.length(),end_message.data());

  return 0;
}/*}}}*/

