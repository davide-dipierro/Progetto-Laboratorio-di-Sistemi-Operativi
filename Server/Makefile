CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c cliente.c codaCassa.c carrello.c cassiere.c codaIngresso.c
OBJS = $(SRCS:.c=.o)
TARGET = server

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
