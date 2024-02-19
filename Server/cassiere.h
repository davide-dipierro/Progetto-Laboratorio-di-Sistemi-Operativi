#ifndef CASSIERE_H
#define CASSIERE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "carrello.h"
#include "codaCassa.h"
#include "config.h"

typedef struct {
    int tempoCassiere;
    int tempoElaborazioneProdotto;
    carrello_t* carrelli;
    coda_casse_t* coda_casse;
} cassiere_t;

extern pthread_mutex_t mutex_coda_casse;

void cassiereEntra(int, int, carrello_t*, coda_casse_t*);
void* aspettaFila(void*);
void* elaboraCarrello(void*);

#endif