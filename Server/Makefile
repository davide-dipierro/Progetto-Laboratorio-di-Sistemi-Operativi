CC = gcc
#CFLAGS = -Wall -Wextra

SERVER_SRCS = server.c
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

CLIENTE_CONTROLLER_SRCS = clientecontroller.c
CLIENTE_CONTROLLER_OBJS = $(CLIENTE_CONTROLLER_SRCS:.c=.o)

CARRELLO_SRCS = carrello.c
CARRELLO_OBJS = $(CARRELLO_SRCS:.c=.o)

all: server

server: $(SERVER_OBJS) $(CLIENTE_CONTROLLER_OBJS) $(CARRELLO_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f server $(SERVER_OBJS) $(CLIENTE_CONTROLLER_OBJS) $(CARRELLO_OBJS)
