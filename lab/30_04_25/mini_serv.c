#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Write the 9 global variables
int count = 0, max_fd = 0;
fd_set wfds, rfds, afds;
char *msgs[65356];
int ids[65356];
char buf_read[1001], buf_write[42];

int extract_message(char **buf, char **msg) {
  char *newbuf;
  int i; // to track the position of the newline character

  *msg = 0;
  if (*buf == 0)
    return (0); // if the buffer is empty, NULL pointer, we return a 0

  i = 0;
  while ((*buf)[i]) {
    if ((*buf)[i] == '\n') {
      newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
      if (newbuf == 0)
        return (-1); // if calloc fails we return -1
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

void fatal_error() {
  write(2, "Fatal error\n", 12);
  exit(1);
}

void notify_others(int author, char *str) {
  for (int fd = 0; fd <= max_fd; fd++) {
    if (FD_ISSET(fd, &wfds) && fd != author) {
      send(fd, str, strlen(str), 0);
    }
  }
}

void register_client(int fd) {
  max_fd = fd > max_fd ? fd : max_fd;
  ids[fd] = count++;
  msgs[fd] = NULL;
  FD_SET(fd, &afds);
  sprintf(buf_write, "server: client %d just arrived\n", ids[fd]);
  notify_others(fd, buf_write);
}

void remove_client(int fd) {
  sprintf(buf_write, "server: client %d just left\n", ids[fd]);
  notify_others(fd, buf_write);
  free(msgs[fd]);
  FD_CLR(fd, &afds);
  close(fd);
}

void send_msg(int fd) {
  char *msg;
  while (extract_message(&(msgs[fd]), &msg)) {
    sprintf(buf_write, "client %d: ", ids[fd]);
    notify_others(fd, buf_write);
    notify_others(fd, msg);
    free(msg);
  }
}

int create_socket() {
  int sockfd = 0;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    fatal_error();
  max_fd = sockfd;
  FD_SET(sockfd, &afds);
  return sockfd;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    write(2, "Wrong number of arguments\n", 26);
    exit(1);
  }
  FD_ZERO(&afds);
  //   int sockfd, connfd;
  int sockfd = create_socket();
  //   socklen_t len;
  //   struct sockaddr_in servaddr, cli;
  struct sockaddr_in servaddr;

  // socker create and verficiation
  //   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //   if (sockfd == -1) {
  // printf("socket creation failed ...\n"); // This needs to be changed to an
  // error function and exit with 1
  //   exit(0);
  // }
  // else printf("Socket successfully created ...\n"); // No message on success
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
  servaddr.sin_port =
      htons(atoi(argv[1])); // This needs to be changed to atoi(argv[1])

  // Binding newly created socker to given IP and verfication
  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0))
    fatal_error();
  //    {
  // printf("socket bind failed ...\n"); // This needs to be changed to an
  // error function and exit with 1
  // exit(0);
  //   } else
  // printf("Socket successfully binded ...\n");
  if (listen(sockfd, SOMAXCONN) != 0)
    //   { // 10 should be changed to SOMAXCONN
    // printf("cannot listen\n");   // This needs to be changed to an error
    // function and exit with 1
    // exit(0);
    //   }
    fatal_error();
  // Here wll start the while loop
  //   len = sizeof(cli);
  //   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  //   if (connfd < 0) {
  //     printf("server acccept failed ...\n");
  //     exit(0);
  //   } else
  //     printf("server acccept the client ...\n");
  while (1) {
  }
}