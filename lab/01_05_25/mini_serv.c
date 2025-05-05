#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int count = 0, max_fd = 0;
fd_set rfds, wfds, afds;
int ids[65536];
char *msgs[65536];
char buf_write[42], buf_read[1000];

int extract_message(char **buf, char **msg) {
  char *newbuf;
  int i;

  *msg = 0;
  if (*buf == 0)
    return (0);
  i = 0;
  while ((*buf)[i]) {
    if ((*buf)[i] == '\n') {
      newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
      if (newbuf == 0)
        return (-1);
      strcpy(newbuf, *buf + i + 1);
      *msg = *buf;
      (*msg)[i + 1] = 0;
      *buf = newbuf;
      return (1);
    }
    i++;
  }
  return (0);
}

char *str_join(char *buf, char *add) {
  char *newbuf;
  int len;

  if (buf == 0)
    len = 0;
  else
    len = strlen(buf);
  newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
  if (newbuf == 0)
    return (0);
  newbuf[0] = 0;
  if (buf != 0)
    strcat(newbuf, buf);
  free(buf);
  strcat(newbuf, add);
  return (newbuf);
}

// 6 functions:
// void fatal_error(); // 2 lines
// int create_socket(); // 5 lines
// void register_client(int fd); // 6 lines
// void remove_client(int fd); // 5 lines
// void notify_others(int author, char *str); // 3 lines
// void send_msg(int fd); // 6 lines

void fatal_error() {
  write(2, "bla\n", 3);
  exit(1);
}

int create_socket() {
  max_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (max_fd < 0)
    fatal_error();
  FD_SET(max_fd, &afds);
  return max_fd;
}

void notify_others(int author, char *str) {

  for (int fd = 0; fd <= max_fd; fd++) {
    if (FD_ISSET(fd, &wfds) && fd != author) {
      send(fd, str, sizeof(str), 0);
    }
  }
}

void register_client(int fd) {
  max_fd = fd > max_fd ? fd : max_fd;
  FD_SET(fd, &afds);
  ids[fd] = count++;
  msgs[fd] = NULL;
  sprintf(&buf_write, "server: client %d just arrived\n", ids[fd]);
  notify_others(fd, buf_write);
}

// remove client

void remove_client(int fd) {
  FD_CLR(fd, &afds);
  free(msgs[fd]);
  sprintf(&buf_write, "server: client %d just left\n", ids[fd]);
  notify_others(fd, buf_write);
  close(fd);
}

// send_msg

void send_msg(int fd) {
  char *msg;

  while (extract_message(&(msgs[fd]), &msg)) {
    sprintf(&buf_write, "client %d: ", fd);
    notify_others(fd, buf_write);
    notify_others(fd, msg);
    free(msg);
  }
}

int main(int argc, char **argv) {
  // SECTION A
  if (argc != 2)
    write(2, "bla\n", 4);
  //   int sockfd, connfd, len;
  //   struct sockaddr_in servaddr, cli;

  // socket create and verification
  //   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // SECTION B  - Server Socket Creation
  int sockfd = create_socket();
  if (sockfd < 0)
    fatal_error();
  //   if (sockfd == -1) {
  //     printf("socket creation failed...\n");
  //     exit(0);
  //   } else
  //     printf("Socket successfully created..\n");

  // SECTION C  - Bind
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
  servaddr.sin_port = htons(atoi(argv[1]));

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    fatal_error();
  //   {
  // printf("socket bind failed...\n");
  // exit(0);
  //   } else
  // printf("Socket successfully binded..\n");

  // SECTION C  - Listen
  if (listen(sockfd, SOMAXCONN) != 0)
    fatal_error();
  // Into to While
  while (1) {
    // A
    wfds = rfds = afds;
    select(max_fd + 1, &rfds, &wfds, NULL, NULL);
    // B
    for (int fd = 0; fd <= max_fd; fd++) {
      // C
      if (!(FD_ISSET(fd, &rfds)))
        continue;
      if (fd == sockfd) {
        // D 1  - 5 lines
        socklen_t len = sizeof(servaddr);
        int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
        if (client_fd >= 0) {
          register_client(client_fd);
          break;
        }

      } else {
        // D 2
      }
    }
  }
  //   {
  // printf("cannot listen\n");
  // exit(0);
  //   }
  //   len = sizeof(cli);
  //   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  //   if (connfd < 0) {
  // printf("server acccept failed...\n");
  //     exit(0);
  //   } else
  //     printf("server acccept the client...\n");
  return 0;
}