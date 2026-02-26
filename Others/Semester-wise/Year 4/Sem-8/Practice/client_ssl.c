#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define HOST "html.duckduckgo.com"
#define PORT "443"
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
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
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
    if (sfd <=0 ) error("Socket");
    
        if (connect(sfd, result->ai_addr, result->ai_addrlen) < 0) error("Connect");

        // printf("\nOK\n");
        // fflush(stdout);

    // SSL_library_init();
    // SSL_load_error_strings();
    SSL_CTX *ctx;
    ctx = SSL_CTX_new(TLS_client_method());
    SSL *ssl;
    ssl = SSL_new(ctx);
    
    if(SSL_set_fd(ssl, sfd) <= 0) error("SSL_set_fd");
    if(SSL_connect(ssl) < 0) error("SSL_connect");

    printf("Enter query: ");
    fflush(stdout);
    char input[BUF_SIZE];
    memset(input, 0, sizeof(input));
    fgets(input, BUF_SIZE, stdin);
    input[strlen(input)-1] = 0;

    char request[BUF_SIZE];
    memset(request, 0, sizeof(request));
    sprintf(request, "GET /html/?q=%s HTTP/1.1\n"
        "Host: %s\n"
        // "User-Agent: Mozilla/5.0\n"
        "Connection: close\n\n", input, argv[1]
    );
    // printf("ok");
    fflush(stdout);
    if(SSL_write(ssl, request, sizeof(request)) <=0) error("SSL_write");
    // printf("who");
    fflush(stdout);

    char x[BUF_SIZE];
    memset(x, 0, sizeof(x));
    printf("Server - \n");
    fflush(stdout);
    int sz;
    while((sz=SSL_read(ssl, x, sizeof(x))) > 0){
        // x[sz] = 0;
        printf("%s", x);
        fflush(stdout);
        memset(x, 0, sizeof(x));
        break;
    }

    // SSL_shutdown(ssl);
    // SSL_free(ssl);

    // close(sfd);
}