# Libraries are -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

CC = clang
CFLAGS = -std=c18 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-unused-variable 
CFLAGS += -Wno-gnu-statement-expression -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments

LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: dirs game

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
	@echo "Cleaned out object files and bin/ directory!"
