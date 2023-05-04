#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#ifndef _SOCKET_H_
#define _SOCKET_H_
#define PORT 9877
#define MAX_LEN_BUF 256
#define TRUE 1
#define FALSE 0
#endif


int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    struct sockaddr_in serv_addr =
        {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(PORT)
        };
    char buf[MAX_LEN_BUF];
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket < 0) {
        perror("Error: socket() failed");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error: connect() failed: %d", errno);
        return -1;
    }

    int bytes;
    char bufServer[BUFSIZ];
    int BUF_SIZE = 256;
    char SOCKET_NAME[BUF_SIZE];
    

    printf("Enter name of socket: ");
    fgets(SOCKET_NAME, BUF_SIZE, stdin);
    if (send(sock, SOCKET_NAME, strlen(SOCKET_NAME) + 1, 0) < 0) {
        perror("Error: sendto() failed");
        return -1;
    }
    printf("Name of socket sending!\n");
    bytes = recv(sock, bufServer, sizeof(bufServer), 0);
    printf("Server answer: %s\n", bufServer);
    
    close(sock);
    return 0;
}