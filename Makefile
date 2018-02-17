CC = gcc

NCURSES_LIB = -lncurses -I/usr/include
CFLAGS = -g -Wall -Wextra -std=c11

DEPS = board.h logic.h player.h debug.h inputParser.h records.h debug.h render.h AI_Logic.h
OBJ = game.o logic.o player.o board.o inputParser.o records.o render.o render.o AI_Logic.o
OTHELLO = othello

all: $(OTHELLO)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OTHELLO): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(NCURSES_LIB)

clean:
	rm -f $(OBJ)