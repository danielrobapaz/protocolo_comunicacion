#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int n_nodes = 2;

void write_pipe(int n_pipe, int *value_write) {
    if (write(n_pipe, value_write, sizeof(int*))==-1) {
        printf("error escribiendo al pipe\n");
        exit(1);
    }
    close(n_pipe);
}

void read_pipe(int n_pipe, int *value_read) {
    if (read(n_pipe, value_read, sizeof(int*))==-1) {
        printf("error leyendo el pipe\n");
        exit(1);
    }
}

int main() {
    int pipes[n_nodes][2], ch_pid, my_node;

    my_node = -1;
    /* inicializamos los pipes*/
    for (int i=0; i<n_nodes; i++) {
        if (pipe(pipes[i])) {
            printf("error en la creacion del pipe.\n");
            exit(1);
        }
    }

    for (int i=0; i<n_nodes; i++) {
        /* creamos a los hijos */
        ch_pid = fork();

        if (ch_pid) {
            /* caso del proceso padre*/
            write_pipe(pipes[i][WRITE_END], &i);
        } else {
            /* caso del proceso hijo*/
            read_pipe(pipes[i][READ_END], &my_node);
            break; /* se corta del ciclo para evitar generar hijos de mas*/
        }
    }

    if (my_node==1){
        printf("soy el nodo 1");
    }


    if (my_node==0) {
        printf("soy el nodo 2");
    }

    while (wait(NULL)>0) {
        ;
    }

    return 0;
}