#ifndef CLIENTECONTROLLER_H 
#define CLIENTECONTROLLER_H
#define MAX_CLIENTI 3

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "carrello.h"

void clienteEntra(int, char*, carrello_t*);
void clienteEsce(int, char*, carrello_t*);
void clienteParser(char*, char*, carrello_t*);
void clienteAggiunge(int, char*, char*, carrello_t*);
void clienteRimuove(int, char*, char*, carrello_t*);
void clienteStampa(int, char*, carrello_t*);


#endif