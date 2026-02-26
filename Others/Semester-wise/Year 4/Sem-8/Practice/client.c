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
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

        sfd = socket(result->ai_family, result->ai_socktype,
                    result->ai_protocol);
        if (sfd == -1) error("Socket");

        if (connect(sfd, result->ai_addr, result->ai_addrlen) < 0) error("Connect");

        while(1){
            char input[BUF_SIZE];
            memset(input, 0, sizeof(input));
            fgets(input, BUF_SIZE, stdin);
            send(sfd, input, sizeof(input), 0);

            char x[BUF_SIZE];
            memset(x, 0, sizeof(x));
            read(sfd, x, sizeof(x));
            printf("\nServer - %s\n", x);
        }

        close(sfd);
}