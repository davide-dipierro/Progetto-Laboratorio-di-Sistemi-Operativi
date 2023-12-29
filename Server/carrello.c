#include "carrello.h"

void aggiungi_prodotto(carrello_t* carrello, prodotto_t prodotto) {
    // Creazione del nuovo nodo
    nodo_t* nuovo_nodo = (nodo_t*)malloc(sizeof(nodo_t));
    nuovo_nodo->prodotto = prodotto;
    nuovo_nodo->next = NULL;

    if (carrello->head == NULL) {
        // La coda è vuota, il nuovo nodo diventa la testa e la coda
        carrello->head = nuovo_nodo;
        carrello->tail = nuovo_nodo;
    } else {
        // Aggiunta del nuovo nodo in coda
        carrello->tail->next = nuovo_nodo;
        carrello->tail = nuovo_nodo;
    }

    carrello->n_prodotti++;
    carrello->occupato = true;
}

void rimuovi_prodotto(carrello_t* carrello, int id_prodotto) {
    nodo_t* nodo_corrente = carrello->head;
    nodo_t* nodo_precedente = NULL;

    while (nodo_corrente != NULL) {
        if (nodo_corrente->prodotto.id == id_prodotto) {
            // Rimozione del nodo corrente
            if (nodo_precedente == NULL) {
                // Il nodo corrente è la testa
                carrello->head = nodo_corrente->next;
            } else {
                // Il nodo corrente non è la testa
                nodo_precedente->next = nodo_corrente->next;
            }

            // Se il nodo corrente è la coda, aggiornamento della coda
            if (nodo_corrente == carrello->tail) {
                carrello->tail = nodo_precedente;
            }

            // Deallocazione del nodo corrente
            free(nodo_corrente);

            carrello->n_prodotti--;
            return;
        }

        nodo_precedente = nodo_corrente;
        nodo_corrente = nodo_corrente->next;
    }
}

void stampa_carrello(carrello_t* carrello) {
    nodo_t* nodo_corrente = carrello->head;

    while (nodo_corrente != NULL) {
        printf("ID: %d, Nome: %s, Prezzo: %.2f\n", nodo_corrente->prodotto.id, nodo_corrente->prodotto.nome, nodo_corrente->prodotto.prezzo);
        nodo_corrente = nodo_corrente->next;
    }
}

float calcola_totale(carrello_t* carrello) {
    int totale = 0;
    nodo_t* nodo_corrente = carrello->head;

    while (nodo_corrente != NULL) {
        totale += nodo_corrente->prodotto.prezzo;
        nodo_corrente = nodo_corrente->next;
    }

    return totale;
}