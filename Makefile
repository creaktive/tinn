BIN = test

CFLAGS = -std=c99 -Wall -Wextra -pedantic -Ofast -flto -march=native

LDFLAGS = -lm

CC = gcc

SRC = test.c Tinn.c

all:
	$(CC) -o $(BIN) $(SRC) $(CFLAGS) $(LDFLAGS)
	$(CC) -o batch batch.c Tinn.c $(CFLAGS) $(LDFLAGS)

run:
	./$(BIN)

clean:
	rm -f $(BIN)
