CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address

SRCS = main.c codaCassa.c carrello.c cassiere.c codaIngresso.c cliente.c
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
