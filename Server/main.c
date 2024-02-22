#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/socket.h>

#include "carrello.h"
#include "cliente.h"
#include "codaCassa.h"
#include "config.h"
#include "cassiere.h"
#include "codaIngresso.h"

carrello_t carrelli[VARIABILE_C];
coda_casse_t coda_casse;
coda_ingresso_t coda_ingresso;
pthread_mutex_t mutex_coda_ingresso;
pthread_mutex_t mutex_coda_casse;
pthread_mutex_t mutex_n_clienti;
pthread_mutex_t mutex_chiocciola;
pthread_mutex_t mutex_carrelli;

#define PORT 5050
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
void inviaCatalogo(char* response);
void* riordinaCarrelli();
void* ui();
void* buttafuoriAllIngresso();
void stampa_stickman(int num_stickman);

pthread_mutex_t mutex_cassieri = PTHREAD_MUTEX_INITIALIZER;

int main() {
    printf("[SERVER] Inizializzo carrelli\n");
    inizializza_carrelli(carrelli);
    printf("[SERVER] Entrano i cassieri\n");
    for(int i=0; i<N_CASSE; i++) cassiereEntra(5, 2, carrelli, &coda_casse);
    int server_socket = 0;
    int client_socket = 0;
    struct sockaddr_in server_address;
    pthread_t thread_client;
    pthread_t thread_pulisci_carrelli;
    if(pthread_create(&thread_pulisci_carrelli, NULL, riordinaCarrelli, NULL) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
    printf("[SERVER] Thread pulizia carrelli creato\n");
    pthread_t thread_buttafuori;
    if(pthread_create(&thread_buttafuori, NULL, buttafuoriAllIngresso, NULL) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
    printf("[SERVER] Thread buttafuori creato\n");
    //pthread_t thread_ui;
    //if(pthread_create(&thread_ui, NULL, ui, NULL) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
    //printf("[SERVER] Thread UI creato\n");

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("Could not create socket"), exit(EXIT_FAILURE);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Collego il socket all'indirizzo e alla porta specificati
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) perror("Could not bind socket"), exit(EXIT_FAILURE);
    // Ascolto le connessioni in entrata
    if (listen(server_socket, MAX_CONNECTIONS) < 0) perror("Could not listen on socket"), exit(EXIT_FAILURE);
    printf("[SERVER] Server inizializzato\n[SERVER] Waiting for connections...\n");
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);

        // Se non riesco ad accettare la connessione, stampo un messaggio di errore e termino il programma
        if (client_socket < 0) perror("Could not establish new connection"), exit(EXIT_FAILURE);

        // Creo un thread per gestire la connessione
        if (pthread_create(&thread_client, NULL, process, (void *)&client_socket) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
    }

    return 0;
}

void* process(void * ptr) {
    int socket = *((int *) ptr);
    char request[MAX_REQUEST_SIZE];
    char response[MAX_RESPONSE_SIZE];

    memset(request, 0, MAX_REQUEST_SIZE);
    memset(response, 0, MAX_RESPONSE_SIZE);

    // Leggo la richiesta del client
    read_request(socket, request);

    // Processo la richiesta
    if(strstr(request, "cliente") != NULL) {
        printf("[CLIENTE] Request: %s\n", request);
        clienteParser(request, response, carrelli, &coda_casse, &coda_ingresso);
        printf("[SERVER] Response sent: %s", response);
    } else if(strstr(request, "cassiere") != NULL) {
        printf("[CASSIERE] Request: %s\n", request);
        strcpy(response, "cassiere ok");
        printf("[SERVER] Response sent: %s", response);
    }
    else if(strstr(request, "catalogo") != NULL) {
        printf("[ANONIMO] Request: %s\n", request);
        inviaCatalogo(response);
        printf("[SERVER] Response sent: catalogo\n");
    }
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

void inviaCatalogo(char* response) {
    //printf("Richiesta catalogo\n");
    // Apro il file catalogo.txt
    FILE* catalogo = fopen("catalogo.json", "r");
    if(catalogo == NULL) perror("Errore apertura catalogo"), exit(1);

    // Leggo il file catalogo.txt e lo salvo nella stringa response
    char buffer[MAX_RESPONSE_SIZE];
    while(fgets(buffer, MAX_RESPONSE_SIZE, catalogo) != NULL) {
        strcat(response, buffer);
    }

    // Chiudo il file catalogo.txt
    fclose(catalogo);
    
}

void* riordinaCarrelli() {
    while(1) {
        sleep(TIMER_PULIZIA_CARRELLI); 
        printf("[NEGRETTO] Riordino carrelli\n");
        for(int i = 0; i < VARIABILE_C; i++) {
            if(carrelli[i].status != LIBERO && carrelli[i].ultima_operazione + TIMER_PULIZIA_CARRELLI < time(NULL)) {
                pthread_mutex_lock(&carrelli[i].mutex);
                printf("[NEGRETTO] Carrello %d riordinato\n", i);
                svuota_carrello(&carrelli[i]);
                if(carrelli[i].status == IN_CODA) rimuovi_cliente_coda_id(i, &coda_casse);
                carrelli[i].status = LIBERO;
                decrementa_n_clienti();
                pthread_mutex_unlock(&carrelli[i].mutex);
            }
        }
    }
}

void* buttafuoriAllIngresso(){
    while(1) {
        sleep(TIMER_BUTTAFUORI); 
        printf("[BUTTAFUORI] Controllo se ci sono ancora tutti\n");
        pthread_mutex_lock(&mutex_coda_ingresso);
        nodoIngresso_t* nodoIngresso_corrente = coda_ingresso.head;
        while(nodoIngresso_corrente != NULL) {
            if(nodoIngresso_corrente->ultima_operazione + TIMER_BUTTAFUORI < time(NULL)) {
                printf("[BUTTAFUORI] Butto fuori %d\n", nodoIngresso_corrente->id_cliente);
                rimuovi_cliente_coda_ingresso_id(nodoIngresso_corrente->id_cliente, &coda_ingresso);
            }
            nodoIngresso_corrente = nodoIngresso_corrente->next;
        }
    }
}

void* ui(){
    while(1){
        printf("\033[H\033[J");
        printf("\nIN NEGOZIO:\n");
        int persone = 0;
        for(int i = 0; i < VARIABILE_C; i++) {
            if (carrelli[i].status == IN_NEGOZIO) {
                persone++;
            }
        }
        stampa_stickman(persone);
        persone = 0;
        printf("IN CODA:\n");
        fflush(stdout);
        for(int i = 0; i < VARIABILE_C; i++) {
            if (carrelli[i].status == IN_CODA) {
                persone++;
            }
        }
        stampa_stickman(persone);
        persone = 0;
        printf("IN CASSA:\n");
        for(int i = 0; i < VARIABILE_C; i++) {
            if (carrelli[i].status == IN_CASSA || carrelli[i].status == PAGAMENTO) {
                persone++;
            }
        }
        stampa_stickman(persone);
        printf("\n");
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}

void stampa_stickman(int num_stickman) {
    int i, j;
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < num_stickman; j++) {
            if (i == 0)
                printf(" O ");
            else if (i == 1)
                printf("/|\\");
            else if (i == 2)
                printf(" | ");
            else if (i == 3)
                printf("/ \\");
            else
                printf("   ");
            
            printf("   "); // spazio tra gli stickman
        }
        printf("\n");
    }
}