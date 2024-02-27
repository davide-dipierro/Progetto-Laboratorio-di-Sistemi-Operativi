#include "cassiere.h"

void cassiereEntra(int id, int tempoCassiere, int tempoElaborazioneProdotto, carrello_t* carrelli, coda_casse_t* coda_casse) {
    pthread_t thread_cassiere;
    cassiere_t* cassiere = (cassiere_t*)malloc(sizeof(cassiere_t));
    cassiere->id = id;
    cassiere->tempoCassiere = tempoCassiere;
    cassiere->tempoElaborazioneProdotto = tempoElaborazioneProdotto;
    cassiere->carrelli = carrelli;
    cassiere->coda_casse = coda_casse;
    if(pthread_create(&thread_cassiere, NULL, aspettaFila, (void*)cassiere) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
} 

void* aspettaFila(void* ptr) {
    cassiere_t* cassiere = (cassiere_t*)ptr;
    coda_casse_t* coda_casse = cassiere->coda_casse;
    while(1) {
        pthread_t thread_elabora_carrello;
        pthread_mutex_lock(&mutex_cassieri);
        if(coda_casse->head != NULL) { 
            printf("[CASSIERE %d] C'è qualcuno in fila\n", cassiere->id);
            if(pthread_create(&thread_elabora_carrello, NULL, elaboraCarrello, (void*)cassiere) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
            pthread_join(thread_elabora_carrello, NULL);
            printf("[CASSIERE %d] Carrello elaborato\n", cassiere->id);
        } else {
            pthread_mutex_unlock(&mutex_cassieri);
        }
        sleep(cassiere->tempoCassiere);
    }
    pthread_exit(NULL);
}

void* elaboraCarrello(void* ptr) {
    cassiere_t* cassiere = (cassiere_t*)ptr;
    carrello_t* carrelli = cassiere->carrelli;
    coda_casse_t* coda_casse = cassiere->coda_casse;
    int id = coda_casse->head->id_cliente;
    rimuovi_cliente_coda_id(id, coda_casse);
    pthread_mutex_unlock(&mutex_cassieri);
    pthread_mutex_lock(&carrelli[id].mutex);
    carrelli[id].status = IN_CASSA;
    for(int i = 0; i < carrelli[id].n_prodotti; i++) sleep(cassiere->tempoElaborazioneProdotto);
    sleep(cassiere->tempoCassiere);
    carrelli[id].status = PAGAMENTO;
    pthread_mutex_unlock(&carrelli[id].mutex);
    pthread_exit(NULL);
}

