#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<errno.h>

#define BUF_SIZE 256
char SOCKET_NAME[BUF_SIZE];

int main () {
    printf("Enter name of socket: ");
    fgets(SOCKET_NAME, BUF_SIZE, stdin);

    int sfd = socket(AF_UNIX, SOCK_DGRAM,0);
    if (sfd == -1) {
        perror("socket call error");
        return errno;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;

    strcpy(addr.sun_path, SOCKET_NAME);
    printf("sun_path=%s, len=%ld\n", addr.sun_path, strlen(SOCKET_NAME));
    if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror ("Can't set dest address");
        return errno;
    }

    if (send(sfd, SOCKET_NAME, strlen(SOCKET_NAME), 0) == -1) {
        perror("send call error");
        return errno;
    }
    else {
        printf("send message: %s\n", SOCKET_NAME);
    }
    close (sfd);
    return 0;
}