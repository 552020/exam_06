#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int client_sock;
  struct sockaddr_in server_addr;
  char message[1024];
  ssize_t bytes_received;

  // Create the client socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (client_sock < 0) {
    perror("Error creating socket");
    exit(1);
  }

  int send_buf_size;
  int recv_buf_size;
  socklen_t optlen = sizeof(int);

  // Get send buffer size
  if (getsockopt(client_sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &optlen) <
      0) {
    perror("getsockopt SO_SNDBUF failed");
  } else {
    printf("Send buffer size: %d bytes\n", send_buf_size);
  }

  // Get receive buffer size
  if (getsockopt(client_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &optlen) <
      0) {
    perror("getsockopt SO_RCVBUF failed");
  } else {
    printf("Receive buffer size: %d bytes\n", recv_buf_size);
  }

  // Set up the server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost
  server_addr.sin_port = htons(PORT);

  // Connect to the server
  if (connect(client_sock, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0) {
    perror("Error connecting to server");
    exit(1);
  }

  printf("Connected to server. Sending data to fill the buffer, while server "
         "taking a nap ...\n");

  // Fill the socket's send buffer by sending a lot of data
  //   char fill_buffer[16384];
  char fill_buffer[65536]; // Increased to 64KB
  const char *msg = "DATA";
  size_t msg_len = strlen(msg);
  for (int i = 0; i < sizeof(fill_buffer) - 1; i += msg_len) {
    size_t remaining = sizeof(fill_buffer) - 1 - i;
    size_t copy_len = (remaining < msg_len) ? remaining : msg_len;
    memcpy(fill_buffer + i, msg, copy_len);
  }
  fill_buffer[sizeof(fill_buffer) - 1] = '\0'; // Null-terminate the string

  size_t total_bytes_sent = 0;
  int sends_to_fill =
      (send_buf_size / sizeof(fill_buffer)) + 100; // +100 for good measure

  printf("Attempting to fill the buffer with  %zu bytes!\n",
         (size_t)sends_to_fill * sizeof(fill_buffer));

  for (int i = 0; i < sends_to_fill; i++) {
    ssize_t bytes_sent = send(client_sock, fill_buffer, sizeof(fill_buffer), 0);
    if (bytes_sent < 0) {
      perror("Error sending data");
      exit(1);
    }
    total_bytes_sent += bytes_sent;
    printf("Client sent chunk #%d (%zd bytes, total: %zu)\n", i + 1, bytes_sent,
           total_bytes_sent);
  }
  printf("We will probably never reach this point\n");

  // Receive the message from the server (which will block)
  bytes_received = recv(client_sock, message, sizeof(message), 0);
  if (bytes_received < 0) {
    perror("Error receiving data");
    exit(1);
  }

  printf("Client received: %s\n", message);

  // Close the client socket
  close(client_sock);

  return 0;
}
