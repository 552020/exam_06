#include <sys/socket.h> // socket
#include <stdio.h> // printf
#include <unistd.h> // write
#include <stdlib.h> // exit
// #include <netinet/in.h>
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


	close(server_fd);
	return (0);
}
