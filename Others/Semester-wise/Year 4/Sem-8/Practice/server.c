#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUF_SIZE 2048

void error(const char *err){
    printf("Error: %s\n",err);
    exit(1);
}

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    sfd = socket(result->ai_family, result->ai_socktype,
            result->ai_protocol);
    if (sfd == -1) error("Socket");
    

    if (bind(sfd, result->ai_addr, result->ai_addrlen) < 0) error("Bind");

    if(listen(sfd, 5) <0) error("Listen");

    printf("Server started on port ... %s\n", argv[1]);

    int client_fd = accept(sfd, NULL, NULL);

    while(1){
        char req[BUF_SIZE];
        read(client_fd, req, sizeof(req));
        printf("\nClient - %s\n", req);

        send(client_fd,"Received",8,0);
    }

    close(client_fd);
    close(sfd);
}