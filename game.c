#include <stdio.h>

#include "board.h"
#include "player.h"


int main(int argc, char **argv){

    Board gameBoard;

    initBoard(&gameBoard);
    printBoard(&gameBoard);

    //scanf()

    return 0;
}

