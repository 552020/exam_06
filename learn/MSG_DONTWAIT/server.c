// Man 3 Server - "Normal" send from man 3 - Blocking

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/syscall.h> // For SYS_sendto
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

int main()
{
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);
	ssize_t bytes_sent;

	// Create the server socket
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
	{
		perror("Error creating socket");
		exit(1);
	}
	printf("Server socket created\n");

	int send_buf_size;
	int recv_buf_size;
	socklen_t optlen = sizeof(int);

	// Get send buffer size
	if (getsockopt(server_sock, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &optlen) <
		0)
	{
		perror("getsockopt SO_SNDBUF failed");
	}
	else
	{
		printf("Send buffer size: %d bytes\n", send_buf_size);
	}

	// Get receive buffer size
	if (getsockopt(server_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &optlen) <
		0)
	{
		perror("getsockopt SO_RCVBUF failed");
	}
	else
	{
		printf("Receive buffer size: %d bytes\n", recv_buf_size);
	}

	// Set up the server address struct
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost
	server_addr.sin_port = htons(PORT);

	// Bind the socket to the address
	if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
		0)
	{
		perror("Error binding socket");
		exit(1);
	}
	printf("Socket bound to address\n");
	// Listen for incoming connections
	if (listen(server_sock, 1) < 0)
	{
		perror("Error listening on socket");
		exit(1);
	}

	printf("Server listening on port %d...\n", PORT);

	// Accept the client connection
	client_sock =
		accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
	if (client_sock < 0)
	{
		perror("Error accepting connection");
		exit(1);
	}

	printf("Client connected! I'll take a nap, before pinging the client!\n");

	// Sleep for a few seconds to let the client fill the buffer
	//   sleep(2); // Adjust this sleep time based on the client sending rate
	int sleep_time = 3;
	for (int i = 0; i < sleep_time; i++)
	{
		printf("%d\n", sleep_time - i);
		sleep(1);
	}

	char buffer[6] = "Ping!";
	// Send a message to the client (the server will block here if the buffer is
	// full)
	printf("Just before Ping send!\n");
	bytes_sent = send(client_sock, buffer, sizeof(buffer), 0);
	printf("Just after Ping send!\n");
	if (bytes_sent < 0)
	{
		perror("Error sending data");
		exit(1);
	}

	printf("Server sent: %s\n", buffer);

	//   char large_message[262144] =
	//       "256KB "; // 256KB - It didn't work
	//   char large_message[524288] =
	//   "512KB "; // 512KB - It didn't work
	// Create an even larger message - 1MB
	//   char large_message[1048576] =
	//       "1MB "; // 1MB - It didn't work
	// Create a massive message - 4MB - It worked!
	//   char large_message[4194304] =
	//       "This is a large message meant to fill the buffer. "; // 4MB

	size_t large_message_size = 4194304;
	char *large_message = malloc(large_message_size); // 4MB
	if (large_message == NULL)
	{
		perror("Failed to allocate memory");
		exit(1);
	}
	//   strcpy(large_message, "This is a large message meant to fill the buffer.
	//   ");
	printf("Just before memset\n");
	fflush(stdout);
	memset(large_message, 'X', large_message_size - 1); // Fill with 'X's
	large_message[large_message_size - 1] = '\0';
	printf("Just after memset\n");
	fflush(stdout);
	printf("Attempting to send large buffer (%zu bytes)...\n",
		   large_message_size);
	fflush(stdout);
	/* Blocking send */
	//   bytes_sent = send(client_sock, large_message, sizeof(large_message), 0);
	//   if (bytes_sent < 0) {
	//     perror("Error sending data");
	//     exit(1);
	//   }
	// printf("Server sent %zd bytes of large message\n", bytes_sent);
	//   printf("We will probably never reach this point. And not even the point "
	//          "above!\n");

	/* Non-blocking send */
	for (int i = 0; i < 2; i++)
	{
		printf("Iteration %d\n", i);

		// bytes_sent =
		// 	send(client_sock, large_message, large_message_size, MSG_DONTWAIT);

		/* Direct syscall with MSG_DONTWAIT */
		bytes_sent = syscall(SYS_sendto, client_sock, large_message,
							 large_message_size, MSG_DONTWAIT, NULL, 0);
		printf("Bytes sent: %zd\n", bytes_sent);
		if (bytes_sent < 0)
		{
			printf("Error number: %d\n", errno);
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf(
					"Socket buffer is full! Would block on send (EAGAIN/EWOULDBLOCK)\n");
			}
			else
			{
				perror("Error sending data");
				exit(1);
			}
		}
		else
		{
			printf("Server sent %zd bytes of large message\n", bytes_sent);
		}
	}
	printf("We reached this point because we used MSG_DONTWAIT!\n");
	free(large_message);

	// Close the client socket and server socket
	close(client_sock);
	close(server_sock);

	return 0;
}
