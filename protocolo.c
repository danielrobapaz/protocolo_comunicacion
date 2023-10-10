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
void write_from_child_parent(int child_pipe[], int *value);
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

    printf("Estados iniciales.\n");
    printf("JG: %d, jp: %d\n-------------------------\n", liters_JG, liters_jp);

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
        printf("anadir [recibir] (JG, jp, 3), Estado jp: %d\n-------------------------\n", liters_jp);

        // vaciar(jp)
        liters_jp = 0;
        msj = 0;

        // listo_para_recibir()
        write_from_child_parent(child_to_parent, &msj);

        // transvasar(JG, jp)
        read_from_child_parent(parent_to_child, &msj);
        liters_jp += msj;
        printf("transvasar [recibir] (JG, jp), Estado jp: %d\n-------------------------\n", liters_jp);

        // listo_para_recibir()
        msj = 0;
        write_from_child_parent(child_to_parent, &msj);

        // anadir(JG, jp, 1)
        read_from_parent_child(parent_to_child, &msj);
        liters_jp += msj;
        printf("anadir [recibir] (JG, jp, 1), Estado jp: %d\n-------------------------\n", liters_jp);

    } else { // JG
        // llenar(JG, 5)
        liters_JG = 5;
        printf("llenar(JG, 5), Estado JG: %d\n-------------------------\n", liters_JG);

        // anadir(JG, jp, 3)
        msj = 3;
        liters_JG -= msj;
        printf("anadir(JG, jp, 3), Estado JG: %d\n-------------------------\n", liters_JG);
        write_from_parent_child(parent_to_child, &msj);

        // listo_para_recibir()
        read_from_child_parent(child_to_parent, &msj);
        liters_JG += msj;

        // transvasar(JG, jp)
        msj = liters_JG;
        liters_JG -= msj;
        printf("transvasar(JG, jp), Estado JG: %d\n-------------------------\n", liters_JG);
        write_from_parent_child(parent_to_child, &msj);

        // listo_para_recibir()
        read_from_child_parent(child_to_parent, &msj);
        liters_JG += msj;

        // llenar(JG, 5)
        liters_JG = 5;
        printf("llenar(JG, 5), Estado JG: %d\n-------------------------\n", liters_JG);

        // anadir(JG, jp, 1)
        msj = 1;
        liters_JG -= msj;
        printf("anadir(JG, jp, 1), Estado JG: %d\n-------------------------\n", liters_JG);
        write_from_parent_child(parent_to_child, &msj);

        close(parent_to_child[WRITE_END]);
        close(parent_to_child[READ_END]);
        close(child_to_parent[WRITE_END]);
        close(child_to_parent[READ_END]);
        wait(NULL);
        
        sleep(1);
        printf("\n-------------------------\nEstados finales\n");
        printf("JG: %d jp: %d\n-------------------------\n", liters_JG, liters_jp);
        printf("Presione enter para terminar\n");
    }

    return 0;
}

void write_from_parent_child(int parent_pipe[], int *value) {
    //close(parent_pipe[READ_END]);
    if (write(parent_pipe[WRITE_END], value, size_of)==-1) {
        printf("error escribiendo el pipe parent_to_chid\n");
        exit(1);
    }
}

void read_from_parent_child(int parent_pipe[], int *value) {
    if (read(parent_pipe[READ_END], value, size_of)==-1){
        printf("error leyendo el pipe parent_to_child\n");
        exit(1);
    }
}

void write_from_child_parent(int child_pipe[], int *value) {
    if (write(child_pipe[WRITE_END], value, size_of) == -1) {
        printf("error escribiendo pipe child_to_parent\n");
        exit(1);
    }
}

void read_from_child_parent(int child_pipe[], int *value) {
    if (read(child_pipe[READ_END], value, size_of) == -1){
        printf("error leyendo pipe child_to_parent\n");
        exit(1);
    }
}