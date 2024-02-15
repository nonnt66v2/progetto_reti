//
// Created by nonnt66 on 22/10/23.
//

#ifndef RETI_WRAPPER_H
#define RETI_WRAPPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>

/* COMMENTI IN "LIB.C" */

/// genera un socket
/// \param famiglia AF_INET IPV4
/// \param tipo SOCK_STREAM UDP-TCP
/// \param protocollo DI SOLITO 0 (VA CON FAMIGLIA E TIPO)
/// \return
int getSocket(int dominio, int tipo, int protocollo);

/// converte la stringa "indirizzo" in un indirizzo di rete e lo memorizza
/// in "servaddr.sin_addr.s_addr"
/// \param famiglia
/// \param indirizzo dato da linea di comando (di solito 127.0.0.1)
/// \param servaddr struct (composta da 3 elementi)
///                         \1) sin_family = famiglia
///                         \2) sin_port = porta
///                         \3) sin_addr = struct 1)s_addr = indirizzo IP network
void setInet(int tipo, char *indirizzo, struct sockaddr_in servaddr);


/// connette il socket "sockfd" al "servaddr"
/// \param sockfd socket accettato nella  getSocket senza errori
/// \param servaddr struct
/// \param servSize dimensione in byte di "servaddr"
void connessioneSocket(int sockfd, struct sockaddr_in servaddr, ssize_t serverSize);


///
/// \param sockfd socket
/// \param buffer sul quale andiamo a scrivere le informazioni lette dal server
/// \param bufferSize dimensione del buffer
void Read(int sockfd, char buffer[], ssize_t bufferSize);


///
/// \param connfd socket della connessione acquisita da "listenfd"
/// \param buffer buffer dal quale prelevare le informazioni da scrivere
/// \param bufferSize dimensione del buffer
void Write(int connfd, const void *buffer, ssize_t bufferSize);


/// genera un socket per l'ascolto
/// \param famiglia
/// \param tipo
/// \param protocollo
/// \return
int getListenSocket(int famiglia, int tipo, int protocollo);

/// binda l'indirizzo da "servaddr" a "listenfd" per poi mettere "listenfd" in ascolto
/// \param listenfd socket sulla quale viene bindato l'indirizzo contenuto in "servaddr"
/// \param servaddr struct che contiene l'indirizzo
/// \param servSize dimensione in byte del "servaddr"
void bindListener(int listenfd, struct sockaddr_in servaddr, ssize_t serverSize);

/// genera una socket, la precedente ("listenfd") rimane in ascolto e la nuova ("connfd") viene usata per le operazioni (lettura,scrittura,etc...)
/// \param listenfd socket che viene usata per gestire la nuova connessione e rimane in ascolto restituendone un'altra
/// \param clientaddr indirizzo del client (può essere NULL)
/// \param clientAddrSize size del addrClientAddr (può essere NULL)
/// \return
int accettaConnessione(int listenfd, struct sockaddr *clientaddr, socklen_t *clientAddrSize);


/// la funzione in caso di errore comunica i caratteri mancanti da scrivere e ne restituisce il numero così da poter effettuare una nuova write su questi ultimi
/// \param fd socket accettata dal Listener
/// \param buf buffer dal quale estrarre le informazioni da scrivere
/// \param count dimensione del buffer
/// \param nwritten  numero di caratteri scritti
/// \param nleft numero di caratteri rimanenti
/// \return
ssize_t FullRead(int fd, void *buf, size_t count);

/// esattamente come la FullWrite ma con la differenza che legge al posto di scrivere
/// \param fd
/// \param buf
/// \param count
/// \return
ssize_t FullWrite(int fd, const void *buf, size_t count);

/// funzione per impostare la porta lato server (evita errori
/// del tipo "bind: address already in use")
/// \return numero porta
int setPorta();

/// funzione per ottenere la porta alla quale collegarsi lato
///client
/// \return numero porta
int getPorta();

#endif //RETI_WRAPPER_H
