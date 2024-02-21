#ifndef CLIENTECONTROLLER_H 
#define CLIENTECONTROLLER_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "carrello.h"
#include "codaCassa.h"
#include "config.h"
#include "codaIngresso.h"

void decrementa_n_clienti();
void incrementa_n_clienti();
int get_n_clienti();

void clienteEntra(int*, char*, carrello_t*, coda_ingresso_t*);
void clienteEsce(int, char*, carrello_t*);
void clienteParser(char*, char*, carrello_t*, coda_casse_t*, coda_ingresso_t*);
void clienteAggiunge(int, char*, char*, carrello_t*);
void clienteRimuove(int, char*, char*, carrello_t*);
void clienteStampa(int, char*, carrello_t*);
void clienteSiMetteInCodaAllaCassa(int, char*, carrello_t*, coda_casse_t*);
void clientePaga(int, char*, carrello_t*);
void clienteEntraInCodaIngresso(int, char*, coda_ingresso_t*);

#endif