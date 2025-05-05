#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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
  if (newbuf = 0)
    return (0);
  newbuf[0] = 0;
  if (buf != 0)
    strcat(newbuf, buf);
  free(buf);
  strcat(newbuf, add);
  return (newbuf);
}

int main() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socker create and verficiation
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed ...\n"); // This needs to be changed to an
                                            // error function and exit with 1
    exit(0);
  } else
    printf("Socket successfully created ...\n"); // No message on success
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
  servaddr.sin_port = htons(8081); // This needs to be changed to atoi(argv[1])

  // Binding newly created socker to given IP and verfication
  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) !=
       0)) {
    printf("socket bind failed ...\n"); // This needs to be changed to an error
                                        // function and exit with 1
    exit(0);
  } else
    printf("Socket successfully binded ...\n");
  if (listen(sockfd, 10) != 0) { // 10 should be changed to SOMAXCONN
    printf("cannot listen\n"); // This needs to be changed to an error function
                               // and exit with 1
    exit(0);
  }

  // Here wll start the while loop
  len = sizeof(cli);
  connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  if (connfd < 0) {
    printf("server acccept failed ...\n");
    exit(0);
  } else
    printf("server acccept the client ...\n");
}