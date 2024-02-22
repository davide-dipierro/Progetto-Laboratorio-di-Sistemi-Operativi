#ifndef CARRELLO_H
#define CARRELLO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "config.h"

// Struttura dati per il prodotto
typedef struct {
    int id;
    char nome[50];
    float prezzo;
} prodotto_t;

// nodo_carr della coda
typedef struct nodo_carr {
    prodotto_t prodotto;
    struct nodo_carr* next;
} nodo_t_carr;

typedef enum {
    LIBERO,
    IN_NEGOZIO,
    IN_CODA,
    IN_CASSA,
    PAGAMENTO,
    PAGATO
} stato_carrello_t;

// Struttura dati per la carrello
typedef struct {
    stato_carrello_t status; // Stato del carrello
    time_t ultima_operazione; // Ultima operazione effettuata sul carrello
    int n_prodotti; // Numero di prodotti nel carrello
    nodo_t_carr* head; // Puntatore alla testa della coda
    nodo_t_carr* tail; // Puntatore alla coda della coda
    pthread_mutex_t mutex; // Mutex per il carrello
} carrello_t;


void stampa_carrello(char*, carrello_t*);
void aggiungi_prodotto(carrello_t*, prodotto_t);
float calcola_totale(carrello_t*);
void rimuovi_prodotto(carrello_t*, int);
void inizializza_carrelli(carrello_t*);
void svuota_carrello(carrello_t*);

#endif