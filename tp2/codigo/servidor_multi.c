#include <signal.h>
#include <errno.h>

#include "biblioteca.h"

/* Estructura que almacena los datos de una reserva. */
typedef struct {
	int posiciones[ANCHO_AULA][ALTO_AULA];
	int cantidad_de_personas;
	
	int rescatistas_disponibles;
} t_aula;

/* Estructura para la creacion de los workers*/
typedef struct worker_parameters{
	int socket_fd;
	t_aula *el_aula;
} t_worker_parameters;

//firma de la funcion worker
void *new_worker_handler(void *new_client_parameters);

void t_aula_iniciar_vacia(t_aula *un_aula)
{
	int i, j;
	for(i = 0; i < ANCHO_AULA; i++)
	{
		for (j = 0; j < ALTO_AULA; j++)
		{
			un_aula->posiciones[i][j] = 0;
		}
	}
	
	un_aula->cantidad_de_personas = 0;
	
	un_aula->rescatistas_disponibles = RESCATISTAS;
}

void t_aula_ingresar(t_aula *un_aula, t_persona *alumno)
{
	un_aula->cantidad_de_personas++;
	un_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]++;
}

void t_aula_liberar(t_aula *un_aula, t_persona *alumno)
{
	un_aula->cantidad_de_personas--;
	un_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]--;
}

static void terminar_servidor_de_alumno(int socket_fd, t_aula *aula, t_persona *alumno) {
	printf("[Thread cliente %d] >> Se interrumpió la comunicación con una consola.\n", socket_fd);
	close(socket_fd);
	t_aula_liberar(aula, alumno);
}


t_comando intentar_moverse(t_aula *el_aula, t_persona *alumno, t_direccion dir)
{
	int fila = alumno->posicion_fila;
	int columna = alumno->posicion_columna;
	alumno->salio = direccion_moverse_hacia(dir, &fila, &columna);

	///char buf[STRING_MAXIMO];
	///t_direccion_convertir_a_string(dir, buf);
	///printf("%s intenta moverse hacia %s (%d, %d)... ", alumno->nombre, buf, fila, columna);
	
	
	bool entre_limites = (fila >= 0) && (columna >= 0) &&
	     (fila < ALTO_AULA) && (columna < ANCHO_AULA);
	     
	bool pudo_moverse = alumno->salio ||
	    (entre_limites && el_aula->posiciones[fila][columna] < MAXIMO_POR_POSICION);
	
	
	if (pudo_moverse)
	{
		if (!alumno->salio)
			el_aula->posiciones[fila][columna]++;
		el_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]--;
		alumno->posicion_fila = fila;
		alumno->posicion_columna = columna;
	}
	
	
	//~ if (pudo_moverse)
		//~ printf("OK!\n");
	//~ else
		//~ printf("Ocupado!\n");


	return pudo_moverse;
}

void colocar_mascara(t_aula *el_aula, t_persona *alumno, int socket_fd)
{
	printf("[Thread cliente %d] Esperando rescatista. Ya casi %s!\n", socket_fd, alumno->nombre);
		
	alumno->tiene_mascara = true;
}


void *atendedor_de_alumno(int socket_fd, t_aula *el_aula)
{
	t_persona alumno;
	t_persona_inicializar(&alumno);
	
	if (recibir_nombre_y_posicion(socket_fd, &alumno) != 0) {
		/* O la consola cortó la comunicación, o hubo un error. Cerramos todo. */
		terminar_servidor_de_alumno(socket_fd, NULL, NULL);
	}
	
	printf("[Thread cliente %d] Atendiendo a %s en la posicion (%d, %d)\n", 
			socket_fd, alumno.nombre, alumno.posicion_fila, alumno.posicion_columna);
		
	t_aula_ingresar(el_aula, &alumno);
	
	/// Loop de espera de pedido de movimiento.
	for(;;) {
		t_direccion direccion;
		
		/// Esperamos un pedido de movimiento.
		if (recibir_direccion(socket_fd, &direccion) != 0) {
			/* O la consola cortó la comunicación, o hubo un error. Cerramos todo. */
			terminar_servidor_de_alumno(socket_fd, el_aula, &alumno);
		}
		
		/// Tratamos de movernos en nuestro modelo
		bool pudo_moverse = intentar_moverse(el_aula, &alumno, direccion);

		printf("[Thread cliente %d] %s se movio a: (%d, %d)\n", socket_fd, alumno.nombre, alumno.posicion_fila, alumno.posicion_columna);

		/// Avisamos que ocurrio
		enviar_respuesta(socket_fd, pudo_moverse ? OK : OCUPADO);		
		//printf("aca3\n");
		
		if (alumno.salio)
			break;
	}
	
	colocar_mascara(el_aula, &alumno, socket_fd);

	t_aula_liberar(el_aula, &alumno);
	enviar_respuesta(socket_fd, LIBRE);
	
	printf("[Thread cliente %d] Listo, %s es libre!\n", socket_fd, alumno.nombre);
	
	return NULL;

}


int main(void)
{
	//signal(SIGUSR1, signal_terminar);
	int socketfd_cliente, socket_servidor, socket_size;
	struct sockaddr_in local, remoto;
	
	/* Crear un socket de tipo INET con TCP (SOCK_STREAM). */
	if ((socket_servidor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("creando socket");
	}

	/* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede conectarse aquí. */
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);
	
	if (bind(socket_servidor, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("haciendo bind");
	}

	/* Escuchar en el socket y permitir 5 conexiones en espera. */
	if (listen(socket_servidor, 5) == -1) {
		perror("escuchando");
	}
	
	t_aula el_aula;
	t_aula_iniciar_vacia(&el_aula);
	
	/// Aceptar conexiones entrantes.
	socket_size = sizeof(remoto);
	for(;;){		
		if (-1 == (socketfd_cliente = 
					accept(socket_servidor, (struct sockaddr*) &remoto, (socklen_t*) &socket_size)))
		{			
			fprintf(stderr, "!! Error al aceptar conexion\n");
		}else{
			//no necesito monitorear ni nada al worker, asi que no necesito una lista de los hilos lanzados ni algun "estado" posible de ellos
			//levantar hilo worker para atender el pedido. No es necesario que sea joineable, asi que lo hago dettachable
			pthread_attr_t pthread_attributes; 
			pthread_attr_init (&pthread_attributes);
			pthread_attr_setdetachstate (&pthread_attributes, PTHREAD_CREATE_DETACHED);
			//me tengo que armar un struct para enviarle como "parametros" al nuevo worker thread el socketfd_cliente
			//como se le pasa un ptr, necesito un t_worker_parameters para cada uno, no problem. Uso memoria dinamica
			//QUE ES LIBERADA DESDE EL THREAD AL FINALIZAR en la funcion new_worker_parameters
			t_worker_parameters* new_worker_parameters = (t_worker_parameters*) malloc(sizeof(t_worker_parameters));
			//inicializo parametros
			new_worker_parameters->socket_fd = socketfd_cliente;
			new_worker_parameters->el_aula = &el_aula;
		
			//creo el nuevo hilo y lo lanzo a ejecutar. asincronicamente se siguen escuchando nuevos llamados
			pthread_t nuevo_worker;
			if(pthread_create(&nuevo_worker, &pthread_attributes, new_worker_handler, new_worker_parameters)){
				fprintf(stderr, "Error creating thread for client%d\n", socketfd_cliente);
				exit(1);
			}else{
				printf("[Main thread] Lanzado nuevo hilo para el cliente %d\n", socketfd_cliente);
			}
			
		}
	}

	return 0;
}

/* this function is run by the second thread */
void *new_worker_handler(void *new_client_parameters)
{
	t_worker_parameters* parameters = (t_worker_parameters*) new_client_parameters;
	printf("[Thread cliente %d] Hola, nuevo worker con FD %d\n", parameters->socket_fd, parameters->socket_fd);
	atendedor_de_alumno(parameters->socket_fd, parameters->el_aula);		

	//POR CONVENCION MIA, INSTANCIO EL t_worker_parameters AL CREAR EL THREAD Y LO LIBERO ACA.	
	printf("[Thread cliente %d] Chau, termino el worker con FD %d\n", parameters->socket_fd, parameters->socket_fd);
	free(new_client_parameters);
	return NULL;//no hace falta devolver algo concreto, NULL basta, no quiero monitorear los workers ni nada de eso.
}