#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<errno.h>

#define BUF_SIZE 256
char SOCKET_NAME[BUF_SIZE];

static int sfd;

void cleanup_socket (int sockfd, char *SOCKET_NAME) {
    close(sockfd);
    unlink(SOCKET_NAME);
}

void sighandler(int signum) {
    cleanup_socket(sfd, SOCKET_NAME);
    exit(0);
}

int main(void) {
    printf("Enter name of socket: ");
    fgets(SOCKET_NAME, BUF_SIZE, stdin);
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        perror("Socket call error");
        return errno;
    }

    struct sockaddr_un sock_addr;
    sock_addr.sun_family = AF_UNIX;

    strncpy(sock_addr.sun_path, SOCKET_NAME, strlen(SOCKET_NAME));
    if (bind(sfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
        perror("bind call error");
        cleanup_socket(sfd, SOCKET_NAME);
        return errno;
    }
    
    if (signal(SIGINT, sighandler) == SIG_ERR) {
        perror("signal call error");
        cleanup_socket(sfd, SOCKET_NAME);
        return errno;
    }

    char msg[BUF_SIZE];
    for(;;) {
        size_t bytes = recv(sfd, msg, sizeof(msg)-1, 0);
        if (bytes == -1) {
            perror("recv call error");
            cleanup_socket(sfd, SOCKET_NAME);
            return errno;
        }
        msg[bytes] = '\0';
        printf("Got message: %s\n", msg);
    }
    return 0;
}