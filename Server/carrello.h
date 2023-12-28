// Scopo: Definire le funzioni per la gestione della coda

// Librerie
#include <stdio.h>
#include <stdlib.h>

// Struttura dati per la coda
typedef struct carrello {
    int id_prodotto;
    struct carrello *next;
} carrello_t;

//array di carrelli
carrello_t carrelli[MAX_CLIENTI];


// Variabili per la coda
carrello_t *head[MAX_CLIENTI] = {NULL};
carrello_t *tail[MAX_CLIENTI] = {NULL};

// Prototipi delle funzioni
void acquista(int n_carrello, int id_prodotto);
void vendi(int n_carrello);

// Funzione per l'inserimento di un cliente nella coda
void acquista(int n_carrello, int id_prodotto) {
    carrello_t *node = (carrello_t*)malloc(sizeof(carrello_t));
    node->id_prodotto = id_prodotto;
    node->next = NULL;
    if (head[n_carrello] == NULL) {
        head[n_carrello] = node;
        tail[n_carrello] = node;
    } else {
        tail[n_carrello]->next = node;
        tail[n_carrello] = node;
    }
}

// Funzione per l'estrazione di un cliente dalla coda
void vendi(int n_carrello) {
    carrello_t *node = head[n_carrello];
    head[n_carrello] = head[n_carrello]->next;
    free(node);
}
