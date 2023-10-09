#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int n_nodes = 2;

int main() {
    int pipes[n_nodes][2], ch_pid;

    /* inicializamos los pipes*/
    for (int i=0; i<n_nodes; i++) {
        if (pipe(pipes[i])) {
            printf("error en la creacion del pipi.\n");
            exit(1);
        }
    }
    
    for (int i=0; i<n_nodes; i++) {
        /* creamos a los hijos */
        ch_pid = fork();

        if (ch_pid) {
            /* caso del proceso padre*/

        } else {
            /* caso del proceso hijo*/
            break; /* se corta del ciclo para evitar generar hijos de mas*/
        }
    }

    while (wait(NULL)>0) {
        ;
    }

    return 0;
}