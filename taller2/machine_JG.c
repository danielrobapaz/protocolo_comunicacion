#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#define SERVER_PORT 4321
#define BUFFER_LEN 1024


int main(int argc, char *argv[]){

    printf("Inicio de maquina 1 - JG");

    int liters_JG, msg;

    // Initial state
    liters_JG = 0;
    msg = 0;

    // Start Comunication
    // llenar(JG,5)
    liters_JG = 5;
    printf("llenar(JG, 5), Estado JG: %d\n-------------------------\n", liters_JG);

    // anadir(JG,jp,3)
    msg = 3;
    liters_JG -= msg;
    printf("anadir(JG, jp, 3), Estado JG: %d\n-------------------------\n", liters_JG);
    send('localhost', &msg);


    return 0;

}

void recieve(int *value) {

    int sockfd; /* descriptor para el socket */
    struct sockaddr_in my_addr; /* direccion IP y numero de puerto local */
    struct sockaddr_in their_addr; /* direccion IP y numero de puerto del cliente */
    
    /* addr_len contendra el taman~o de la estructura sockadd_in y numbytes el
    * numero de bytes recibidos */
    int addr_len, numbytes;
    char buf[BUFFER_LEN]; /* Buffer de recepción */
    /* se crea el socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Se establece la estructura my_addr para luego llamar a bind() */
    my_addr.sin_family = AF_INET; /* usa host byte order */
    my_addr.sin_port = htons(SERVER_PORT); /* usa network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* escuchamos en todas las IPs */
    bzero(&(my_addr.sin_zero), 8); /* rellena con ceros el resto de la estructura */
    /* Se le da un nombre al socket (se lo asocia al puerto e IPs) */
    printf("Asignado direccion al socket ....\n");
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(2);
    }

    /* Se reciben los datos (directamente, UDP no necesita conexión) */
    addr_len = sizeof(struct sockaddr);
    printf("Esperando datos ....\n");
    if ((numbytes=recvfrom(sockfd, buf, BUFFER_LEN, 0, (struct sockaddr *)&their_addr,
    (socklen_t *)&addr_len)) == -1) {
        perror("recvfrom");
        exit(3);
    }

    /* Se visualiza lo recibido */
    printf("paquete proveniente de : %s\n",inet_ntoa(their_addr.sin_addr));
    printf("longitud del paquete en bytes: %d\n",numbytes);
    buf[numbytes] = '\0';
    printf("el paquete contiene: %s\n", buf);

    /* cerramos descriptor del socket */
    close(sockfd);
}

void send(char *host, char *msg){
    int sockfd; /* descriptor a usar con el socket */
    struct sockaddr_in their_addr; /* almacenara la direccion IP y numero de puerto del servidor */
    struct hostent *he; /* para obtener nombre del host */
    int numbytes; /* conteo de bytes a escribir */
 
    /* convertimos el hostname a su direccion IP */
    if ((he=gethostbyname(host)) == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    /* Creamos el socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(2);
    }

    /* a donde mandar */
    their_addr.sin_family = AF_INET; /* usa host byte order */
    their_addr.sin_port = htons(SERVER_PORT); /* usa network byte order */
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8); /* pone en cero el resto */
    
    /* enviamos el mensaje */
    if ((numbytes=sendto(sockfd,msg,strlen(msg),0,(struct sockaddr *)&their_addr,
    sizeof(struct sockaddr))) == -1) {
        perror("sendto");
        exit(2);
    }
    printf("enviados %d bytes hacia %s\n",numbytes,inet_ntoa(their_addr.sin_addr));

    /* cierro socket */
    close(sockfd);
}
