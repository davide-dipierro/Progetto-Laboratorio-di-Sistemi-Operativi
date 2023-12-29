#ifndef CARRELLO_H
#define CARRELLO_H
#define MAX_PRODOTTI 10

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdbool.h>

// Struttura dati per il prodotto
typedef struct {
    int id;
    char nome[50];
    float prezzo;
} prodotto_t;

// Nodo della coda
typedef struct nodo {
    prodotto_t prodotto;
    struct nodo* next;
} nodo_t;

// Struttura dati per la carrello
typedef struct {
    bool occupato; // 0 = libero, 1 = occupato
    int n_prodotti; // Numero di prodotti nel carrello
    nodo_t* head; // Puntatore alla testa della coda
    nodo_t* tail; // Puntatore alla coda della coda
} carrello_t;

void rimuovi_prodotto(carrello_t*, int);
void stampa_carrello(carrello_t*);
void aggiungi_prodotto(carrello_t*, prodotto_t);
float calcola_totale(carrello_t*);
void rimuovi_prodotto(carrello_t*, int);

#endif