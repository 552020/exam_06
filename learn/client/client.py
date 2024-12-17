import socket


def main():
    # Create a TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Server address (127.0.0.1:8080)
    server_address = ("127.0.0.1", 8080)

    try:
        # Connect to the server
        client_socket.connect(server_address)
        print("Connected to server!")
        print(f"Local socket: {client_socket.getsockname()}")

        # Send a message to the server
        message = "Hello from Python client!\n"
        client_socket.sendall(message.encode())

        # Receive a response from the server
        response = client_socket.recv(1024)
        print("Server replied:", response.decode())

    except Exception as e:
        print("Error:", e)

    finally:
        # Close the socket
        client_socket.close()


if __name__ == "__main__":
    main()
