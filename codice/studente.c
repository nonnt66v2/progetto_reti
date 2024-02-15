//
// Created by nonnt66 on 15/02/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql/mysql.h>
#include "./libreria/wrapper.h"

#define PORT 12345

MYSQL *conn;

int main(int argc, char **argv){
    int sockfd, n;
    char recvline[1025];
    struct sockaddr_in servaddr;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }
    sockfd=getSocket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(getPorta());
    setInet(AF_INET, argv[1], servaddr);
    connessioneSocket(sockfd,servaddr,sizeof(servaddr));
    Read(sockfd,recvline,sizeof(recvline));
    exit(0);
}

