#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define READ_END 0
#define WRITE_END 1

sem_t parent_JG, parent_jp;
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

    /* inicializamos el mutex lock */
    sem_init(&parent_JG, 1, 2);
    sem_init(&parent_jp, 1, 2);

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
    
    if (my_node==0) { //JG
        // llenar(JG, 5)
        liters_JG = 5;
        

        // anadir(JG, jp, 3)
        msj = 3;
        liters_JG -= msj;
        
        sem_wait(&parent_JG);
        close(parent_to_child[my_node][READ_END]);
        close(child_to_parent[my_node][READ_END]);
        if (write(child_to_parent[my_node][WRITE_END], &msj, sizeof(int*))==-1){
            printf("uwu\n");
            exit(1);
        }
        close(child_to_parent[my_node][WRITE_END]);
        sem_post(&parent_JG);
        
        printf("JG %d\n", liters_JG);
    }
    sem_post(&parent_JG);

    sem_wait(&parent_JG);
    if (my_node == -1) {
        sem_wait(&parent_JG);
        read_from_child_parent(child_to_parent[JG], &msj);
        sem_post(&parent_JG);

        printf("msj_parent = %d", msj);

        sem_wait(&parent_jp);
        close(parent_to_child[jp][READ_END]);
        if (write(parent_to_child[jp][WRITE_END], &msj, sizeof(int*))==-1){
            printf("hasta cuando papa dios\n");
            exit(1);
        }
        close(parent_to_child[jp][WRITE_END]);
        sem_post(&parent_jp);
    }



    
    if (my_node==1){ //jp
        liters_jp = 0;
        sem_wait(&parent_jp);
        close(parent_to_child[my_node][WRITE_END]);
        if(read(parent_to_child[my_node][READ_END], &msj, sizeof(int*))==-1){
            printf("ausilio\n");
            exit(1);
        }
        close(parent_to_child[my_node][READ_END]);
        sem_post(&parent_jp);
        //read_pipe(pipes[my_node][READ_END], &msj);
        liters_jp += msj;
    }
    
    while (wait(NULL)>0) {
        ;
    }

    
    sem_destroy(&parent_JG);
    sem_destroy(&parent_jp);
    
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

void read_from_child_parent(int child_pipe[], int *value) {
    close(child_pipe[WRITE_END]);
    if (read(child_pipe[READ_END], &value, sizeof(int*))==-1){
        printf("error leyendo pipe\n");
        exit(1);
    }
    close(child_pipe[READ_END]);
}