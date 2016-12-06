#ifndef __BOARD_H__
#define __BOARD_H__


#define BOARD_SIZE 8

//Defines the three states each field in a reversi board may have.
//Uses ASCII-characters as values to make it easier to draw the board.
typedef enum Field{
EMPTY=' ',
BLACK='@',
WHITE='.'
} Field;

//Struct for storing a board
typedef struct Board{
	Field fields [BOARD_SIZE][BOARD_SIZE];
} Board;
#endif //__BOARD_H__

void initBoard(Board* board);
void printBoard(const Board* board);