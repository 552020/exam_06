#include <sys/socket.h> // socket
#include <stdio.h> // printf
#include <unistd.h> // write
#include <stdlib.h> // exit
#include <netinet/in.h> // sockaddr_in, htons, INADDR_LOOPBACK
// 1. Write and compile an Hello World program in C

// 2. Substitute the printf function with the write function 
// 2.1. Understand what happens with the null terminator at the end of the string. Do we need to "print" it or not?
// 2.2. Understand what happens if the value of the bites to write is bigger of the actual string
// 2.3. The compilier automatically adds a null terminator at the end of the string, if it's built on the fly

// 3. Add argc and argv to the program and check for errors. In the mini serv we will pass the number of the port the mini serv will listen to. > - If no argument is given, it should write in stderr "Wrong number of arguments" followed by a \n and exit with status 1
// 3.1. Can we use strlen? No. 
// 4. Create a listening server server_fd
// 4.1. Create a socket --> socket() // man 2 socket  
// 4.1.1 Remember AF_INET as first argument, SOCK_STREAM as second argument and 0 as third argumetn.
// 4.2. Bind the socket --> bind()
// 4.3. Listen to a port --> listen()
// 5. Accept a connection --> accept()

// 6. Use a nc to start a client --> nc IP PORT --> nc 127.0.0.1 8080
// 7. Implement select

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "Wrong number of arguments\n", 26);
		exit(1);
	}
	// printf("Hello World!\n");
	// write(1, "Hello World!\n", 12); // Without new line
	write(1, "Hello World!\n", 13); // With new line
	// write(1, "Hello World!\n", 14); // With null terminator
	// write(1, "Hello World!\n", 15); // Passed null terminator

	int server_fd = 0;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		write(2, "Fatal error\n", 12);
		exit(1);
	}
	printf("Socket created with fd %d\n", server_fd);

	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int port = atoi(argv[1]);
	
	// You can find this definition in the man pages, just look for 'man -K sockaddr_in'
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(port);
	size_t addrlen = sizeof(addr);

	if (bind(server_fd, (const struct sockaddr *) &addr, addrlen) < 0)
	{
		write(2, "Fatal error\n", 12);
		exit(1);
	}
	printf("Socket bound to IP 127.0.0.1 and port %d\n", port);

	if (listen(server_fd, 128) < 0)
	{
		write(2, "Fatal error\n", 12);
		exit(1);
	}

	printf("Socket listening ...\n");
	fd_set read_fds, write_fds, all_fds;
	int max_fd = server_fd;
	while(1)
	{
		read_fds = write_fds = all_fds;
		select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
		for (int fd = 0; fd <= max_fd; fd++)
		{
			if (FD_ISSET(fd, &read_fds))
			{
				if (fd == server_fd)
				{
					// ... means new client wants connect!
					// 1. Create a new client socket with accept()
					// 2. Add the new socket to all_fds with FD_SET
					// 3. Update the max_fd if the new fd is > than max_fd
					// 4. Broadcast a message to all other client sockets
				}
				else
				{
					// ... means an existing client want to talk!
					// 1. Listen to it with recv
					// ssize_t recv(int sockfd, void *buf, size_t len, int flags);
					ssize_t bytes = 0;
					char buff[1024];
					bytes = recv(fd, buff, sizeof(buff), 0);
					if (bytes =< 0)
					{
						// -1 is an error --> close connection
						// 0 orderly shut down --> close connection
						// 1. Close connection
						// 2. Broadcast messag to all other clients, the client left 
						// 3. Remove the fd from all_fds FD_CLR
						// 4. Update max_fd if client_fd > max_fd
					}
					else 
					{
						// 1. Broadcast the message to all other clients
					}
				}


			}
		}

		// select(int nfds, fd_set *readfds, fd_set *writefds, fd_set, *exceptfds, struct timeval *timeout);

	}

	struct sockaddr_in client_addr;
	int client_fd;
	socklen_t client_addrlen = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addrlen) ;
	if (client_fd == -1)
	{
		printf("Something went wrong while creating the client socket\n");
	}
	int client_ids = 0;
	char client_arrived[50];
	sprintf(client_arrived, "server: client %d just arrived\n");
	send(client_fd, client_arrived, 50, 0);
	client_ids++;
	char msg[1024];
	recv(client_fd, msg, sizeof(msg), 0);
	printf("msg: %s\n", msg);
	recv(client_fd, msg, sizeof(msg), 0);
	printf("msg: %s\n", msg);
	// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

	close(client_fd);
	close(server_fd);
	return (0);
}
