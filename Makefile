CC = gcc
SRC = $(wildcard src/*.c) $(wildcard src/engine/*.c) $(wildcard src/model/*.c) $(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
BIN = main

%.o: %.c
	$(CC) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lm

.PHONY: clean
clean: $(BIN)
	@rm $(OBJ) $(BIN)
