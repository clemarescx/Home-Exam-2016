#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "stringParser.h"

short playerCount = 0;

/**
 * Initialise a player struct variables and name;
 * @param player
 * @return
 */
int playerInit(Player *player) {

    player->score = 2;
    player->roundsPlayed = 0;

    player->name = (char *) calloc(PLAYERNAME_SIZE, sizeof(char));


#ifdef DEBUGMODE

    snprintf(player->name, PLAYERNAME_SIZE, "Pl4Y3r-number-%d", playerCount + 1);

#else
    printf("Player %d, please enter your name: ", playerCount+1);

    parseString(player->name, PLAYERNAME_SIZE);

    int c = 0;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {} //flush extra characters from stdin

    if (strlen(player->name) < 3) {
        printf("Your name was too short, so we baptized you 'Player%d'\n"
                       "We hope you're happy about that.\n\n", playerCount + 1);
        snprintf(player->name, 8, "Player%i", playerCount + 1);
    }
#endif

    playerCount++;

    return 1;
}