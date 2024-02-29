#ifndef DIRETTORE_H
#define DIRETTORE_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "carrello.h"
#include "config.h"

void* controllaUscita(void*);

#endif