# Usa un'immagine di base di Linux
FROM gcc:latest

# Imposta la directory di lavoro nel container
WORKDIR /app

# Copia i file sorgente nel container
COPY autopilota.c autopilota.sh /app/

# Compila il programma e specifica il nome dell'eseguibile di output
RUN gcc -o autopilota autopilota.c

# Rendi il file autopilota.sh eseguibile
RUN chmod +x autopilota.sh

# Imposta il comando di avvio del server
CMD ["./autopilota.sh", "20"]