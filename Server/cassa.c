#include "cassa.h"

void aggiungi_cliente_coda(int id_cliente, coda_casse_t* coda_casse){
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    nodo_t* nuovo_nodo = (nodo_t*) malloc(sizeof(nodo_t));
    nuovo_nodo->id_cliente = id_cliente;
    nuovo_nodo->next = NULL;
    if(coda_casse->head == NULL){
        coda_casse->head = nuovo_nodo;
        coda_casse->tail = nuovo_nodo;
    } else {
        coda_casse->tail->next = nuovo_nodo;
        coda_casse->tail = nuovo_nodo;
    }
    pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
}

int rimuovi_cliente_coda(coda_casse_t* coda_casse){
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    if(coda_casse->head == NULL){
        pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex if the queue is empty
        return -1;
    } else {
        nodo_t* nodo_da_rimuovere = coda_casse->head;
        int id_cliente = nodo_da_rimuovere->id_cliente;
        coda_casse->head = coda_casse->head->next;
        free(nodo_da_rimuovere);
        pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
        return id_cliente;
    }
}

int numero_clienti_coda(coda_casse_t* coda_casse){
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    int numero_clienti = 0;
    nodo_t* nodo_corrente = coda_casse->head;
    while(nodo_corrente != NULL){
        numero_clienti++;
        nodo_corrente = nodo_corrente->next;
    }
    pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
    return numero_clienti;
}

int posizione_cliente_coda(int id_cliente, coda_casse_t* coda_casse){
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    int posizione = 0;
    nodo_t* nodo_corrente = coda_casse->head;
    while(nodo_corrente != NULL){
        if(nodo_corrente->id_cliente == id_cliente){
            pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
            return posizione;
        }
        posizione++;
        nodo_corrente = nodo_corrente->next;
    }
    pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
    return -1;
}