CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -pthread

CLIENT_SRC = Client/cliente.c
SERVER_SRC = Server/main.c Server/codaCassa.c Server/carrello.c Server/cassiere.c Server/codaIngresso.c Server/cliente.c Server/direttore.c
AUTOPILOTA_SRC = Client/autopilota.c

CLIENT_DIR = Client
SERVER_DIR = Server

CLIENT_OBJS = $(CLIENT_SRC:$(CLIENT_DIR)/%.c=$(CLIENT_DIR)/%.o)
SERVER_OBJS = $(SERVER_SRC:$(SERVER_DIR)/%.c=$(SERVER_DIR)/%.o)
AUTOPILOTA_OBJS = $(AUTOPILOTA_SRC:$(CLIENT_DIR)/%.c=$(CLIENT_DIR)/%.o)

CLIENT_TARGET = $(CLIENT_DIR)/client
SERVER_TARGET = $(SERVER_DIR)/server
AUTOPILOTA_TARGET = $(CLIENT_DIR)/autopilota

.PHONY: all client server autopilota clean

all: client server autopilota

client: $(CLIENT_TARGET)

server: $(SERVER_TARGET)

autopilota: $(AUTOPILOTA_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS)

$(AUTOPILOTA_TARGET): $(AUTOPILOTA_OBJS)
	$(CC) $(CFLAGS) -o $@ $(AUTOPILOTA_OBJS)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS)

$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_DIR)/%.o: $(SERVER_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_TARGET) $(CLIENT_DIR)/*.o $(SERVER_TARGET) $(SERVER_DIR)/*.o $(AUTOPILOTA_TARGET) $(CLIENT_DIR)/*.o log*
