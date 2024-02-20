#include "cassiere.h"

void cassiereEntra(int tempoCassiere, int tempoElaborazioneProdotto, carrello_t* carrelli, coda_casse_t* coda_casse) {
    pthread_t thread_cassiere;
    cassiere_t* cassiere = (cassiere_t*)malloc(sizeof(cassiere_t));
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
        //printf("Aspetto fila\n");
        pthread_t thread_elabora_carrello;
        pthread_mutex_lock(&mutex_coda_casse);
        if(coda_casse->head != NULL) { 
            printf("C'è qualcuno in fila\n");
            pthread_mutex_unlock(&mutex_coda_casse);
            if(pthread_create(&thread_elabora_carrello, NULL, elaboraCarrello, (void*)cassiere) < 0) perror("Could not create thread"), exit(EXIT_FAILURE);
            pthread_join(thread_elabora_carrello, NULL);
            printf("Elaborato carrello\n");
        } else {
            pthread_mutex_unlock(&mutex_coda_casse);
        }
        sleep(cassiere->tempoCassiere);
    }
    pthread_exit(NULL);
}

void* elaboraCarrello(void* ptr) {
    printf("Elaboro carrello\n");
    cassiere_t* cassiere = (cassiere_t*)ptr;
    carrello_t* carrelli = cassiere->carrelli;
    coda_casse_t* coda_casse = cassiere->coda_casse;
    int id = coda_casse->head->id_cliente;
    rimuovi_cliente_coda_id(id, coda_casse);
    pthread_mutex_lock(&carrelli[id].mutex);
    for(int i = 0; i < carrelli[id].n_prodotti; i++) sleep(cassiere->tempoElaborazioneProdotto);
    sleep(cassiere->tempoCassiere);
    carrelli[id].status = PAGAMENTO;
    pthread_mutex_unlock(&carrelli[id].mutex);
    pthread_exit(NULL);
}

