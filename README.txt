# CONFIGURAZIONE DI DEFAULT #

Numero casse: 2
Variabile C (Massimo numero di clienti): 10
Variabile E (Range di variazione del numero di client): 3



# CONTENUTO DELLA DIRECTORY #

ProgettoLSO
├── Client
│   ├── Dockerfile----------------------------> File per la creazione dell'immagine dell'autopilota
│   ├── Makefile------------------------------> File per la compilazione del client e dell'autopilota
│   ├── autopilota.c--------------------------> File sorgente dell'autopilota
│   ├── autopilota.sh-------------------------> Script per il lancio di più autopiloti
│   └── cliente.c-----------------------------> File sorgente del client da terminale
│
├── ClientAndroid.zip-------------------------> Cartella compressa con il codice sorgente del client Android
│
├── Server
│   ├── Dockerfile----------------------------> File per la creazione dell'immagine del server
│   ├── Makefile------------------------------> File per la compilazione del server
│   ├── carrello.c----------------------------> File sorgente del carrello
│   ├── carrello.h----------------------------> File header del carrello
│   ├── cassiere.c----------------------------> File sorgente del cassiere
│   ├── cassiere.h----------------------------> File header del cassiere
│   ├── catalogo.json-------------------------> File contenente il catalogo dei prodotti
│   ├── cliente.c-----------------------------> File sorgente del cliente
│   ├── cliente.h-----------------------------> File header del cliente
│   ├── codaCassa.c---------------------------> File sorgente della coda della cassa
│   ├── codaCassa.h---------------------------> File header della coda della cassa
│   ├── codaIngresso.c------------------------> File sorgente della coda d'ingresso
│   ├── codaIngresso.h------------------------> File header della coda d'ingresso
│   ├── config.h------------------------------> File header per la configurazione del server
│   ├── direttore.c---------------------------> File sorgente del direttore
│   ├── direttore.h---------------------------> File header del direttore
│   └── main.c--------------------------------> File sorgente principale del server
│
├── Makefile----------------------------------> File per la compilazione dell'intero progetto
├── README.txt--------------------------------> Questo file
├── app-debug.apk-----------------------------> File per l'installazione dell'applicazione su Android
└── compose.yaml------------------------------> File per la creazione dei container



# IL CODICE SORGENTE DEL CLIENT ANDROID E' VISIBILE SU GITHUB NEL RAMO MASTER #



# ISTRUZIONI PER LA COMPILAZIONE #

All'interno della directory del progetto è possibile compilare il client e il server con il comando:

    make

Per la compilazione del client e del server è necessario avere installato il compilatore `gcc` e il gestore di pacchetti `make`.

E' possibile compilare il client, il server e l'autopilota separatamente con i comandi:

    make client
    make server
    make autopilota

E' possibile eliminare i file oggetto e i file eseguibili con il comando:
    
    make clean




# ISTRUZIONI PER L'ESECUZIONE #

Una volta compilati i programmi, è possibile avviare il server con il comando:

    ./server

In questo modo il server sarà in ascolto su `localhost` sulla porta `5050`.
E' possibile avviare il server con un'interfaccia grafica con il comando:

    ./server UI

In questo modo il server sarà in ascolto su `localhost` sulla porta `5050` e verrà visualizzata l'interfaccia grafica.

Per avviare il client è sufficiente lanciare il comando:

    ./client

In questo modo il client si connetterà automaticamente al server e sarà possibile interagire con il supermercato tramite il terminale.

E' possibile avviare un autopilota con il comando:

    ./autopilota

In questo modo l'autopilota si connetterà automaticamente al server e simulerà l'arrivo di un cliente che effettuerà le seguenti operazioni:
    - Attende in fila all'ingresso
    - Entra nel supermercato
    - Mette 3 prodotti nel carrello
    - Si mette in cola alle casse
    - Paga
    - Esce dal supermercato
L'autopilota avvia un cliente con un ritardo casuale che varia da 0.1 a 2.0 secondi per simulare l'arrivo di clienti casuali.

E' possibile avviare più autopiloti con il comando:

    ./autopilota.sh N

Dove `N` è il numero di autopiloti da avviare.
Si consiglia di avviare al massimo 20 autopiloti per evitare sovraccarichi al server.




# ISTRUZIONI PER DOCKER-COMPOSE #

Per avviare il progetto è necessario avere installato Docker e Docker-Compose.
Una volta installati, posizionarsi nella directory del progetto e lanciare il comando:

    docker-compose up

Per terminare l'esecuzione del progetto è sufficiente premere `CTRL+C` e lanciare il comando:

    docker-compose down

L'immagine dell'autopilota e del server verranno create automaticamente e i container verranno avviati.
I programmi saranno compilati e avviati automaticamente.
Il client si connetterà automaticamente al server e lancererà 20 autopiloti.




# AUTORI #

    - Davide Di Pierro (N86004755)
    - Emilia Napolano (N86004758)



# DISCLAIMER #

Attualmente all'interno della schermata principale del client Android è presente una textbox che permette di inserire
l'indirizzo IP del server, ciò è stato introdotto al solo scopo di ridurre le ore di utilizzo del server di AWS e non 
è da ritenersi parte del prodotto finale.

