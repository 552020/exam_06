

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> // exit,
#include <string.h>
#include <sys/socket.h> // socket, bind
#include <unistd.h>     // write

int main(int argc, char **argv) {
  // 1. Check arguments
  if (argc != 2) {
    write(2, "Wrong number of arguments\n", 27);
    exit(1);
  }

  // 2. Create the server socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    write(2, "Fatal error\n", 13);
    exit(1);
  }
  printf("Socket created with fd: %d\n", server_fd);

  // 3. Bind the socket to a port
  int port = atoi(argv[1]);

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(server_fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
    write(2, "Fatal error\n", 13);
    close(server_fd);
    exit(1);
  };
  printf("Socket bound to IP 127.0.0.1, and port %d\n", port);

  // 4. Listen for incoming connections
  if (listen(server_fd, 1024) < 0) {
    write(2, "Fatal error\n", 13);
    close(server_fd);
    exit(1);
  }
  printf("Socket listening to IP 127.0.0.1, and port %d\n", port);
  // 4. Prepare for select()

  fd_set read_fds;
  int max_fd = server_fd; // Keep track of the highest file descriptor

  FD_ZERO(&read_fds);           // Initialize the fd_set
  FD_SET(server_fd, &read_fds); // Add the servers socket to the set

  // 6. Main server loop
  while (1) {
    fd_set tmp_fds =
        read_fds; // Temporary copy of the FD set, cause select will modify it.

    // Call select to monitor file descriptors
    if (select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) < 0) {
      //   write(2, "Select Error\n", 13);
      //   exit(1);
      continue; // So Rossi
    }
    // 6.1 Check if the server socket is ready (new incoming connection)
    if (FD_ISSET(server_fd, &tmp_fds)) {
      struct sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);
      int client_fd =
          accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
      if (client_fd < 0) {
        // write(2, "Fatal error\n", 13);
        continue; // The server should continue to listen also if the createing
                  // of the clinet faisl
      }
      // Print client information
      char client_ip[INET_ADDRSTRLEN];
      //   inet_ntop(): Converts the client's IP address from binary to a
      //   human-readable string.
      inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
      //   ntohs(): Converts the client's port number from network byte order to
      //   host byte order.
      int client_port = ntohs(client_addr.sin_port);

      printf("Accepted connection from client IP: %s, Port %d\n", client_ip,
             client_port);
      // This will be replaced with a message broadcastet to all
      const char *msg = "Welcome to the server!\n";
      send(client_fd, msg, strlen(msg), 0);

      // Add the new client to the monitored file descriptors
      FD_SET(client_fd, &read_fds);
      if (client_fd > max_fd)
        max_fd = client_fd;
    }
    // 6.2 Check clien sockets for incoming data
    for (int fd = server_fd + 1; fd <= max_fd; fd++) {
      if (FD_ISSET(fd, &tmp_fds)) {
        char buffer[1024];
        int bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
          // Connection cloese or error
          printf("Clien fd %fd disconnected.\n", fd);
          close(fd);
          FD_CLR(fd, &read_fds);
        } else {
          // Print received message and echo it
          buffer[bytes_read] = '\0';
          printf("Received from fd %d: %s", fd, buffer);
          send(fd, buffer, bytes_read, 0);
        }
      }
    }
  }

  close(server_fd);
  return 0;
}