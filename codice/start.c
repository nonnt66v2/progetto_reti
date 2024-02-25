#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/***
 * Esegue uno script in un nuovo terminale
 * @param script_path
 * @return
 */
void *execute_script(void *script_path);


int main() {
    pthread_t threads[3];

    /*** Array di stringhe contenente i percorsi degli script ***/
    char *script_paths[3] = {
            "./script/startServer.sh",
            "./script/startSegreteria.sh",
            "./script/startStudente.sh"
    };

    /*** Creazione dei thread ***/
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, execute_script, (void *)script_paths[i]) != 0) {
            fprintf(stderr, "Errore durante la creazione del thread per lo script %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
        sleep(1); // Aspettiamo un secondo prima di creare un nuovo thread
    }

    /*** Attesa del completamento dei thread ***/
    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Errore durante l'attesa del completamento del thread per lo script %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }
    /*** Tutti i thread hanno completato l'esecuzione ***/
    return 0;
}
void *execute_script(void *script_path) {

    pid_t pid = fork(); // Creiamo un nuovo processo
    if (pid == 0) {
        // Questo è il processo figlio, eseguiamo lo script
        execlp("x-terminal-emulator", "x-terminal-emulator", "-e", "bash", "-c", script_path, NULL);
        // Se siamo qui, c'è stato un errore nell'esecuzione dello script
        perror("Errore nell'esecuzione dello script");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Errore nella creazione del processo figlio
        perror("Errore nella creazione del processo");
        exit(EXIT_FAILURE);
    }
    return NULL;
}
