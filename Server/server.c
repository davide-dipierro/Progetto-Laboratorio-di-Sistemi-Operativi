#define MAX_CASSIERI 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <semaphore.h>

#include "carrello.h"
#include "clientecontroller.h"

void cassiereEntra(char*, char*);
void cassiereEsce(char*, char*);

carrello_t carrelli[MAX_CLIENTI];

#define PORT 8080
#define MAX_CONNECTIONS 10
#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 1024

typedef struct {
    int socket;
    struct sockaddr_in address;
} connection_t;

void* process(void * ptr);
void send_response(int socket, char * response);
void read_request(int socket, char * request);

pthread_mutex_t mutex_cassieri = PTHREAD_MUTEX_INITIALIZER;
int n_cassieri = 0;

int main(int argc, char * argv[]) {
    printf("Starting server\n");
    int server_socket = 0;
    int client_socket = 0;
    struct sockaddr_in server_address;
    pthread_t thread;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("Could not create socket"), exit(EXIT_FAILURE);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Collego il socket all'indirizzo e alla porta specificati
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) perror("Could not bind socket"), exit(EXIT_FAILURE);
    // Ascolto le connessioni in entrata
    if (listen(server_socket, MAX_CONNECTIONS) < 0) perror("Could not listen on socket"), exit(EXIT_FAILURE);
    printf("Server started\nWaiting for connections...\n");
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);

        // Se non riesco ad accettare la connessione, stampo un messaggio di errore e termino il programma
        if (client_socket < 0) perror("Could not establish new connection"), exit(EXIT_FAILURE);

        // Creo un thread per gestire la connessione
        if (pthread_create(&thread, NULL, process, (void *) &client_socket) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
    }

    return 0;
}

void* process(void * ptr) {
    int socket = *((int *) ptr);
    char request[MAX_REQUEST_SIZE];
    char response[MAX_RESPONSE_SIZE];

    // Leggo la richiesta del client
    read_request(socket, request);

    // Processo la richiesta
    if(strstr(request, "cliente") != NULL) clienteParser(request, response, carrelli);
    else if(strstr(request, "cassiere") != NULL) cassiereEsce(request, response);
    else printf("Richiesta non valida\n"); 
    

    // Invio la risposta al client
    send_response(socket, response);

    close(socket);
    return NULL;
}

void send_response(int socket, char* response) {
    if(write(socket, response, strlen(response)) == -1) perror("Write"), exit(1);
}

void read_request(int socket, char* request) {
    if(read(socket, request, MAX_REQUEST_SIZE) == -1) perror("Read"), exit(1);
}


void cassiereEntra(char* request, char* response){
    pthread_mutex_lock(&mutex_cassieri);
    n_cassieri++;
    pthread_mutex_unlock(&mutex_cassieri);
    printf("Cassiere entrato, cassieri in negozio: %d\n", n_cassieri);
    strcpy(response, "Sei entrato nel negozio\n\0");
}

void cassiereEsce(char* request, char* response){
    pthread_mutex_lock(&mutex_cassieri);
    n_cassieri--;
    pthread_mutex_unlock(&mutex_cassieri);
    printf("Cassiere uscito, cassieri in negozio: %d\n", n_cassieri);
    strcpy(response, "Sei uscito dal negozio\n\0");
}   