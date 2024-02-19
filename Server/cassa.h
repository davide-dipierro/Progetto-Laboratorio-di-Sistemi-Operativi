#ifndef CASSE_H
#define CASSE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include "config.h"

// Mutex per la coda delle casse
extern pthread_mutex_t mutex_coda_casse;

// Nodo della coda
typedef struct nodo {
    int id_cliente;
    struct nodo* next;
} nodo_t;

// Struttura dati per la cassa
typedef struct {
    int id; // Identificativo della cassa
    bool aperta; // Stato della cassa
    int cliente; // Stato della cassa (-1 = non c'è gente)
} cassa_t;

// Struttura dati per la coda alle casse
typedef struct {
    nodo_t* head;
    nodo_t* tail;
} coda_casse_t;

void aggiungi_cliente_coda(int, coda_casse_t*);
int rimuovi_cliente_coda(coda_casse_t*);
int rimuovi_cliente_coda_id(int, coda_casse_t*);
int numero_clienti_coda(coda_casse_t*);
int posizione_cliente_coda(int, coda_casse_t*);

#endif