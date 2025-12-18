#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

#define USAGE                                                        \
    "usage:\n"                                                         \
    "  echoserver [options]\n"                                         \
    "options:\n"                                                       \
    "  -m                  Maximum pending connections (default: 5)\n" \
    "  -p                  Port (Default: 39483)\n"                    \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"help",          no_argument,            NULL,           'h'},
    {"maxnpending",   required_argument,      NULL,           'm'},
    {"port",          required_argument,      NULL,           'p'},
    {NULL,            0,                      NULL,             0}
};


int main(int argc, char **argv) {
    int portno = 39483; /* port to listen on */
    int option_char;
    int maxnpending = 5;

    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'h': // help
            fprintf(stdout, "%s ", USAGE);
            exit(0);
            break;
        case 'p': // listen-port
            portno = atoi(optarg);
            break;
        case 'm': // server
            maxnpending = atoi(optarg);
            break;
        default:
            fprintf(stderr, "%s ", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }
    if (maxnpending < 1) {
        fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__, maxnpending);
        exit(1);
    }


  /* Socket Code Here */


    struct sockaddr_storage client_addr; // client's address information
    socklen_t client_addr_len;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // assign the address of my local host to the socket structures

    int rv;
    char portstr[10];
    sprintf(portstr, "%d", portno);
    // getaddrinfo -> Give me all the local network addresses I can bind to, for port
    // So the list getaddrinfo returns are local addresses (your own machine’s interfaces)
    // that are usable for binding a socket.
    // getaddrinfo is a translator between human-readable stuff (like "localhost", "example.com", "8080")
    // and the raw binary socket addresses (struct sockaddr) that socket(), bind(), and connect() actually use.
    // It produces a linked list of 'struct addrinfo' results that contain socket-ready addresses,
    // based on what you ask for in hints.
    // Each result('struct addrinfo) has inside it a 'struct sockaddr *ai_addr' that you can pass to bind()(server) or
    // connect()(client).
    // The struct addrinfo has a field called ai_addr which is a pointer to a struct sockaddr.
    // This is a pointer to a struct sockaddr that contains the binary address(like IP + port).
    // If IPv4 : it’s really a struct sockaddr_in *
    // If IPv6 : it’s really a struct sockaddr_in6 *
    // This is what you pass directly to bind() or connect().

    // struct addrinfo
    // {
    //   int ai_flags;             // Options (e.g. AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST)
    //   int ai_family;            // Address family (AF_INET for IPv4, AF_INET6 for IPv6, AF_UNSPEC for either)
    //   int ai_socktype;          // Socket type (SOCK_STREAM for TCP, SOCK_DGRAM for UDP)
    //   int ai_protocol;          // Specific protocol (IPPROTO_TCP, IPPROTO_UDP) or 0 to let system choose
    //   socklen_t ai_addrlen;     // Length of ai_addr in bytes
    //   char *ai_canonname;       // Canonical (official) name of the host, if requested
    //   struct sockaddr *ai_addr; // Pointer to a socket address struct (IP + port)
    //   struct addrinfo *ai_next; // Next node in linked list of results
    // };

    // NULL + AI_PASSIVE tells the OS:
    // “Give me an address I can bind() to on any local interface, for this port.”
    // server will accept connections on all network interfaces (loopback, WiFi,
    // Ethernet, etc.) at the given port.
    if ((rv = getaddrinfo(NULL, portstr, &hints, &servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }

    int server_socket_fd;
    // loop through all the results and bind to the first we can
    // serveinfo is a linkedlist of all possible addresses(struct addrinfo)
    for (p = servinfo; p != NULL; p = p->ai_next)
    {

      // create a server socket
      if ((server_socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      {
        perror("socket");
        continue;
      }

      int yes = 1;
      // used to prevent "Address already in use" error from bind() call when you
      // restart the server quickly after killing it
      if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
      {
        perror("setsockopt");
        exit(EXIT_FAILURE);
      }

      // bind the server socket to the port and IP address returned by getaddrinfo
      // bind() associates a socket file descriptor (created by socket()) with a specific local address.
      // That local address includes:
      // IP address → which network interface you are binding to
      // Port number → which port on that interface
      if (bind(server_socket_fd, p->ai_addr, p->ai_addrlen) == -1)
      {
        close(server_socket_fd);
        perror("bind");
        continue;
      }

      break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
      fprintf(stderr, "server: failed to bind\n");
      exit(EXIT_FAILURE);
    }

      // listen on the sever socket for incoming connections
      if (listen(server_socket_fd, maxnpending) < 0)
      {
        perror("listen");
        close(server_socket_fd);
        exit(EXIT_FAILURE);
      }

      // printf("Server listening on port %d\n", portno);

      // Keep accepting incoming connections from clients
      while (1)
      {
        client_addr_len = sizeof client_addr;
        // Accept the incoming connection from the client
        int client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

        // When the connection is established, the kernel knows:
        // The client’s IP address(source IP).The client’s port(source port).It writes this info into
        // the memory you gave it(client_addr).
        // So after the accept() call:
        // client_addr contains the remote client’s address. We can use it to printf the client’s IP.

        if (client_socket_fd < 0)
        {
          perror("accept");
          continue;
        }

        // Handle client connection
        char buffer[BUFSIZE]; // buffer to store the data received from the client

        // Read the data sent by the client into the buffer.
        ssize_t received_n = recv(client_socket_fd, buffer, sizeof(buffer), 0);
        if (received_n < 0)
        {
          perror("recv");
          close(client_socket_fd);
          continue;
        }

        // Echo the same received message back to the client
        ssize_t sent_n = send(client_socket_fd, buffer, received_n, 0);
        if (received_n != sent_n)
        {
          printf("Error sending data back to the client");
          continue;
        }
        close(client_socket_fd);
      }

      return 0;
    }
