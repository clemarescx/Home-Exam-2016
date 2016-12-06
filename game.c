#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "player.h"


int playerInit(Player *player);

int main(void){

    Player player1, player2;

    if ( !playerInit(&player1) ) return EXIT_FAILURE;
    player1.token = WHITE;

    if ( !playerInit(&player2) ) return EXIT_FAILURE;
    player2.token = BLACK;


    printf("WELCOOOME, %s AND %s!!!\n", player1.name, player2.name);
    getchar();

    Board gameBoard;

    initBoard(&gameBoard);
    printBoard(&gameBoard);

    free(player1.name);
    free(player2.name);

    return 0;
}


/**
 * Player name entry
 * @param player
 * @return
 */
int playerInit(Player *player){

    player->score = 2;

    char *nameInput = (char *) malloc( 51 * sizeof(char));
    player->name = (char *) malloc( 51 * sizeof(char));

    printf("Player %d, please enter your name: ", playerCount+1);

    if( fgets(nameInput, 51, stdin) == NULL){
        printf("An error occured.\n");
        return 0;
    }

    sscanf(nameInput,"%s", player->name);
    playerCount++;
    free(nameInput);

    return 1;
}