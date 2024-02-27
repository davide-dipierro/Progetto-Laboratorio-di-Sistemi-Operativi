/*
 * Client che si connette al server e invia una richiesta
 * e stampa la risposta
 */


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
#include <arpa/inet.h>

#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 1024

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5050

void send_request(int socket, char * request);
void read_response(int socket, char * response);

int getScelta();
void printmenu();

void ingresso(char* request, char* response, int* id_cliente, int* id_carrello);
void entra(char* request, char* response, int* id_cliente, int* id_carrello);
void esci(char* request, char* response, int* id_cliente, int* id_carrello);
void aggiungi(char* request, char* response, int* id_cliente, int* id_carrello);
void rimuovi(char* request, char* response, int* id_cliente, int* id_carrello);
void stampa(char* request, char* response, int* id_cliente, int* id_carrello);
void mettiInCoda(char* request, char* response, int* id_cliente, int* id_carrello);
void paga(char* request, char* response, int* id_cliente, int* id_carrello);
void stampaCatalogo(char* request, char* response, int* id_cliente, int* id_carrello);
void aggiungi_con_id(char* request, char* response, int* id_cliente, int* id_carrello, int id_prodotto);
void autopilota();

#define NUM_THREADS 5

int main(int argc, char** argv) {
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Creazione thread %d\n", i);
        pthread_create(&threads[i], NULL, (void *) autopilota, NULL);
        //sleep(1);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}


int create_socket(){
    int sockfd;
    struct sockaddr_in server_address;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        exit(1);
    }
    
    return sockfd;
}



void autopilota() {
    char request[MAX_REQUEST_SIZE];
    char response[MAX_RESPONSE_SIZE];
    int posizione=-1;
    int app;

    int id_carrello=-1;
    int id_cliente=-1;

    printf("Thread autopilota\n");
    

    do {
        ingresso(request,response, &id_cliente, &id_carrello);
        if (strstr(response, "ID_cliente") != NULL) sscanf(response, "ID_cliente:%d:%d\n", &app, &posizione);
        sleep(7);
    } while(posizione!=0);

    do {
        entra(request,response, &id_cliente, &id_carrello);
        sleep(7);
    } while(id_carrello==-1);
      
    aggiungi_con_id(request,response, &id_cliente, &id_carrello, 1);
    sleep(2);
    aggiungi_con_id(request,response, &id_cliente, &id_carrello, 2);
    sleep(2);
    aggiungi_con_id(request,response, &id_cliente, &id_carrello, 1);
    sleep(2);
    stampa(request,response, &id_cliente, &id_carrello);

    posizione=-1;

    do{
        mettiInCoda(request,response, &id_cliente, &id_carrello);
        sscanf(response, "%d\n", &posizione);
        sleep(7);
    }while(posizione!=0);

    do{
        paga(request,response, &id_cliente, &id_carrello);
        sleep(7);
    }while((strcmp(response,"Carrello in elaborazione\n"))==0);
        
    esci(request,response, &id_cliente, &id_carrello);
}

void aggiungi_con_id(char* request, char* response, int* id_cliente, int* id_carrello, int id_prodotto){
    int sockfd=create_socket();
    char nome_prodotto[50];
    float prezzo_prodotto;
    switch (id_prodotto) {
        case 1: strcpy(nome_prodotto, "Pasta"); prezzo_prodotto = 1.99; break;
        case 2: strcpy(nome_prodotto, "Latte"); prezzo_prodotto = 0.99; break;
        case 3: strcpy(nome_prodotto, "Pane"); prezzo_prodotto = 2.49; break;
        case 4: strcpy(nome_prodotto, "Salsa di pomodoro"); prezzo_prodotto = 1.79; break;
        case 5: strcpy(nome_prodotto, "Pollo"); prezzo_prodotto = 5.99; break;
        case 6: strcpy(nome_prodotto, "Uova"); prezzo_prodotto = 2.29; break;
        case 7: strcpy(nome_prodotto, "Banane"); prezzo_prodotto = 0.69; break;
        case 8: strcpy(nome_prodotto, "Yogurt"); prezzo_prodotto = 1.49; break;
        case 9: strcpy(nome_prodotto, "Cereali"); prezzo_prodotto = 3.99; break;
        case 10: strcpy(nome_prodotto, "Acqua minerale"); prezzo_prodotto = 0.89; break;
    }
    sprintf(request, "cliente:%d:aggiungi\n:%d:%s:%f", id_carrello, id_prodotto, nome_prodotto, prezzo_prodotto);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

void send_request(int socket, char * request) {
    if(write(socket, request, strlen(request)) == -1) perror("Write"), exit(1);
}

void read_response(int socket, char * response) {
    memset(response, 0, MAX_RESPONSE_SIZE);
    if(read(socket, response, MAX_RESPONSE_SIZE) == -1) perror("Read"), exit(1);
}

int getScelta() {
    int scelta;
    while (1) {
        printmenu();
        printf("Inserisci la tua scelta: ");
        scanf("%d", &scelta);
        if (scelta >= 1 && scelta <= 9) {
            return scelta;
        } else {
            system("clear");
            printf("Scelta non valida\n\n");
        }
    }
}

void printmenu() {
    printf("1. Mettiti in coda per entrare al negozio\n");
    printf("2. Entra nel negozio\n");
    printf("3. Stampa catalogo\n");
    printf("4. Aggiungi un prodotto al carrello\n");
    printf("5. Rimuovi un prodotto dal carrello\n");
    printf("6. Stampa il carrello\n");
    printf("7. Mettiti in coda alla cassa\n");
    printf("8. Paga\n");
    printf("9. Esci dal negozio\n");
}

void ingresso(char* request, char* response, int* id_cliente, int* id_carrello){
    int position=-1;
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:ingresso\n", id_cliente);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    if (strstr(response, "ID_cliente") != NULL) sscanf(response, "ID_cliente:%d:%d\n", &id_cliente, &position);
    close(sockfd);
}

void entra(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:entra\n", id_carrello);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    if (strstr(response, "ID_carrello") != NULL) sscanf(response, "ID_carrello:%d\n", &id_carrello);
    close(sockfd);
}

void esci(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:esce\n", id_carrello);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

void stampaCatalogo(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "catalogo\n");
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}


void aggiungi(char* request, char* response, int* id_cliente, int* id_carrello) {
    int id_prodotto;
    printf("1. Pasta\n");
    printf("2. Latte\n");
    printf("3. Pane\n");
    printf("4. Salsa di pomodoro\n");
    printf("5. Pollo\n");
    printf("6. Uova\n");
    printf("7. Banane\n");
    printf("8. Yogurt\n");
    printf("9. Cereali\n");
    printf("10. Acqua minerale\n");

    printf("Inserisci l'ID del prodotto: ");
    scanf("%d", &id_prodotto);
    aggiungi_con_id(request,response, id_cliente, id_carrello, id_prodotto);
}

void rimuovi(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    int id_prodotto;
    printf("Inserisci l'ID del prodotto da rimuovere: ");
    scanf("%d", &id_prodotto);
    sprintf(request, "cliente:%d:rimuovi\n:%d", id_carrello, id_prodotto);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

void stampa(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:stampa", id_carrello);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

void mettiInCoda(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:coda", id_carrello);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

void paga(char* request, char* response, int* id_cliente, int* id_carrello) {
    int sockfd=create_socket();
    sprintf(request, "cliente:%d:paga", id_carrello);
    send_request(sockfd, request);
    read_response(sockfd, response);
    printf("%s\n", response);
    close(sockfd);
}

