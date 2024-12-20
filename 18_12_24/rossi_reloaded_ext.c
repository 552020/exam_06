#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_SEND_BUFFER_SIZE 1000100
#define MAX_RECV_BUFFER_SIZE 1000100

typedef struct s_client {
  int id;
  //   char msg[290000];
  char msg[1000000];
} t_client;

t_client clients[1024];
fd_set read_set, write_set, all_set;
int max_fd = 0, gid = 0; // What is gid?
// char send_buffer[300000], recv_buffer[300000];
char send_buffer[MAX_SEND_BUFFER_SIZE], recv_buffer[MAX_RECV_BUFFER_SIZE];

void err(char *msg) {
  if (msg)
    write(2, msg, strlen(msg));
  else
    write(2, "Fatal error", 11);
  write(2, "\n", 1);
  exit(1);
}

void send_to_all(int except) {
  for (int fd = 0; fd <= max_fd; fd++) {
    if (FD_ISSET(fd, &write_set) && fd != except)
      if (send(fd, send_buffer, strlen(send_buffer), 0) == -1)
        err(NULL);
  }
}

int main(int argc, char **argv) {
  if (argc != 2)
    err("Wrong number of arguments!");

  struct sockaddr_in addr;
  socklen_t addrlen;
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1)
    err(NULL);
  max_fd = server_fd;

  FD_ZERO(&all_set);
  FD_SET(server_fd, &all_set);
  bzero(clients, sizeof(clients));
  bzero(&addr, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY); // This is wrong!
  addr.sin_port = htons(atoi(argv[1]));

  if (bind(server_fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1 ||
      listen(server_fd, 100) == -1)
    err(NULL);

  while (1) {
    read_set = write_set = all_set;
    if (select(max_fd + 1, &read_set, &write_set, 0, 0) == -1)
      continue;

    for (int fd = 0; fd <= max_fd; fd++) {
      if (FD_ISSET(fd, &read_set)) {
        if (fd == server_fd) {
          int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
          if (client_fd == -1)
            continue;
          if (client_fd > max_fd)
            max_fd = client_fd;
          FD_SET(client_fd, &all_set);
          clients[client_fd].id = gid++;
          sprintf(send_buffer, "server:client %d just arrived\n",
                  clients[client_fd].id);
          send_to_all(client_fd);
        } else {
          // We are here beacause an fd is in the read_set
          // We should have and else branch of the condition checking if there
          // are messages with new lines ready to be sent in the clients arraa
          int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
          if (ret <= 0) {

            sprintf(send_buffer, "server: client %d just left\n",
                    clients[fd].id);
            send_to_all(fd);
            // Before removing the fd we should check if there are remaining
            // messages with new lines.
            FD_CLR(fd, &all_set);
            close(fd);
            bzero(clients[fd].msg, strlen(clients[fd].msg));
            continue;
          } else {
            for (int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++) {
              clients[fd].msg[j] = recv_buffer[i];
              if (clients[fd].msg[j] == '\n') {
                clients[fd].msg[j] = '\0';
                sprintf(send_buffer, "client %d: %s\n", clients[fd].id,
                        clients[fd].msg);
                // Simple length check before using sprintf
                // if (strlen(send_buffer) + 20 < MAX_SEND_BUFFER_SIZE) {
                //  sprintf(send_buffer, "client %d: %s\n", clients[fd].id,
                //          clients[fd].msg);

                send_to_all(fd);
                bzero(clients[fd].msg, strlen(clients[fd].msg));
                j = -1;
              }
            }
          }
        }
        // This break interrupts the for loop, and we go back to the select. It
        // means we will handle 1 event at a time.
        break;
      }
    }
  }
  return (0);
}
