#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fcntl.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h> 
#include <string.h>
#include "hash_linear_probing.h"
#include "hash.h"
#include "keyvalue.h"
#define MAX_EVENTS 10

void quit(char *s) {
    perror(s);
    abort();
}

int fd_readline(int fd, char *buf) {
    int rc;
    int i = 0;

    while ((rc = read(fd, buf + i, 1)) > 0) {
        if (buf[i] == '\n')
            break;
        i++;
    }

    if (rc < 0)
        quit("read");

    buf[i] = 0;
    return i;
}


int leer_comandos(int socketFd, char** cmds) {
	char buf[200];
	int c = 1;
	fd_readline(socketFd, buf);
	cmds[0] = strtok(buf, " ");
	if(cmds[0] == NULL) return 0;
	for(int i = 1; i < 3; i++) {
		cmds[i] = strtok(NULL, " ");
		if (cmds[i] == NULL) i = 3;
		else c++;
	} 
	return c;
}



void manejador_pedidos(int socketFd, TablaHash tabla) {
	char** comandos = malloc(sizeof(char*)*3);

	int cantComandos = leer_comandos(socketFd, comandos);
	
	if (cantComandos < 2) printf("Cantidad de cmds invalida\n");

	else if (strcmp(comandos[0], "PUT") == 0) {
		KeyValue kv;
		kv.key = strdup(comandos[1]);
		kv.value = strdup(comandos[2]);
		tablahash_insertar(tabla, &kv);
	}

	else if (strcmp(comandos[0], "DEL") == 0) {
		KeyValue kv;
		kv.key = strdup(comandos[1]);
		kv.value = NULL;
		tablahash_eliminar(tabla, &kv);
	 }

	else if (strcmp(comandos[0], "GET") == 0) {
		KeyValue kv;
		kv.key = strdup(comandos[1]);
		kv.value = strdup(comandos[2]);
		KeyValue* ret = tablahash_buscar(tabla, &kv);
		if (ret == NULL) printf("No esta\n");
		else printf("A");
	}
	
	else printf("Comando invalido");

	free(comandos);	
}

void atender_pedidos(int socketFd, TablaHash tabla) {
	struct epoll_event event,  events[MAX_EVENTS];
	int epollFd = epoll_create1(0);
	int eventCounter;

	event.events = EPOLLIN; /*Solo miramos inputs*/
	event.data.fd = socketFd;

	epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event);

	while(1) {
		printf("Esperando orden...\n");
		eventCounter = epoll_wait(epollFd, events, MAX_EVENTS, 30000);
		printf("%d eventos listos\n", eventCounter);

		for (int i = 0; i < eventCounter; ++i) {
            if (events[i].data.fd == socketFd) {
                // Aceptar la nueva conexión
                int clientSock = accept(socketFd, NULL, NULL);
                if (clientSock == -1) {
                    perror("accept");
                    continue;
                }
                // Agregar el nuevo socket al conjunto de descriptores epoll
                event.events = EPOLLIN;
                event.data.fd = clientSock;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSock, &event);
                // Imprimir un mensaje de conexión aceptada
                printf("Nueva conexión aceptada\n");
            } else {
                // Manejar un evento en un socket existente
                manejador_pedidos(events[i].data.fd, tabla);
            }
        }
	}

	close(epollFd);
}


int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(3942);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main() {

	/*
	Primero, creamos el socket que va a escuchar pedidos en el
	puerto 
	*/
	int fd = mk_lsock();

	/*
	Manejo de la memoria compartida
	
	int fdSM = shm_open("/BACK",
							O_RDWR | O_CREAT,
								0644);
	ftruncate(fd,10000);
	void* memptr = mmap(NULL, 10000, PROT_READ | PROT_WRITE,
    MAP_SHARED, fdSM, 0);
	*/
    TablaHash tabla = tablahash_crear(10,
										  keyvalue_copiar,
										  keyvalue_destruir,
										  keyvalue_comparar,
										  (FuncionHash)keyvalue_hash);

	atender_pedidos(fd, tabla);

	/*
	Cerramos memoria compartida
	
	munmap(memptr, 550);
    close(fdSM);
	*/
	return 0;
}