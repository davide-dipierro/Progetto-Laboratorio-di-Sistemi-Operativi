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

#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 1024

void send_request(int socket, char * request);
void read_response(int socket, char * response);

int main(int argc, char** argv) {
    while (1){
    
        int sockfd;
        struct sockaddr_in server_address;

        // Create socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket creation failed");
            exit(1);
        }

        // Set server address
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(5050);
        server_address.sin_addr.s_addr = inet_addr("home.davidedipierro.it");

        // Connect to server
        if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
            perror("Connection failed");
            exit(1);
        }

        char request[MAX_REQUEST_SIZE];
        char response[MAX_RESPONSE_SIZE];

        printf("Inserisci la richiesta: ");
        fgets(request, MAX_REQUEST_SIZE, stdin);
        send_request(sockfd, request);
        read_response(sockfd, response);
        printf("Risposta: %s\n", response);

        close(sockfd);
    }
    return 0;
}

void send_request(int socket, char * request) {
    if(write(socket, request, strlen(request)) == -1) perror("Write"), exit(1);
}

void read_response(int socket, char * response) {
    if(read(socket, response, MAX_RESPONSE_SIZE) == -1) perror("Read"), exit(1);
}

