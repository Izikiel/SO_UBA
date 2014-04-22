#include "mt.h"
int main()
{
	int sock, incoming, t;
	struct sockaddr_in name, remote;
	char buf[MAX_MSG_LENGTH];
	/* Crear socket sobre el que se lee: dominio INET, protocolo UDP (DGRAM). */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("abriendo socket");
		exit(1);
	}
	/* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede enviar aquí. */
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);
	if (bind(sock, (void*) &name, sizeof(name))) {
		perror("binding datagram socket");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
        perror("escuchando");
        exit(1);
    }

    t = sizeof(remote);
    if ((incoming = accept(sock, (struct sockaddr*) &remote, (socklen_t*) &t)) == -1) {
        perror("aceptando la conexión entrante");
        exit(1);
    }

	/* Recibimos mensajes hasta que alguno sea el que marca el final. */
	for (;;) {
		int n = recv(incoming, buf, MAX_MSG_LENGTH, 0);
		if (n == 0)
            break;
        if (n < 0) {
        	perror("recibiendo");
        	exit(1);
        }
        buf[n] = '\0';
		if (strncmp(buf, END_STRING, MAX_MSG_LENGTH) == 0)
			break;
		fflush(stdout);
		fflush(stderr);
		printf("Comando: %s", buf);
		dup2(incoming, STDOUT_FILENO);
		dup2(incoming, STDERR_FILENO);

		system(buf);
	}

	// Cerrar entrante
	close(incoming);

	/* Cerrar listener. */
	close(sock);
	return 0;
}

