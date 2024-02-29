#include "direttore.h"

void* controllaUscita(void* ptr){
    carrello_t* carrelli = (carrello_t*)ptr;
    while(1){
        sleep(TIMER_DIRETTORE);
        for(int i=0; i<VARIABILE_C; i++){
            if(carrelli[i].status == CONFERMA) {
                printf("[DIRETTORE] Carrello %d deve uscire\n", i);
                pthread_mutex_lock(&carrelli[i].mutex);
                carrelli[i].status = PAGATO;
                pthread_mutex_unlock(&carrelli[i].mutex);
                printf("[DIRETTORE] Carrello %d può uscire\n", i);
            }
        }
    }
}