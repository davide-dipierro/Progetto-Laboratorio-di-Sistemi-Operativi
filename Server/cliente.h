void clienteEntra(){
    pthread_mutex_lock(&mutex_clienti);
    clienti++;
    printf("Cliente entrato, clienti in negozio: %d\n", clienti);
    pthread_mutex_unlock(&mutex_clienti);
}
