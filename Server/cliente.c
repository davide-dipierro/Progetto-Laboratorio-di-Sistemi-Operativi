#include "cliente.h"

extern pthread_mutex_t mutex_n_clienti;
int n_clienti = 0;

int get_n_clienti(){
    int n;
    pthread_mutex_lock(&mutex_n_clienti);
    n = n_clienti;
    pthread_mutex_unlock(&mutex_n_clienti);
    return n;
}

void incrementa_n_clienti(){
    pthread_mutex_lock(&mutex_n_clienti);
    n_clienti++;
    pthread_mutex_unlock(&mutex_n_clienti);
}

void decrementa_n_clienti(){
    pthread_mutex_lock(&mutex_n_clienti);
    n_clienti--;
    pthread_mutex_unlock(&mutex_n_clienti);
}

void clienteParser(char* request, char* response, carrello_t* carrelli, coda_casse_t* coda_casse){
    printf("Richiesta: %s\n", request);
    int id;
    char comando[10];
    char dati[100];
    sscanf(request, "cliente:%d:%s\n:%s", &id, comando, dati);
    printf("Comando: %s\n", comando);
    printf("Dati: %s\n", dati);
    if(strcmp(comando, "entra") == 0) clienteEntra(&id, response, carrelli);
    else if(carrelli[id].status == LIBERO) { strcpy(response, "Sessione scaduta\n\0"); return; }
    else if(strcmp(comando, "esce") == 0) clienteEsce(id, response, carrelli);
    else if(strcmp(comando, "aggiungi") == 0) clienteAggiunge(id, request, response, carrelli);
    else if(strcmp(comando, "rimuovi") == 0) clienteRimuove(id, request, response, carrelli);
    else if(strcmp(comando, "stampa") == 0) clienteStampa(id, response, carrelli);
    else if(strcmp(comando, "coda") == 0) clienteSiMetteInCodaAllaCassa(id, response, carrelli, coda_casse);
    else if(strcmp(comando, "paga") == 0) clientePaga(id, response, carrelli);
    else strcpy(response, "Comando non riconosciuto\n\0");

    if(strcmp(comando, "esce") != 0) carrelli[id].ultima_operazione = time(NULL);
    // TODO: Per il momento non c'è la coda per entrare nel negozio
}

void clienteEntra(int* id, char* response, carrello_t* carrelli){
    incrementa_n_clienti();
    printf("Cliente entrato, clienti in negozio: %d\n", get_n_clienti());
    int i = 0;
    while(i < MAX_CLIENTI && carrelli[i].status != LIBERO) i++;
    if(i < MAX_CLIENTI){
        carrelli[i].status = IN_NEGOZIO;
        carrelli[i].ultima_operazione = time(NULL);
        pthread_mutex_lock(&carrelli[i].mutex);
        printf("Carrello %d assegnato al cliente\n", i);
        sprintf(response, "ID_carrello:%d\n", i);
        *id = i;
    }else{
        printf("Non ci sono carrelli disponibili\n");
        strcpy(response, "Non ci sono carrelli disponibili\n\0");
    }
}

void clienteEsce(int id, char* response, carrello_t* carrelli){
    if ( carrelli[id].status == PAGATO ) {
        printf("Cliente uscito, clienti in negozio: %d\n", get_n_clienti());
        svuota_carrello(&carrelli[id]);
        carrelli[id].status = LIBERO;
        decrementa_n_clienti();
        strcpy(response, "Sei uscito dal negozio\n\0");
    } else {
        printf("Richiesta su carrello %d non in pagamento\n", id);
        strcpy(response, "Sessione scaduta\n\0");
    }
    
}

void clienteAggiunge(int id, char* request, char* response, carrello_t* carrelli){
    int id_prodotto;
    char nome_prodotto[50];
    float prezzo_prodotto;
    sscanf(request, "cliente:%d:aggiungi\n:%d:%[^:]:%f", &id, &id_prodotto, nome_prodotto, &prezzo_prodotto);
    prodotto_t prodotto;
    prodotto.id = id_prodotto;
    strcpy(prodotto.nome, nome_prodotto);
    prodotto.prezzo = prezzo_prodotto;
    if(carrelli[id].status == IN_NEGOZIO) {
        aggiungi_prodotto(&carrelli[id], prodotto);
        printf("Prodotto %d aggiunto al carrello %d\n", id_prodotto, id);
        strcpy(response, "ok\n\0");
    } else {
        printf("Richiesta su carrello %d non in negozio\n", id);
        strcpy(response, "Sessione scaduta\n\0");
    }
}

void clienteRimuove(int id, char* request, char* response, carrello_t* carrelli){
    int id_prodotto;
    sscanf(request, "cliente:%d:rimuovi\n%d", &id, &id_prodotto);
    rimuovi_prodotto(&carrelli[id], id_prodotto);
    printf("Prodotto %d rimosso dal carrello %d\n", id_prodotto, id);
    strcpy(response, "Prodotto rimosso dal carrello\n\0");
}

void clienteStampa(int id, char* response, carrello_t* carrelli){
    printf("Stampa carrello %d\n", id);
    stampa_carrello(&carrelli[id]);
    sprintf(response, "Totale: %.2f\n", calcola_totale(&carrelli[id]));
}

void clienteSiMetteInCodaAllaCassa(int id, char* response, carrello_t* carrelli, coda_casse_t* casse){
    printf("Cliente %d si mette in coda CHIAMATA\n", id);
    if(carrelli[id].n_prodotti == 0) {
        sprintf(response, "0\n");
        return;
    }
    printf("Status: %d == %d\n", carrelli[id].status, IN_NEGOZIO);
    if(carrelli[id].status == IN_NEGOZIO) {
        aggiungi_cliente_coda(id, casse);
        carrelli[id].status = IN_CODA;
        printf("Cliente %d si è messo in coda alle casse\n", id);
    }
    int position = posizione_cliente_coda(id, casse);
    if(position == 0 && carrelli[id].status == IN_CODA) {
        carrelli[id].status = IN_CASSA;
        pthread_mutex_unlock(&carrelli[id].mutex);
    }
    sprintf(response, "%d\n", position);
}

void clientePaga(int id, char* response, carrello_t* carrelli) {
    if(carrelli[id].status == PAGAMENTO || carrelli[id].status == PAGATO) {
        sprintf(response, "ok\n");
        carrelli[id].status = PAGATO;
    } else {
        sprintf(response, "Carrello in elaborazione\n");
    }
}