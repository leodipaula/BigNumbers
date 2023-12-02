CC = gcc
CFLAGS = -std=c99 -Wall

all: client

client: client.c bignumber.c
	$(CC) $(CFLAGS) -o client client.c bignumber.c

clean:
	rm -f client