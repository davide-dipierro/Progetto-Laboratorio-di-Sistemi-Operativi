#ifndef CODAINGRESSO_H
#define CODAINGRESSO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include "config.h"


// Mutex per la coda di ingresso
extern pthread_mutex_t mutex_coda_ingresso;

// Nodo della coda
typedef struct nodoIngresso {
    int id_cliente;
    struct nodoIngresso* next;
} nodoIngresso_t;

typedef struct {
    nodoIngresso_t* head;
    nodoIngresso_t* tail;
} coda_ingresso_t;

void aggiungi_cliente_coda_ingresso(int, coda_ingresso_t*);
int rimuovi_cliente_coda_ingresso(coda_ingresso_t*);
int rimuovi_cliente_coda_ingresso_id(int, coda_ingresso_t*);
int numero_clienti_coda_ingresso(coda_ingresso_t*);
int posizione_cliente_coda_ingresso(int, coda_ingresso_t*);


#endif