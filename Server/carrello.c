#include "carrello.h"

void aggiungi_prodotto(carrello_t* carrello, prodotto_t prodotto) {
    // Creazione del nuovo nodo_carr
    noto_t_carr* nuovo_nodo_carr = (noto_t_carr*)malloc(sizeof(noto_t_carr));
    nuovo_nodo_carr->prodotto = prodotto;
    nuovo_nodo_carr->next = NULL;

    if (carrello->head == NULL) {
        // La coda è vuota, il nuovo nodo_carr diventa la testa e la coda
        carrello->head = nuovo_nodo_carr;
        carrello->tail = nuovo_nodo_carr;
    } else {
        // Aggiunta del nuovo nodo_carr in coda
        carrello->tail->next = nuovo_nodo_carr;
        carrello->tail = nuovo_nodo_carr;
    }

    carrello->n_prodotti++;
}

void rimuovi_prodotto(carrello_t* carrello, int id_prodotto) {
    noto_t_carr* nodo_carr_corrente = carrello->head;
    noto_t_carr* nodo_carr_precedente = NULL;

    while (nodo_carr_corrente != NULL) {
        if (nodo_carr_corrente->prodotto.id == id_prodotto) {
            // Rimozione del nodo_carr corrente
            if (nodo_carr_precedente == NULL) {
                // Il nodo_carr corrente è la testa
                carrello->head = nodo_carr_corrente->next;
            } else {
                // Il nodo_carr corrente non è la testa
                nodo_carr_precedente->next = nodo_carr_corrente->next;
            }

            // Se il nodo_carr corrente è la coda, aggiornamento della coda
            if (nodo_carr_corrente == carrello->tail) {
                carrello->tail = nodo_carr_precedente;
            }

            // Deallocazione del nodo_carr corrente
            free(nodo_carr_corrente);

            carrello->n_prodotti--;
            return;
        }

        nodo_carr_precedente = nodo_carr_corrente;
        nodo_carr_corrente = nodo_carr_corrente->next;
    }
}

void stampa_carrello(carrello_t* carrello) {
    noto_t_carr* nodo_carr_corrente = carrello->head;

    while (nodo_carr_corrente != NULL) {
        printf("ID: %d, Nome: %s, Prezzo: %.2f\n", nodo_carr_corrente->prodotto.id, nodo_carr_corrente->prodotto.nome, nodo_carr_corrente->prodotto.prezzo);
        nodo_carr_corrente = nodo_carr_corrente->next;
    }
}

float calcola_totale(carrello_t* carrello) {
    int totale = 0;
    noto_t_carr* nodo_carr_corrente = carrello->head;

    while (nodo_carr_corrente != NULL) {
        totale += nodo_carr_corrente->prodotto.prezzo;
        nodo_carr_corrente = nodo_carr_corrente->next;
    }

    return totale;
}

void inizializza_carrelli(carrello_t* carrelli) {
    for (int i = 0; i < MAX_CLIENTI; i++) {
        carrelli[i].status = LIBERO;
        carrelli[i].n_prodotti = 0;
        carrelli[i].head = NULL;
        carrelli[i].tail = NULL;
    }
}