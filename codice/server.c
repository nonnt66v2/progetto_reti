//
// Created by nonnt66 on 20/02/24.
//
/***
 * Il server universitario si occupa di gestire le richieste della segreteria, che possono essere di due tipi:
 * 1) aggiunta di un appello
 * 2) aggiunta di una prenotazione
 */
#include "libreria/wrapper.h"

int PORT_SERVER = 1026;


void aggiungiAppello(int);

void aggiungiPrenotazione(int);

MYSQL *connection();

int main() {
    int listenfd, connfd = -1;
    int request;
    struct sockaddr_in servaddr;
    /**
     * Utilizzo la system call socket, che prende in input tre parametri di tipo intero, per creare una nuova socket
     * da associare al descrittore "listenfd". I tre parametri in input riguardano, in ordine, il dominio
     * degli indirizzi IP (IPv4 in questo caso), il protocollo di trasmissione (in questo caso TCP), mentre l'ultimo
     * parametro, se messo a 0, specifica che si tratta del protocollo standard.
     */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore nella creazione della socket!");
        exit(1);
    }

    /**
     * Specifico la struttura dell'indirizzo del server tramite i campi di una struct di tipo sockaddr_in.
     * Vengono utilizzati indirizzi IPv4, vengono accettate connessioni da qualsiasi indirizzo e la porta su cui
     * il server risponderà ai client sarà la 1025.
     */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(setPorta());

    /**
     * La system call bind permette di assegnare l'indirizzo memorizzato nel campo s_addr della struct sin_addr, che è
     * a sua volta un campo della struct sockaddr_in (servaddr nel nostro caso), al descrittore listenfd.
     */
    if ((bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        perror("Errore nell'operazione di bind!");
        exit(1);
    }

    /**
     * Mettiamo il server in ascolto, specificando quante connessioni possono essere in attesa di venire accettate
     * tramite il secondo argomento della chiamata.
     */
    if ((listen(listenfd, 1024)) < 0) {
        perror("Errore nell'operazione di listen!");
        exit(1);
    }

    /**
     * Ci serviamo di un "insieme" di descrittori per strutturare la successiva select.
     * In particolare abbiamo read_set che mantiene l'insieme dei descrittori in lettura.
     * La variabile max_fd serve a specificare quante posizioni dell'array di descrittori devono essere controllate
     * all'interno della funzione select.
     */

    printf("Server in esecuzione...\n");
    fd_set read_set;
    int max_fd;
    max_fd = listenfd;

    /**
     * Imposto un ciclo while "infinito" in modo che il server possa servire una nuova connessione,
     * quindi un nuovo client, dopo averne terminata una (di connessione client).
     */
    while (1) {
        /**
         * Ad ogni iterazione reinizializzo a 0 il read_set e vi aggiungo la socket che permette l'ascolto di nuove
         * connessioni da parte della segreteria.
         */
        FD_ZERO(&read_set);
        FD_SET(listenfd, &read_set);

        /**
         * Se il descrittore della socket relativa alla connessione con la segreteria è maggiore di -1 significa che la
         * segreteria è connessa e quindi si aggiunge anche il suo descrittore al read_set.
         */
        if (connfd > -1) {
            FD_SET(connfd, &read_set);
        }

        /**
         * La funzione select restituisce il numero di descrittori pronti.
         */
        if (select(max_fd + 1, &read_set, NULL, NULL, NULL) < 0) {
            perror("Errore nell'operazione di select!");
        }

        /**
         * Si controlla se sono in attesa di essere accettate nuove connessioni.
         */
        if (FD_ISSET(listenfd, &read_set)) {
            /**
             * La system call accept permette di accettare una nuova connessione (lato server) in entrata da un client.
             */
            if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
                perror("Errore nell'operazione di accept!");
            }

            /**
             * Si ricalcola il numero di posizioni da controllare nella select
             */
            if (connfd > max_fd) {
                max_fd = connfd;
            }
        }

        /**
         * Si controlla se la segreteria vuole inviare una nuova richiesta al server universitario.
         */
        if (FD_ISSET(connfd, &read_set)) {
            /**
             * In caso affermativo si effettua la read, sempre se è possibile effettuarla, ossia se non viene
             * chiusa la segreteria.
             */
            if (read(connfd, &request, sizeof(request)) > 0) {
                /**
                 * Se la richiesta della segreteria è 1 si richiama la funzione di aggiunta di un appello, mentre se
                 * è 2 si richiama la funzione di aggiunta di una prenotazione di uno studente per un determinato
                 * appello.
                 */
                if (request == 1) {
                    aggiungiAppello(connfd);
                } else if (request == 2) {
                    aggiungiPrenotazione(connfd);
                }
            }
        }
    }
}

/**
 * Procedura per l'aggiunta di un appello all'interno del database a partire dal nome dell'esame e dalla data passati
 * dalla segreteria.
 * @param connfd socket di connessione con la segreteria (client)
 */
void aggiungiAppello(int connfd) {
    MYSQL *conn = connection();

    char nomeEsame[255] = {0};
    char date[12] = {0};

    /**
     * Leggiamo dalla socket di connessione con la segreteria il nome dell'esame di cui si vuole aggiungere un appello.
     */
    read(connfd, nomeEsame, sizeof(nomeEsame));

    /**
     * Leggiamo dalla socket di connessione con la segreteria la date del nuovo appello.
     */
    read(connfd, date, sizeof(date));


    /***
     * prendo l'id dell'esame a partire dal nome
     */
    char query[500];
    int id_esame;
    snprintf(query, sizeof(query), "select esame.id_esame from esame where esame.nome_esame = '%s'",
             nomeEsame);
    MYSQL_RES *res;
    MYSQL_ROW row;
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Errore nella query: %s\n", mysql_error(conn));
    }

    // Ottieni il risultato della query
    res = mysql_use_result(conn);

    // Leggi il risultato e stampalo
    while ((row = mysql_fetch_row(res)) != NULL) {
        id_esame = atoi(row[0]);
    }

    // Rilascia la memoria del risultato e chiudi la connessione
    mysql_free_result(res);

    /**
     * inserisco il nuovo appello nel database
     */
    snprintf(query, sizeof(query),
             "insert into appello (id_esame, data_appello) VALUES ('%d',STR_TO_DATE('%s', '%%Y-%%m-%%d'));",
             id_esame, date);
    if (mysql_query(conn, query) != 0) {
        if (strstr(mysql_error(conn), "foreign key constraint fails")) {
            const char *err = "non esiste un esame con questo nome!";
            write(connfd, err, strlen(err));
        } else {
            const char *err = mysql_error(conn);
            write(connfd, err, strlen(err));
        }
    } else {
        const char *ins = "inserimento del nuovo appello completato con successo!";
        /**
         * Inviamo l'esito dell'operazione di aggiunta di un appello alla segreteria.
         */
        write(connfd, ins, strlen(ins));
    }

    mysql_close(conn);
}

/**
 * Procedura per l'aggiunta di una prenotazione ad un appello da parte di uno studente all'interno del database,
 * a partire dall'id dell'appello, dalla matricola dello studente che si sta prenotando e dalla data nella quale avviene
 * la prenotazione.
 * @param connfd socket di connessione con la segreteria (client)
 */
void aggiungiPrenotazione(int connfd) {
    MYSQL *conn = connection();

    int id, mat;

    /***
     * Leggiamo dalla socket di connessione con la segreteria l'id dell'appello al quale lo studente vuole prenotarsi.
     */
    read(connfd, &id, sizeof(id));
    read(connfd, &mat, sizeof(mat));

    char query_verifica_prenotazione[500];

    snprintf(query_verifica_prenotazione, sizeof(query_verifica_prenotazione),
             "select a.id_esame, MIN(data_appello) "
             "from appello a "
             "join esame e "
             "join studente s "
             "where a.id_esame = %d "
             "and data_appello > sysdate() "
             "and anno_corso_studente >= e.anno_corso_esame "
             "and s.mat_studente = '%d' "
             "and (select count(*) from supera s where s.mat_studente = '%d' and s.id_esame = %d) = 0 "
             "group by a.id_esame;", id, mat, id, mat);

    if (mysql_query(conn, query_verifica_prenotazione) != 0) {
        fprintf(stderr, "\nmysql_query(query_verifica_prenotazione): %s\n", mysql_error(conn));
        write(connfd, mysql_error(conn), strlen(mysql_error(conn)));
    }

    MYSQL_RES *res_qvp = mysql_store_result(conn);
    if (res_qvp == NULL) {
        fprintf(stderr, "\nmysql_store_result(query_verifica_prenotazione): %s\n", mysql_error(conn));
        write(connfd, mysql_error(conn), strlen(mysql_error(conn)));
    }

    unsigned int rows = mysql_num_rows(res_qvp);
    /***
     * Se non esiste un appello con questo id e questa data allora restituisco un errore
     */
    if (!rows) {
        const char *err = "invalid id or data!";
        write(connfd, err, strlen(err));
    } else {
        MYSQL_ROW row_qvp = mysql_fetch_row(res_qvp);
        int id_esame = strtol(row_qvp[0], NULL, 10);
        char data_appello[12];
        strcpy(data_appello, row_qvp[1]);

        char inserimento[255];
        snprintf(inserimento, sizeof(inserimento),
                 "insert into prenota (mat_studente, id_esame, data_prenotazone) VALUES ('%d', %d, sysdate());",
                 mat, id_esame);
        if (mysql_query(conn, inserimento) != 0) {
            write(connfd, mysql_error(conn), strlen(mysql_error(conn)));
        } else {
            char res[255];
            snprintf(res, sizeof(res),
                     "prenotazione inserita con successo! Il tuo appello si terrà il %s", data_appello);
            write(connfd, res, strlen(res));
        }
    }
    fflush(stdin);
    mysql_close(conn);
}

/***
 * Funzione per la connessione al database MySQL.
 * @return
 */
MYSQL *connection() {
    /**
     * Inizializzazione della connessione MySQL, con la funzione init che restituisce un puntatore a questa struttura.
     */
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() fallita\n");
    }

    /**
     * Connessione vera e propria al database MySQL specificato da host, user, password e nome dello schema.
     */
    if (mysql_real_connect(conn, "localhost", "nonnt66", "password", "universita", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() fallita: %s\n", mysql_error(conn));
        mysql_close(conn);
    }

    return conn;
}