#include "board.h"
#include <stdio.h>

Board gameBoard;
Board *pBoard = &gameBoard;

//Set board to starting position
void initBoard() {
	for(int y=0;y<BOARD_SIZE;y++){
		for(int x=0;x<BOARD_SIZE;x++){
            pBoard->fields[x][y] = EMPTY;
		}
	}
    pBoard->fields[3][3] = WHITE;
    pBoard->fields[4][4] = WHITE;
    pBoard->fields[3][4] = BLACK;
    pBoard->fields[4][3] = BLACK;
}

//Prints the board using traditional ascii-art type graphics 
void printBoard(const Board* board){
	printf("    a   b   c   d   e   f   g   h  \n");
	for(int y=0;y<BOARD_SIZE;y++){
		printf("  +---+---+---+---+---+---+---+---+\n");
		printf(" %d",y+1); //Computer counts from 0, the game counts from 1!
		for(int x=0;x<BOARD_SIZE;x++){
			printf("| %c ",board->fields[x][y]);
		}
		printf("| %d\n",y+1); //Computer counts from 0, the game counts from 1!
	}
	printf("  +---+---+---+---+---+---+---+---+\n");
	printf("    a   b   c   d   e   f   g   h  \n");
}

int isOutOfBounds(int x, int y) {
    return (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE);
}

/*
Field getField(Board gameBoard, int x, int y) {
    return gameBoard.fields[x][y];
}
*/

Field getField(Position *position) {
    return pBoard->fields[position->x][position->y];
}

//Demonstrates board functionality
/*
int main(void)
{
	Board currentBoard;
	initBoard(&currentBoard);
	printBoard(&currentBoard);
}
*/