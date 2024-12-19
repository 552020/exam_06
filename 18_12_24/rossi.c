#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct s_client
{
	int id;
	char msg[290000];
} t_client;

t_client clients[1024];
fd_set read_set, write_set, all_set;
int max_fd = 0, gid = 0; // What is gid?
char send_buffer[300000], recv buffer[300000]

void err(char *msg)
{
	if(msg)
		write(2, msg, strlen(msg));
	else
		write(2, "Fatal error", 11);
	write (2, "\n", 1);
	exit(1);
}

void send_to_all(int except)
{
	for (int fd = 0; fd <= max_fd; fd++)
	{
		if (FD_ISSET(fd, &write_set) && fd != except)
			if (send(fd, send_buffer, strlen(send_buffer), 0) == -1
				err(NULL);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		err("Wrong number of arguments!");

	struct sockaddr_in addr;
	socklen_t addrlen;
	int server_fd = socker(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) err(NULL)
	max_fd = server_fd;

	FD_ZERO(&all_set); 
	FD_SET(server_fd, &all_set);
	bzero(clients_sizeof(clients));
	bzero(&addr, sizeof(addr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY) // This is wrong!
	serveraddr.sin_port = htons(atoi(argv[1]);

	if (bind(server_fd), (const struct sockaddr *)&addr, sizeof(addr)) == -1 || listen(server_fd, 100) == -1) err(NULL);

	while(1)
	{
		read_set = write_set = all_set;
		select(max_fd + 1, &read_set, &write_set, 0, 0) == -1;

		for (int fd = 0, fd <= max_fd; fd++)
		{
			if (FD_ISSET(fd, &read_set)
			{
				if (fd == server_fd)
				{
					int client_fd = accept(server_fd, (struct sockaddr *)&addr, *addrlen);
					if (client_fd == -1)
				}
			}
		}

	}
}

