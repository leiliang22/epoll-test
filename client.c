#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define SERVER_PORT 5000
#define INPUTBUFFER_SIZE 1024
#define _EPOLL_TRACE_ 1

int connectToServer(char* ip, int port);

int main(int argc, char* argv[]) {
    char ip[20] = "127.0.0.1";
    int sfd_client;
    sfd_client = connectToServer(ip, SERVER_PORT);
#if _EPOLL_TRACE_
    printf("sfd_client = %d.\n", sfd_client);
#endif
    
    while(sfd_client > 0) {
        printf("input: ");
        char inputBuf[INPUTBUFFER_SIZE] = "";
        fgets(inputBuf, INPUTBUFFER_SIZE, stdin);
        if(send(sfd_client, inputBuf, sizeof(inputBuf), 0) < 0) {
            perror("send failed:");
        }
    }

    return 0;
}

int connectToServer(char* ip, int port) {
    //填充服务器信息
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    int sfd;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed:");
        return -1;
    }

    //连接服务器端
    if(connect(sfd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        perror("connect failed:");
        return -1;
    }

#if _EPOLL_TRACE_
    printf("epollClient finished.\n");
#endif

    return sfd;
}
