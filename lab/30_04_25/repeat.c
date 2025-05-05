#include <stdlib.h>
#include <sys/select.h>

int max_fd = 0, count = 0;
int sockfd;
int ids[65536];
char *msgs[65356];
fd_set wfds, rfds, afds;
char buf_write[42], buf_read[1000];

void notify_others(int author, char *str) {
  for (int fd = 0; fd <= max_fd; fd++) {
    if (FD_ISSET(fd, &wfds) && fd != author)
      send(sockfd, str, strlen(str), 0);
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
  if (msgs[fd])
    free(msgs[fd]);
  FD_CLR(fd, &afds);
  close(fd);
}

void send_msg(int fd) {
  char *msg;

  while (extract_message(&(msgs[fd]), &msg)) {
    sprintf(buf_write, "client %d:", ids[fd]);
    notify_others(fd, buf_write);
    notify_others(fd, msg);
    free(msg);
  }
}