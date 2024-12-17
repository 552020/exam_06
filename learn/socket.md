# Socket()

The arguments passed to `socket` specify the type of socket you want to create. Here's a detailed explanation of the three arguments you're passing:

```c
int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
```

---

### **1. `AF_INET`**

- **Definition**:
  - `AF_INET` stands for **Address Family Internet**.
  - It specifies that the socket will use the **IPv4 protocol** for network communication.
- **Use**:

  - If you want to create a socket for IPv4, you use `AF_INET`.
  - For IPv6, you would use `AF_INET6`.

- **Alternative Address Families**:

  - `AF_UNIX` (or `AF_LOCAL`): For local inter-process communication (IPC) using UNIX domain sockets.
  - `AF_PACKET`: For low-level socket programming (e.g., raw sockets for packet sniffing).
  - `AF_NETLINK`: For kernel/user-space communication.

- **Example**:
  When you create an IPv4 TCP socket for a web server or client, `AF_INET` is what you typically use.

---

### **2. `SOCK_STREAM`**

- **Definition**:

  - `SOCK_STREAM` specifies that the socket will be a **stream socket**, meaning it will provide a reliable, connection-oriented communication channel (e.g., TCP).

- **Features of `SOCK_STREAM`**:

  - **Connection-Oriented**: Ensures a connection is established before data is sent.
  - **Reliable**: Guarantees that data is delivered in order and without loss.
  - **Stream-Based**: Data is treated as a continuous stream of bytes, not discrete packets.

- **Alternative Socket Types**:
  - `SOCK_DGRAM`: For datagram sockets, which provide connectionless communication (e.g., UDP).
  - `SOCK_RAW`: For raw sockets, used to directly access lower-level protocols (e.g., creating your own transport-layer protocols).
  - `SOCK_SEQPACKET`: For connection-oriented, message-based sockets (rarely used).

---

### **3. The `0` Protocol Argument**

- **Definition**:

  - The last argument specifies the **protocol** to use with the socket.
  - A value of `0` means the system will automatically select the default protocol for the specified combination of `AF_INET` and `SOCK_STREAM`.

- **Why Use `0`?**

  - For `AF_INET` and `SOCK_STREAM`, the default protocol is **TCP**.
  - If you were using `AF_INET` and `SOCK_DGRAM`, the default protocol would be **UDP**.

- **Explicit Protocol Values**:
  - Instead of `0`, you can explicitly specify the protocol:
    - For TCP: `IPPROTO_TCP` (value is usually `6`).
    - For UDP: `IPPROTO_UDP` (value is usually `17`).

---

### **Summary of Your Example**

```c
int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
```

- **`AF_INET`**: You’re creating an IPv4 socket.
- **`SOCK_STREAM`**: The socket will use TCP for reliable, connection-oriented communication.
- **`0`**: The system will automatically select TCP as the default protocol.

### **Equivalent Explicit Call**

If you want to make the protocol explicit, you could write:

```c
int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

---

### **Practical Use Example**

Imagine you’re writing a simple TCP server. The `socket` function call with these arguments creates a socket that:

1. Uses IPv4 addressing (e.g., `192.168.1.1`).
2. Uses TCP for a reliable, stream-based communication channel.
3. Automatically sets the protocol to TCP.

If you were writing a UDP server, the call would look like this:

```c
int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
```

Let me know if you’d like more details or an example of how these are used!
