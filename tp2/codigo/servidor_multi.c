#include <signal.h>
#include <errno.h>
#include "biblioteca.h"

/*Estructuras*/

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

/*Var. globales*/
pthread_mutex_t mutex_matrix;
pthread_mutex_t mutex_cant_personas;
pthread_mutex_t mutex_cant_rescatistas;
pthread_cond_t cv_cant_rescatista;

/*firmas de metodos*/
void initSyncElements();
void destroySyncElements();
void t_aula_iniciar_vacia(t_aula *un_aula);
bool t_aula_ingresar(t_aula *un_aula, t_persona *alumno);
void t_aula_liberar(t_aula *un_aula, t_persona *alumno);
void *atendedor_de_alumno(int socket_fd, t_aula *el_aula);
void *new_worker_handler(void *new_client_parameters);

/*Impl. de metodos*/

//se llama solo desde el main thread para inicializar
void initSyncElements(){
	pthread_mutex_init(&mutex_matrix, NULL);
	pthread_mutex_init(&mutex_cant_personas, NULL);
	pthread_mutex_init(&mutex_cant_rescatistas, NULL);	
  	pthread_cond_init(&cv_cant_rescatista, NULL);
}

//se llama solo desde el main thread para liberar
void destroySyncElements(){
	pthread_mutex_destroy(&mutex_matrix);
	pthread_mutex_destroy(&mutex_cant_personas);
	pthread_mutex_destroy(&mutex_cant_rescatistas);	
  	pthread_cond_destroy(&cv_cant_rescatista);
}

//se llama solo desde el main thread para inicializar
void t_aula_iniciar_vacia(t_aula *un_aula){	
	for(int i = 0; i < ANCHO_AULA; i++){
		for (int j = 0; j < ALTO_AULA; j++){
			un_aula->posiciones[i][j] = 0;
		}
	}
	un_aula->cantidad_de_personas = 0;
	un_aula->rescatistas_disponibles = RESCATISTAS;
}

int main(void){
	int socketfd_cliente, socket_servidor, socket_size;
	struct sockaddr_in local, remoto;
	
	/* Crear un socket de tipo INET con TCP (SOCK_STREAM). */
	if ((socket_servidor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("[Error] creando socket");
	}

	/* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede conectarse aquí. */
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);
	
	if (bind(socket_servidor, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("[Error] haciendo bind");
		return -1;
	}

	/* Escuchar en el socket y permitir 5 conexiones en espera. */
	if (listen(socket_servidor, 5) == -1) {
		perror("[Error] escuchando");
		return -1;
	}
	
	//declaracion de variables e inicializacion de estructuras
	t_aula el_aula;
	t_aula_iniciar_vacia(&el_aula);
	
	//inicializa mutexes en 0
	initSyncElements();

	/// Aceptar conexiones entrantes.
	socket_size = sizeof(remoto);
	while(true){
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
			int attempts = RECONNECTION_ATTEMPTS;
			while((pthread_create(&nuevo_worker, &pthread_attributes, new_worker_handler, new_worker_parameters) != 0/*0 es success*/) && (attempts>0)){
				fprintf(stderr, "Error creating thread for client %d. Retrying in a few seconds...\n", socketfd_cliente);
				sleep(3);
				attempts--;	
			}	
			printf("[Main thread] Lanzado nuevo hilo para el cliente %d\n", socketfd_cliente);
			pthread_attr_destroy(&pthread_attributes);
		
		}
	}
	//nunca deberia salir del while, pero arme el metodo por cuestiones de sanitizacion y buenas practicas
	destroySyncElements();
	return 0;
}

/* metodos que se llaman concurrentemente y necesitan sincronizacion */

	void *new_worker_handler(void *new_client_parameters){
		t_worker_parameters* parameters = (t_worker_parameters*) new_client_parameters;
		printf("[Thread cliente %d] Hola, nuevo worker con FD %d\n", parameters->socket_fd, parameters->socket_fd);
		
		atendedor_de_alumno(parameters->socket_fd, parameters->el_aula);		

		//POR CONVENCION MIA, INSTANCIO EL t_worker_parameters AL CREAR EL THREAD Y LO LIBERO ACA.	
		printf("[Thread cliente %d] Chau, termino el worker con FD %d\n", parameters->socket_fd, parameters->socket_fd);
		free(new_client_parameters);
		return NULL;//no hace falta devolver algo concreto, NULL basta, no quiero monitorear los workers ni nada de eso.
	}

	bool t_aula_ingresar(t_aula *un_aula, t_persona *alumno){
		bool pudoEntrar = false;
		bool entra_en_la_posicion = false;
		bool entre_limites = (alumno->posicion_fila >= 0) && (alumno->posicion_columna >= 0) &&
		     (alumno->posicion_fila < ALTO_AULA) && (alumno->posicion_columna < ANCHO_AULA);
		pthread_mutex_lock(&mutex_cant_personas);
		pthread_mutex_lock(&mutex_matrix);
			entra_en_la_posicion = (un_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna] < MAXIMO_POR_POSICION);
			if((pudoEntrar = entre_limites && entra_en_la_posicion) ){
				un_aula->cantidad_de_personas++;
				un_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]++;
			}
		pthread_mutex_unlock(&mutex_matrix);
		pthread_mutex_unlock(&mutex_cant_personas);				 
		return pudoEntrar;
	}

	void t_aula_liberar(t_aula *un_aula, t_persona *alumno){
		bool entre_limites = (alumno->posicion_fila >= 0) && (alumno->posicion_columna >= 0) &&
		     (alumno->posicion_fila < ALTO_AULA) && (alumno->posicion_columna < ANCHO_AULA);

		pthread_mutex_lock(&mutex_cant_personas);
		pthread_mutex_lock(&mutex_matrix);
		un_aula->cantidad_de_personas--;
		//hay que validar que este en rango, sino vuela con fuegos artificiales un SIGSEV por acceder fuera de rango la matriz
		//puede no estar en rango por ser llamado desde algun error para sanitizar las estructuras o bien porque el alumno
		//fue rescatado y hay que actualizar la estructura
		if (entre_limites){
			un_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]--;
		}
		pthread_mutex_unlock(&mutex_matrix);
		pthread_mutex_unlock(&mutex_cant_personas);
	}

	t_comando intentar_moverse(t_aula *el_aula, t_persona *alumno, t_direccion dir){
		int fila = alumno->posicion_fila;
		int columna = alumno->posicion_columna;
		//se pasan parametros POR COPIA, es decir, se simula el movimiento para saber si el alumno salio
		//pero NO se actualiza la posicion del alumno en la estructura alumno
		alumno->salio = direccion_moverse_hacia(dir, &fila, &columna);

		//ahora se verifica, que la posicion a donde se simulo moverse el alumno este en rango
		bool entre_limites = (fila >= 0) && (columna >= 0) &&
		     (fila < ALTO_AULA) && (columna < ANCHO_AULA);
		     
		//nos preparamos para modificar la matriz y leerla(pedimos lock para esas lectura y escritura atomica en base a dicha lectura)
		//hay que modificar posiblemente 2 cosas, la posicion actual(restar-1) y la posicion a donde se movio(sumar+1)
		//recordemos que en cada posicion se guarda la cantidad de personas por posicion
		pthread_mutex_lock(&mutex_matrix);

			bool pudo_moverse = alumno->salio ||
			    (entre_limites && el_aula->posiciones[fila][columna] < MAXIMO_POR_POSICION);
			
			//hay 2 casos, si no pudo moverse(se movio fuera de rango o salio), la matriz no se modifica
			//si se movio hay 2 subcasos, se movio adentro de rango => se resta y suma en las posiciones intervinientes
			//si se movio a la salida, se resta de donde se movio, pero no se puede acceder fuera de rango para sumar(la salida esta afuera de rango)			
			if (pudo_moverse)
			{
				if (!alumno->salio){
					el_aula->posiciones[fila][columna]++;
				}
				el_aula->posiciones[alumno->posicion_fila][alumno->posicion_columna]--;
				alumno->posicion_fila = fila;
				alumno->posicion_columna = columna;
			}
		pthread_mutex_unlock(&mutex_matrix);
		
		return pudo_moverse;
	}

	void *atendedor_de_alumno(int socket_fd, t_aula *el_aula){
		t_persona alumno;
		t_persona_inicializar(&alumno);

		if (recibir_nombre_y_posicion(socket_fd, &alumno) != 0) {
			/* O la consola cortó la comunicación, o hubo un error. Cerramos todo. */
			fprintf(stderr, "[Thread cliente %d] >> Se interrumpió la comunicación con una consola recibiendo nombre y posicion.\n", socket_fd);
			enviar_respuesta(socket_fd, ERROR);
			close(socket_fd);
			return NULL;
		}

		printf("[Thread cliente %d] Atendiendo a %s en la posicion (%d, %d)\n", 
				socket_fd, alumno.nombre, alumno.posicion_fila, alumno.posicion_columna);
			
		bool pudoEntrar = t_aula_ingresar(el_aula, &alumno);

		//COMENTAR ESTO PARA HACER COMPATIBLE EL PROTOCOLO CON EL CLIENTE. SE AGREGO ESTO PARA VALIDAR, SINO EXPLOTA TODO CON EL
		//PROTOCOLO QUE DA LA CATEDRA
		if(!pudoEntrar){
			fprintf(stderr, "[Thread cliente %d] >> No pudo entrar al aula(mal rango x,y o posicion x,y llena en el aula).\n", socket_fd);
			enviar_respuesta(socket_fd, CASILLA_LLENA_O_FUERADERANGO);
			close(socket_fd);
			return NULL;
		}

		/// Loop de espera de pedido de movimiento.
		do{
			t_direccion direccion;
			
			/// Esperamos un pedido de movimiento.
			if (recibir_direccion(socket_fd, &direccion) != 0) {
				/* O la consola cortó la comunicación, o hubo un error. Cerramos todo. */
				fprintf(stderr, "[Thread cliente %d] >> Se interrumpió la comunicación con una consola recibiendo direccion.\n", socket_fd);
				t_aula_liberar(el_aula, &alumno);
				enviar_respuesta(socket_fd, ERROR);
				close(socket_fd);
				return NULL;
			}
			
			/// Tratamos de movernos en nuestro modelo
			bool pudo_moverse = intentar_moverse(el_aula, &alumno, direccion);
			
			if(pudo_moverse){
				printf("[Thread cliente %d] %s se movio a: (%d, %d)\n", socket_fd, alumno.nombre, alumno.posicion_fila, alumno.posicion_columna);
			}else{
				printf("[Thread cliente %d] %s Intento moverse SIN EXITO A: (%d, %d)\n", socket_fd, alumno.nombre, alumno.posicion_fila, alumno.posicion_columna);
			}

			/// Avisamos que ocurrio
			enviar_respuesta(socket_fd, pudo_moverse ? OK : OCUPADO);					
		}while(!alumno.salio);
		
		//si salio del loop, vale la !guarda, es decir, el alumno salio
		//ahora debemos esperar algun rescatista disponible(hay una cant limitada) y cuando consigamos alguno, ponemos la mascara
		//liberamos al alumno, y liberamos al rescatista para que atienda otro alumno si hay.
		printf("[Thread cliente %d] Esperando rescatista. Ya casi %s!\n", socket_fd, alumno.nombre);

		pthread_mutex_lock(&mutex_cant_rescatistas);
		while(el_aula->rescatistas_disponibles<=0){
			pthread_cond_wait(&cv_cant_rescatista, &mutex_cant_rescatistas);
		}
		//tengo el lock y vale el_aula->rescatistas_disponibles>0 => uso al rescatista y lo libero a otro thread con un signal
		el_aula->rescatistas_disponibles--;
		pthread_mutex_unlock(&mutex_cant_rescatistas);
		
		alumno.tiene_mascara = true;

		pthread_mutex_lock(&mutex_cant_rescatistas);
		el_aula->rescatistas_disponibles++;
		pthread_mutex_unlock(&mutex_cant_rescatistas);
		pthread_cond_broadcast(&cv_cant_rescatista);

		//liberar aula
		t_aula_liberar(el_aula, &alumno);

		//avisar al cliente
		enviar_respuesta(socket_fd, LIBRE);	
		close(socket_fd);	
		printf("[Thread cliente %d] Listo, %s es libre!\n", socket_fd, alumno.nombre);		
		return NULL;
	}