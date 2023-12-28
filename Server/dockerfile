# Usa un'immagine di base leggera con il compilatore C
FROM alpine:latest

# Aggiorna il repositvory degli apk e installa gcc e libc-dev
RUN apk update && apk add --no-cache gcc libc-dev

# Copia il file sorgente nel container
COPY server.c /usr/src/app/server/
COPY carrello.h /usr/src/app/server/

# Imposta la directory di lavoro
WORKDIR /usr/src/app/server/

# Compila il programma
RUN gcc -o server server.c

# Esegui il programma quando il container viene avviato
CMD ["./server"]
