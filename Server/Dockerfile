# Usa un'immagine di base di Linux
FROM gcc:latest

# Imposta la directory di lavoro nel container
WORKDIR /app

# Copia i file sorgente nel container
COPY main.c codaCassa.c carrello.c cassiere.c codaIngresso.c cliente.c codaCassa.h carrello.h cassiere.h codaIngresso.h cliente.h direttore.h direttore.c config.h catalogo.json /app/

# Compila il programma
RUN gcc -Wall -Wextra -fsanitize=address -pthread -o server main.c codaCassa.c carrello.c cassiere.c codaIngresso.c cliente.c direttore.c

# Espone la porta su cui il server è in ascolto (se necessario)
EXPOSE 5050

# Avvia il server
CMD ["./server", "UI"]