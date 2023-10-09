#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int n_nodes = 2;


void read_pipe()
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
            if (write(pipes[i][WRITE_END], &i, sizeof(int*))==-1) {
                printf("error escribiendo al pipe\n");
                exit(1);
            }
            close(pipes[i][WRITE_END]);
        } else {
            /* caso del proceso hijo*/
            if (read(pipes[i][READ_END], &my_node, sizeof(int*))==-1) {
                printf("error leyendo el pipe\n");
                exit(1);
            }
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