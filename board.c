#include "board.h"
#include <stdio.h>


//Set board to starting position
void initBoard(Board* board){
	for(int y=0;y<BOARD_SIZE;y++){
		for(int x=0;x<BOARD_SIZE;x++){
			board->fields[x][y]=EMPTY;
		}
	}
	board->fields[3][3]=WHITE;
	board->fields[4][4]=WHITE;
	board->fields[3][4]=BLACK;
	board->fields[4][3]=BLACK;
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

//Demonstrates board functionality
/*
int main(void)
{
	Board currentBoard;
	initBoard(&currentBoard);
	printBoard(&currentBoard);
}
*/