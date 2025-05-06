#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// 9 global variables
fd_set wfds, rfds, afds;
int max_fd = 0, count = 0;
int ids[65536];
char *msgs[65536];
char buf_write[42], buf_read[1001]; // Probably I will need to increase these.

// 6 functions
// void fatal_error() - 2 lines
// create_socket() - 5 lines
// register_client(int fd) - 6 lines
// remove_client(int fd) - 5 lines
// notify_others(int author, char *str) - 3 lines
// send_msg(int fd) - 6 lines

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

void notify_others(int author, char *str);

void fatal_error() {
  write(2, "Fatal error\n", 12);
  exit(1);
}

int create_socket() {
  max_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (max_fd == -1)
    fatal_error();
  FD_SET(max_fd, &afds);
  return max_fd;
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
  free(msgs[fd]);
  msgs[fd] = NULL;
  FD_CLR(fd, &afds);
  sprintf(buf_write, "server: client %d just left\n", ids[fd]);
  notify_others(fd, buf_write);
  close(fd);
}

void notify_others(int author, char *str) {
  for (int fd = 0; fd <= max_fd; fd++)
    if (FD_ISSET(fd, &wfds) && fd != author)
      send(fd, str, strlen(str), 0);
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
int main(int argc, char **argv) {
  if (argc != 2) {
    write(2, "Wrong number of arguments\n", 26);
    exit(1);
  }

  FD_ZERO(&afds);
  // socket create and verification
  //   int sockfd, connfd, len;
  int sockfd = create_socket();
  //   struct sockaddr_in servaddr, cli;
  struct sockaddr_in servaddr;
  socklen_t len = sizeof(servaddr);

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
  servaddr.sin_port = htons(atoi(argv[1]));

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    fatal_error();
  if (listen(sockfd, SOMAXCONN) != 0)
    fatal_error();

  //   len = sizeof(cli);
  //   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  //   if (connfd < 0) {
  //     printf("server acccept failed...\n");
  //     exit(0);
  //   } else
  //     printf("server acccept the client...\n");
  while (1) {
    // A
    wfds = rfds = afds;
    // B
    if (select(max_fd + 1, &rfds, &wfds, 0, 0) == -1)
      continue;
    // C
    for (int fd = 0; fd <= max_fd; fd++) {
      if (!FD_ISSET(fd, &rfds))
        continue;
      // D1 - accept
      if (fd == sockfd) {
        int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
        register_client(client_fd);
        break;
      } else {
        int bytes_read = recv(fd, buf_read, 1000, 0);
        if (bytes_read <= 0) {
          remove_client(fd);
          break;
        } else {
          buf_read[bytes_read] = '\0';
          msgs[fd] = str_join(msgs[fd], buf_read);
          send_msg(fd);
          break;
        }
        // D2 - recv
      }
    }
  }
  return 0;
}