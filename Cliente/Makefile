CC = gcc
CFLAGS = -Wall -Wextra

autopilota: autopilota.c
	$(CC) $(CFLAGS) -o autopilota autopilota.c
	$(CC) $(CFLAGS) -o client cliente.c

clean:
	rm -f autopilota
	rm -f client
	rm -f log*
