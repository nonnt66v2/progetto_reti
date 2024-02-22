//
// Created by nonnt66 on 22/10/23.
//

#include "wrapper.h"




int getSocket(int famiglia, int tipo, int protocollo) {
    int sockfd;
    if ((sockfd = socket(famiglia, tipo, protocollo)) < 0) {
        fprintf(stderr, "socket error\n");
        exit(1);
    }
    return sockfd;
}

void setInet(int famiglia, char *indirizzo, struct sockaddr_in servaddr) {
    if (inet_pton(famiglia, indirizzo, &servaddr.sin_addr) < 0) {
        fprintf(stderr, "inet_pton error for %s\n", indirizzo);
        exit(1);
    }
}

void connessioneSocket(int sockfd, struct sockaddr_in servaddr, ssize_t servSize) {
    if (connect(sockfd, (struct sockaddr *) &servaddr, servSize) < 0) {
        fprintf(stderr, "connect error\n");
        exit(1);
    }
}

void Read(int sockfd, char buffer[], ssize_t bufferSize) {
    int n;
    while ((n = read(sockfd, buffer, bufferSize)) > 0) {
        buffer[n] = 0;
        if (fputs(buffer, stdout) == EOF) {
            fprintf(stderr, "fputs error\n");
            exit(1);
        }
    }
    if (n < 0) {
        fprintf(stderr, "read error\n");
        exit(1);
    }
};

void Write(int connfd, const void *buffer, ssize_t bufferSize) {
    if (write(connfd, buffer, bufferSize) < 0) {
        perror("write");
        exit(1);
    }
}

int getListenSocket(int famiglia, int tipo, int protocollo) {
    int listenfd;
    if ((listenfd = socket(famiglia, tipo, protocollo)) < 0) {
        perror("listen socket");
        exit(1);
    }
    return listenfd;
}

void bindListener(int listenfd, struct sockaddr_in servaddr, ssize_t servSize) {
    if (bind(listenfd, (struct sockaddr *) &servaddr, servSize) < 0) {
        perror("bind");
        exit(1);
    }
}

int accettaConnessione(int listenfd, struct sockaddr *clientaddr, socklen_t *clientAddrSize) {
    int connfd;
    if ((connfd = accept(listenfd, (struct sockaddr *) &clientaddr, (socklen_t *) &clientAddrSize)) < 0) {
        perror("accept");
        exit(1);
    }
    return connfd;
};

ssize_t FullWrite(int fd, const void *buf, size_t count) {
    size_t nleft;
    ssize_t nwritten;
    nleft = count;
    while (nleft > 0) {
        if ((nwritten = write(fd, buf, count)) > 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("system call");
            }
            nleft -= nwritten;
            buf += nwritten;
        } else {
            perror("write");
        }
    }
    return (nleft);
}

ssize_t FullRead(int fd, void *buf, size_t count) {
    size_t nleft;
    ssize_t nread;
    nleft = count;
    while (nleft > 0) {
        if ((nread = read(fd, buf, nleft) < 0)) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("read");
            }
        } else if (nread == 0) { break; }
        nleft -= nread;
        buf += nread;
    }
    buf = 0;
    return (nleft);
}

int setPorta() {
    int porta;
    FILE *fp = fopen("porta/porta.txt", "r+");
    fscanf(fp, "%d", &porta);
    rewind(fp);
    porta++;
    fprintf(fp, "%d\n", porta);
    printf("porta: %d\n", porta);
    fclose(fp);
    return porta;
}

int getPorta() {
    int porta;
    FILE *fp = fopen("porta/porta.txt", "r+");
    fscanf(fp, "%d", &porta);
    printf("porta: %d\n", porta);
    fclose(fp);
    return porta;
}
/// funzione per collegarsi al database
/// \param conn
/// \param host
/// \param user
/// \param passwd
/// \param db
/// \param port
/// \param unix_socket
/// \param clientflag
void connessioneDB(MYSQL *conn, const char *host, const char *user, const char *passwd, const char *db, unsigned int port,
              const char *unix_socket, unsigned long clientflag) {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() fallita\n");
        exit(1);
    }

    /**
     * Connessione vera e propria al database MySQL specificato da host, user, password e nome dello schema.
     */
    if (mysql_real_connect(conn, host, user, passwd, db, port, unix_socket, clientflag) == NULL) {
        fprintf(stderr, "mysql_real_connect() fallita: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    else{
        printf("Connessione al database avvenuta con successo\n");
    }
}
