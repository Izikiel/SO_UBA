#include "header.h"

char* end_str = "chau\n";

#ifndef PORT
#define PORT 5555
#endif

int main(int argc, char const *argv[])
{
    if (argc < 2){
        exit(1);
    }

    char const* ip_addr = argv[1];
    struct in_addr my_ip;

    if(inet_aton(ip_addr, &my_ip) < 1){
        exit(2);
    }

    int s;
    struct sockaddr_in remote;
    char str[MENSAJE_MAXIMO];

    /* Crear un socket de tipo UNIX con TCP (SOCK_STREAM). */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creando socket");
        exit(1);
    }

    /* Establecer la dirección a la cual conectarse. */
    remote.sin_family = AF_INET;
    remote.sin_addr = my_ip;
    remote.sin_port = htons(PORT);

    /* Conectarse. */
    if (connect(s, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("conectandose");
        exit(1);
    }

    /* Establecer la dirección a la cual conectarse para escuchar. */
    while(printf("> "), fgets(str, MENSAJE_MAXIMO, stdin), !feof(stdin)) {
        if (send(s, str, strlen(str), 0) == -1) {
            perror("enviando");
            exit(1);
        }
        if (strncmp(str, end_str, strlen(end_str)) == 0){
            break;
        }
        int n = recv(s, str, MENSAJE_MAXIMO, 0);
        if (n == 0)
            break;
        if (n < 0) {
            perror("recibiendo");
            exit(1);
        }
        str[n] = '\0';
        printf("%s\n", str);
    }

    /* Cerrar el socket. */
    close(s);

    return 0;
}

