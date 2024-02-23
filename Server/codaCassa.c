#include "codaCassa.h"

void aggiungi_cliente_coda(int id_cliente, coda_casse_t* coda_casse){
    //printf("[TEST-CODA-CASSE] Aggiungo il cliente %d alla coda\n", id_cliente);
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    //printf("[TEST-CODA-CASSE] Mutex acquisito\n");
    nodo_t* nuovo_nodo = (nodo_t*) malloc(sizeof(nodo_t));
    //printf("[TEST-CODA-CASSE] Nodo allocato\n");
    nuovo_nodo->id_cliente = id_cliente;
    nuovo_nodo->next = NULL;
    if(coda_casse->head == NULL){
        coda_casse->head = nuovo_nodo;
        coda_casse->tail = nuovo_nodo;
    } else {
        coda_casse->tail->next = nuovo_nodo;
        coda_casse->tail = nuovo_nodo;
    }
    //printf("[TEST-CODA-CASSE] Cliente %d aggiunto alla coda\n", id_cliente);
    pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
    //printf("[TEST-CODA-CASSE] Mutex rilasciato\n");
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

int rimuovi_cliente_coda_id(int id_cliente, coda_casse_t* coda_casse){
    pthread_mutex_lock(&mutex_coda_casse); // Lock mutex before accessing the queue
    if(coda_casse->head == NULL){
        pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex if the queue is empty
        return -1;
    } else {
        nodo_t* nodo_corrente = coda_casse->head;
        nodo_t* nodo_precedente = NULL;
        while(nodo_corrente != NULL){
            if(nodo_corrente->id_cliente == id_cliente){
                if(nodo_precedente == NULL){
                    coda_casse->head = nodo_corrente->next;
                } else {
                    nodo_precedente->next = nodo_corrente->next;
                }
                free(nodo_corrente);
                pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
                return id_cliente;
            }
            nodo_precedente = nodo_corrente;
            nodo_corrente = nodo_corrente->next;
        }
        pthread_mutex_unlock(&mutex_coda_casse); // Unlock mutex after accessing the queue
        return -1;
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