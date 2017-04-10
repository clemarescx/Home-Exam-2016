#ifndef __BOARD_H__
#define __BOARD_H__

//#include "logic.h"


#define BOARD_SIZE 8

//Defines the three states each field in a reversi board may have.
//Uses ASCII-characters as values to make it easier to draw the board.
typedef enum Field {
    EMPTY = ' ',
    BLACK = '@',
    WHITE = '.'
} Field;

//Struct for storing a board
typedef struct Board {
    Field fields[BOARD_SIZE][BOARD_SIZE];
} Board;

// struct to store a position on the board
typedef struct Position {
    int x;
    int y;
} Position;



void initBoard();

void printBoard(const Board *board);

int outOfBounds(Position pos);

Field getField(Board *board, Position *position);

int poscmp(Position *pos1, Position *pos2);

#endif //__BOARD_H__
