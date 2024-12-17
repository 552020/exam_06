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
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (bind(sock_fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
    write(2, "Fatal error\n", 13);
    close(sock_fd);
    exit(1);
  };
  printf("Socket bound to IP 127.0.0.1, and port %d\n", port);
  // 3. listen
  if (listen(sock_fd, 1024) < 0) {
    write(2, "Fatal error\n", 13);
    close(sock_fd);
    exit(1);
  }
  printf("Socket listening to IP 127.0.0.1, and port %d\n", port);
  // 4. Accept

  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);
  if (client_fd < 0) {
    write(2, "Fatal error\n", 13);
    close(sock_fd);
    exit(1);
  }

  // Print client information
  char client_ip[INET_ADDRSTRLEN];
  //   inet_ntop(): Converts the client's IP address from binary to a
  //   human-readable string.
  inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
  //   ntohs(): Converts the client's port number from network byte order to
  //   host byte order.
  int client_port = htohs(client_addr.sin_port);

  printf("Accepter connection from client IP: %s, Port %d\n", client_ip,
         client_port);

  const char *msg = "Welcome to the server!\n";
  send(client_fd, msg, strlen(msg), 0);

  close(client_fd);
  close(sock_fd);
  return 0;
}