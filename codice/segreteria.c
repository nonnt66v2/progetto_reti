//
// Created by nonnt66 on 14/02/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "./libreria/wrapper.h"

#define PORT 12345
#define MAXLINE 1024


int main(){
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096]={'a','b','c'};
    time_t ticks;
    listenfd = getListenSocket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    /// accetta qualsiasi indirizzo
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ///porta sulla quale andiamo ad operare
    servaddr.sin_port = htons(setPorta());
    bindListener(listenfd, servaddr, sizeof(servaddr));
    if (listen(listenfd, 1024) < 0) {
        perror("listen");
        exit(1);
    }
    while(1) {
        printf("in attesa\n");
        connfd = accettaConnessione(listenfd,(struct sockaddr*)NULL,(void*)NULL);
        printf("client accettato\n");
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd,buff,sizeof(buff));
        close(connfd);
    }


}

