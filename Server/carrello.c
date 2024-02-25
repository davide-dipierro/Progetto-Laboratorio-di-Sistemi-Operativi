#include "carrello.h"

void aggiungi_prodotto(carrello_t* carrello, prodotto_t prodotto) {
    // Creazione del nuovo nodo_carr
    nodo_t_carr* nuovo_nodo_carr = (nodo_t_carr*)malloc(sizeof(nodo_t_carr));
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

bool rimuovi_prodotto(carrello_t* carrello, int id_prodotto) {
    nodo_t_carr* nodo_carr_corrente = carrello->head;
    nodo_t_carr* nodo_carr_precedente = NULL;

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
            return true;
        }

        nodo_carr_precedente = nodo_carr_corrente;
        nodo_carr_corrente = nodo_carr_corrente->next;
    }
    return false;
}

void stampa_carrello(char* stringa, carrello_t* carrello) {
    nodo_t_carr* nodo_carr_corrente = carrello->head;

    while (nodo_carr_corrente != NULL) {
        sprintf(stringa, "%s{%d:%s:%f}\n", stringa, nodo_carr_corrente->prodotto.id, nodo_carr_corrente->prodotto.nome, nodo_carr_corrente->prodotto.prezzo);
        nodo_carr_corrente = nodo_carr_corrente->next;
    }
}


float calcola_totale(carrello_t* carrello) {
    int totale = 0;
    nodo_t_carr* nodo_carr_corrente = carrello->head;

    while (nodo_carr_corrente != NULL) {
        totale += nodo_carr_corrente->prodotto.prezzo;
        nodo_carr_corrente = nodo_carr_corrente->next;
    }

    return totale;
}

void inizializza_carrelli(carrello_t* carrelli) {
    for (int i = 0; i < VARIABILE_C; i++) {
        carrelli[i].status = LIBERO;
        carrelli[i].n_prodotti = 0;
        carrelli[i].head = NULL;
        carrelli[i].tail = NULL;
        carrelli[i].ultima_operazione = time(NULL);
    }
}


void svuota_carrello(carrello_t* carrello) {
    nodo_t_carr* nodo_carr_corrente = carrello->head;
    nodo_t_carr* nodo_carr_successivo = NULL;

    while (nodo_carr_corrente != NULL) {
        // Salva il puntatore al prossimo nodo
        nodo_carr_successivo = nodo_carr_corrente->next;

        // Rimuovi il nodo corrente
        rimuovi_prodotto(carrello, nodo_carr_corrente->prodotto.id);

        // Avanza al nodo successivo
        nodo_carr_corrente = nodo_carr_successivo;
    }
}



