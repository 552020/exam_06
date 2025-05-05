#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typdef struct s_client
{
	int id;
	char msg[290000];
} t_cient;

t_cient client[1024];
fd_set read_set, write_set, all_set;
int max_fd = 0, gid = 0;
char send_buffer[300000], recv_buffer[3000000];

void err(char *msg)
{
	if (msg)
		write(2, msg, strlen(msg));
	else 
		write(2, "Fatal error", 11);
	write(2, "\n", 1);
	exit(1);
}

void send_to_all(int except)
{
	for(int fd = 0; fd <= max_fd, fd++)
		if(FD_ISSET(fd, &write_set) &&fd != except)
			if(send(fd, send_buffer, strlen(send_buffer), 0) == -1)
				err(NULL);
}

int main(int argc, char **argv)
{
 if(argc != 2)
 	err("Wrong number of arguments!");

	struct sockaddr_in addr;
	socklen addrlen;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		err(NULL);
	max_fd = server_fd;

	FD_ZERO(&all_set);
	FD_SET(server_fd, &all_set);
	bzero(clients, sizeof(client));
	bzero(&addr, sizeof(addr));
}
				
