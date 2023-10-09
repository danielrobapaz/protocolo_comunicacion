#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int n_nodes = 2;


int main() {
    int parent_to_child[n_nodes][2], child_to_parent[n_nodes][2];
    int ch_pid, my_node, msj;
    int liters_JG, liters_jp, JG, jp;

    JG = 0;
    jp = 1;

    my_node = -1;
    /* inicializamos los pipes*/
    for (int i=0; i<n_nodes; i++) {
        if (pipe(parent_to_child[i])) {
            printf("error en la creacion del pipe.\n");
            exit(1);
        }

        if (pipe(child_to_parent[i])) {
            printf("error en la creacion del pipe. \n");
            exit(1);
        }
    }

    for (int i=0; i<n_nodes; i++) {
        /* creamos a los hijos */
        ch_pid = fork();
        int liters=0;

        if (ch_pid) {
            /* caso del proceso padre*/
            write_from_parent_child(parent_to_child[i], &i);
        } else {
            /* caso del proceso hijo*/
            read_from_parent_child(parent_to_child[i], &i);
            
            break; /* se corta del ciclo para evitar generar hijos de mas*/
        }
    }
    
    if (my_node == -1) {
        close(child_to_parent[0][WRITE_END]);
        if (read(child_to_parent[0][READ_END], &msj, sizeof(int*))==-1){
            printf("no se que poner\n");
            exit(1);
        }
        close(child_to_parent[0][READ_END]);

        printf("msj = %d", msj);

        close(parent_to_child[1][READ_END]);
        if (write(parent_to_child[1][WRITE_END], &msj, sizeof(int*))==-1){
            printf("hasta cuando papa dios\n");
            exit(1);
        }
        close(parent_to_child[1][WRITE_END]);
        //read_pipe(pipes[0][READ_END], &msj);
        //write_pipe(pipes[1][WRITE_END], &msj);
    }


    if (my_node==0) { //JG
        // llenar(JG, 5)
        liters_JG = 5;

        msj = 3;
        liters_JG -= msj;
        
        close(parent_to_child[my_node][READ_END]);
        close(child_to_parent[my_node][READ_END]);
        if (write(child_to_parent[my_node][WRITE_END], &msj, sizeof(int*))==-1){
            printf("uwu\n");
            exit(1);
        }
        close(child_to_parent[my_node][WRITE_END]);

        //write_pipe(pipes[my_node][WRITE_END], &msj);
        
        printf("JG %d\n", liters_JG);
    }

    
    if (my_node==1){
        liters_jp = 0;
        close(parent_to_child[my_node][WRITE_END]);
        if(read(parent_to_child[my_node][READ_END], &msj, sizeof(int*))==-1){
            printf("ausilio\n");
            exit(1);
        }
        close(parent_to_child[my_node][READ_END]);
        //read_pipe(pipes[my_node][READ_END], &msj);
        liters_jp += msj;
    }
    
    while (wait(NULL)>0) {
        ;
    }

    return 0;
}

void write_from_parent_child(int parent_pipe[], int *value) {
    close(parent_pipe[READ_END]);
    if (write(parent_pipe[WRITE_END], value, sizeof(int*))==-1) {
        printf("error escribiendo el pipe\n");
        exit(1);
    }
}


void read_from_parent_child(int parent_pipe[], int *value){
    close(parent_pipe[WRITE_END]);
    if (read(parent_pipe[READ_END], value, sizeof(int*))==-1){
        printf("error leyendo el pipe\n");
        exit(1);
    }

    close(parent_pipe[READ_END]);
}