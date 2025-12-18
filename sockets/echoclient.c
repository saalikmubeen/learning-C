#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <getopt.h>
#include <arpa/inet.h>

/* Be prepared accept a response of this length */
#define BUFSIZE 1024

#define USAGE                                                                       \
    "usage:\n"                                                                      \
    "  echoclient [options]\n"                                                      \
    "options:\n"                                                                    \
    "  -s                  Server (Default: localhost)\n"                           \
    "  -m                  Message to send to server (Default: \"Hello Spring!!\")\n" \
    "  -p                  Port (Default: 39483)\n"                                  \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"port", required_argument, NULL, 'p'},
    {"server", required_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {"message", required_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}};

/* Main ========================================================= */
int main(int argc, char **argv)
{
    int option_char = 0;
    unsigned short portno = 39483;
    char *hostname = "localhost";
    char *message = "Hello Fall!!";

    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "s:p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'p': // listen-port
            portno = atoi(optarg);
            break;
        case 'm': // message
            message = optarg;
            break;
        case 's': // server
            hostname = optarg;
            break;
        case 'h': // help
            fprintf(stdout, "%s", USAGE);
            exit(0);
            break;
        default:
            fprintf(stderr, "%s", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }

    if (NULL == message) {
        fprintf(stderr, "%s @ %d: invalid message\n", __FILE__, __LINE__);
        exit(1);
    }

    if (NULL == hostname) {
        fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
        exit(1);
    }

    /* Socket Code Here */

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv;
    char portstr[10];
    sprintf(portstr, "%d", portno);

    // for a client, getaddrinfo resolves hostnames/IPs into usable socket addresses for connecting.
    // The OS does a DNS lookup for hostname.
    // after the getaddrinfo(), servinfo now contains a list of possible sockaddrs (IP:port pairs).
    // What remote addresses can I connect to for hostname on port portstr?
    if ((rv = getaddrinfo(hostname, portstr, &hints, &servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }

    // loop through all the results and connect to the first we can
    int client_socket_fd;
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
      if ((client_socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      {
        perror("socket");
        continue;
      }

      // If we  don’t call bind() on a client socket, the OS chooses an ephemeral port and suitable
      // local IP automatically when you call connect()
      if (connect(client_socket_fd, p->ai_addr, p->ai_addrlen) == -1)
      {
        perror("connect");
        close(client_socket_fd);
        continue;
      }

      break;
    }

    if (p == NULL)
    {
      fprintf(stderr, "client: failed to connect\n");
      return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    // Send message to the server
    ssize_t bytes_sent = send(client_socket_fd, message, strlen(message), 0);
    if (bytes_sent < 0)
    {
      perror("send");
      close(client_socket_fd);
      exit(EXIT_FAILURE);
    }
    else if (bytes_sent < strlen(message))
    {
      fprintf(stderr, "Only %zd bytes sent out of %zu\n", bytes_sent, strlen(message));
      close(client_socket_fd);
      exit(EXIT_FAILURE);
    }

    // Receive the echoed message from the server
    char buffer[BUFSIZE];
    ssize_t bytes_received = recv(client_socket_fd, buffer, BUFSIZE - 1, 0);
    if (bytes_received <= 0)
    {
      perror("recv");
      close(client_socket_fd);
      exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0'; // Add null terminator to make it a valid string
    printf("%s", buffer);

    // Close the socket for the client
    close(client_socket_fd);
    return 0;
}
