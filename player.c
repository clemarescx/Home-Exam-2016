#include <stdio.h>
#include <stdlib.h>

#include "player.h"

short playerCount = 0;

/**
 * Initialise the players
 * @param player
 * @return
 */
int playerInit(Player *player) {

    player->score = 2;

    char *nameInput = (char *) malloc(51 * sizeof(char));
    player->name = (char *) malloc(51 * sizeof(char));


#ifdef DEBUGON

    snprintf(player->name, 51, "%s%d", "Pl4Y3r-number -", playerCount + 1);

#else
    printf("Player %d, please enter your name: ", playerCount+1);

    if( fgets(nameInput, 51, stdin) == NULL){
        printf("An error occured.\n");
        return 0;
    }
    sscanf(nameInput,"%[^\n]", player->name);
#endif

    playerCount++;
    free(nameInput);

    return 1;
}