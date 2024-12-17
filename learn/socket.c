#include <stdio.h>
#include <stdlib.h>     // exit,
#include <sys/socket.h> // socket
#include <unistd.h>     // write

int ft_putchar(char c, int fd) {
  int ret = 0;
  ret = write(fd, &c, sizeof(char));
  return ret;
}

void ft_error(char *str) {
  for (int i = 0; str[i] != '\0'; ++i) {
    ft_putchar(str[i], 2);
  }
}

int main() {
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) { // or == -1
    //   if (sock_fd) {
    // perror("socket");
    ft_error("Fatal error\n");
    exit(1);
  }
  printf("Socket created with fd: %d\n", sock_fd);
  // Without bind, the socket is not tied to a port, so it can't communicate
  // yet.
  close(sock_fd);
  return 0;
}