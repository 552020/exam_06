#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>     // exit,
#include <sys/socket.h> // socket, bind
#include <unistd.h>     // write

int main(int argc, char **argv) {
  // 1. We create a socket
  if (argc != 2) {
    write(2, "Error\n", 7);
    exit(1);
  }
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    write(2, "Fatal error\n", 13);
    exit(1);
  }
  printf("Socket created with fd: %d\n", sock_fd);
  // 2. We bind the socket to a port
  // 2.0 We convert the port from a string to an int.
  int port = atoi(argv[1]);

  struct sockaddr_in addr;
  // We could also memset the struct to 0.
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  //   addr.sin_addr.s_addr = htonl(INDDR_LOOPBACK); // Host-to-Network Long,
  //   just to convert endian order addr.sin_addr.s_addr = htonl("127.0.0.1");
  //   // This is wrong
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (bind(sock_fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
    write(2, "Fatal error\n", 13);
    exit(1);
  };
  printf("Socket bound to IP 127.0.0.1, and port %d\n", port);

  close(sock_fd);
  return 0;
}