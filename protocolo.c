#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>


#define size_of sizeof(int*) 
#define READ_END 0
#define WRITE_END 1


void write_from_parent_child(int parent_pipe[], int *value);
void read_from_parent_child(int parent_pipe[], int *value);
void write_from_child_parent(int child_pipe[], int parent_pipe[], int *value);
void read_from_child_parent(int child_pipe[], int *value);

int main(int argc, char *argv[]) {
    int parent_to_child[2], child_to_parent[2];
    int ch_pid, msj;
    int liters_JG, liters_jp;
    
    /* inicializamos los pipes*/
    if (pipe(parent_to_child)) {
        printf("error en la creacion de pipe parent_to_child\n");
        exit(1);
    }

    if (pipe(child_to_parent)) {
        printf("error en la creacion de pipe child_to_parent\n");
        exit(1);
    }        
    
    // inicializamos las variables de estado
    liters_JG = 0;
    liters_jp = 0;

    // creamos proceso hijo
    ch_pid = fork();

    if (ch_pid < 0) {
        printf("error creando proceso hijo");
        exit(1);
    }

    if (ch_pid > 0) { // jp
        // anadir(JG, jp, 3)
        read_from_parent_child(parent_to_child, &msj);
        liters_jp += msj;

        // vaciar(jp)
        liters_jp = 0;
        msj = 0;

        // listo_para_recibir()
        write_from_child_parent(child_to_parent, parent_to_child, &msj);

        // transvasar(JG, jp)
        //read_from_child_parent(parent_to_child, &msj);
        //liters_jp += msj;

        printf("jp %d\n", liters_jp);

    } else { // JG
        // llenar(JG, 5)
        liters_JG = 5;

        // anadir(JG, jp, 3)
        msj = 3;
        liters_JG -= msj;
        write_from_parent_child(parent_to_child, &msj);

        // listo_para_recibir()
        read_from_child_parent(child_to_parent, &msj);
        liters_JG += msj;

        // transcasar(JG, jp)
        msj = liters_JG;
        liters_JG -= msj;
        write_from_parent_child(parent_to_child, &msj);

        printf("JG %d\n", liters_JG);
        wait(NULL);
    }

    return 0;
}

void write_from_parent_child(int parent_pipe[], int *value) {
    close(parent_pipe[READ_END]);
    if (write(parent_pipe[WRITE_END], value, size_of)==-1) {
        printf("error escribiendo el pipe parent_to_chid\n");
        exit(1);
    }
}


void read_from_parent_child(int parent_pipe[], int *value) {
    close(parent_pipe[WRITE_END]);
    if (read(parent_pipe[READ_END], value, size_of)==-1){
        printf("error leyendo el pipe parent_to_child\n");
        exit(1);
    }
}

void write_from_child_parent(int child_pipe[], int parent_pipe[], int *value) {
    close(parent_pipe[READ_END]);
    close(child_pipe[READ_END]);
    if (write(child_pipe[WRITE_END], value, size_of) == -1) {
        printf("error escribiendo pipe child_to_parent\n");
        exit(1);
    }
}

void read_from_child_parent(int child_pipe[], int *value) {
    close(child_pipe[WRITE_END]);
        if (read(child_pipe[READ_END], value, size_of) == -1){
            printf("error leyendo pipe child_to_parent\n");
            exit(1);
        }
        close(child_pipe[READ_END]);
}