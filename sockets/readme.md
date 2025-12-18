# Networking Flow: From Connect → Handshake → Accept → Send/Recv

## 1. Server sets up listening socket

```c
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
listen(server_fd, backlog);
// the kernel creates a queue (the listen backlog queue) for incoming connections. This queue is associated with the listening socket (server_fd).
```

### What happens inside the kernel

- socket(): creates an empty socket object in kernel space. No buffers yet.
- bind(): tells kernel “packets destined for this (IP, port) should be delivered to this socket.”
- listen(): marks socket as passive. Kernel allocates two important queues:
  - SYN queue (a.k.a. incomplete connection queue): holds client connections that sent SYN but not yet completed handshake.
  - accept queue (completed connection queue): holds fully established tcp connections waiting for accept().
  - accept queue's size limit(backlog) is what you set in listen(server_fd, backlog).
  - SYN queue is managed entirely inside the kernel’s TCP stack. You don’t control its length directly; it’s bounded by system parameters (somaxconn, tcp_max_syn_backlog, etc.).

## 2. Client initiates connection

```c
int client_fd = socket(AF_INET, SOCK_STREAM, 0);
connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); // blocks until handshake complete
```

### What happens inside the kernel. TCP 3-way handshake (kernel-managed)

- Client kernel sends SYN packet (srcIP:srcPort → dstIP:dstPort).
- Server kernel receives SYN → creates a new socket (not visible to user yet). Puts it in SYN queue.
- Server replies with SYN+ACK.
- Client replies with ACK. Handshake complete.
- At this moment:
  - Server kernel moves socket from SYN queue → accept queue.
  - Client connect() unblocks, and client_fd is now connected.
- connect() → blocking until the 3-way handshake completes (SYN → SYN-ACK → ACK).

## 3. Server accepts the connection

```c
struct sockaddr_storage client_addr;
socklen_t len = sizeof(client_addr);
int conn_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
```

### What happens inside the kernel

- Kernel takes the first connection from the accept queue(the one that completed handshake).
- Creates a new file descriptor (conn_fd) for that client.
- Returns conn_fd to user.
- Fills client_addr with client’s IP/port.
- Now:
  - server_fd: still listening, waiting for more.
  - conn_fd: dedicated to this one client that just connected.
- accept() is blocking if there are no pending connections in the listen queue. It will wait until a client finishes the TCP handshake and is ready. When unblocked, it returns a new conn_fd.
- If the accept queue is full: New clients trying to connect may get ECONNREFUSED or time out.

## 4. Buffers involved

Every TCP socket (both client and server side) has two kernel-space buffers:

- Send buffer: stores data the app wrote with send() but hasn’t yet been ACKed by the peer.
- Receive buffer: stores data received from the peer but not yet read with recv().
- Both client and server have their own pair of buffers.
- When you call accept() on the server, the new socket (conn_fd) gets its own separate send/receive buffers. This is because each client connection is its own TCP stream, with its own state machine, sequence numbers, and flow control.
- Per-client buffers:
   So if you have 100 connected clients, you’ll have 100 separate conn_fd sockets.
   Each one has:
  - Its own send buffer.
  - Its own receive buffer.
This allows the server to talk to each client independently.
- send() doesn’t guarantee network transmission — it just moves data into the kernel send buffer.
- recv() doesn’t guarantee to read everything sent — it just drains whatever is currently in the kernel receive buffer.
- The TCP stack (inside kernel) manages retransmissions, ordering, segmentation, ACKs, flow control, and congestion control.

## 5. Client sends data

```c
send(client_fd, "Hello", 5, 0);
```

### What happens

- Data is copied from user-space buffer ("Hello") → client kernel send buffer.
  - This copy is immediate. That’s why send() can return quickly.
- TCP segments the data, adds headers, and gives it to NIC for transmission.
- As data is ACKed by the server, it’s removed from the client’s send buffer.
- Kernel send buffer: temporary storage until data is ACKed.
- send()/write(): Usually non-blocking in practice as long as there’s room in the send buffer. If the send buffer is full (because the peer isn’t ACKing fast enough), send() will block until space frees up.

## 6. Server receives data

```c
char buf[1024];
int n = recv(conn_fd, buf, sizeof(buf), 0);
```

### What happens

- Server kernel’s receive buffer already contains “Hello” (arrived via TCP).
- recv() copies from kernel receive buffer → user buffer buf.
- Bytes are removed from the receive buffer once copied.
- Kernel receive buffer: holds incoming data until your program reads it by recv().
- recv() / read(): Blocking if the receive buffer is empty. It will wait until data arrives or the connection closes.

## 7. Echo example

```c
send(conn_fd, buf, n, 0);   // server send "Hello" back
```

- Same as before: copy from user space → kernel send buffer specific to the conn_fd.
- TCP ensures delivery.

```c
recv(client_fd, buf, sizeof(buf), 0); // Client receives reply
```

- Copy from client kernel receive buffer → user buffer.

## 8. Closing the connection

- When the client calls close(client_fd):
  - Kernel sends a FIN packet.
  - Server responds with ACK, eventually sends its own FIN.
  - TCP performs a graceful 4-step teardown.
- Buffers are flushed and destroyed.
