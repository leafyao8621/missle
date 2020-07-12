CC = gcc
SRC = $(wildcard src/*.c)\
	$(wildcard src/engine/*.c)\
	$(wildcard src/model/*.c)\
	$(wildcard src/runner/*.c)\
	$(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -lm -pthread
BIN = main

%.o: %.c
	$(CC) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(CFLAGS)

.PHONY: clean
clean: $(BIN)
	@rm $(OBJ) $(BIN)
