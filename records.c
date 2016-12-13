//
// Created by clement on 13/12/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "records.h"

static Records records = {.roundCount = 0};

void addMoveToLog(Player player, Position move);


void addMoveToLog(Player player, Position move) {
    Move currentMove = {
            .move = move,
            .player = player
    };
    records.gameLog[records.roundCount++] = currentMove;
}

void printLogToFile() {

    char *fileName = (char *) calloc(255, sizeof(char));
    int gameNumber = 1;
    sprintf(fileName, "%s_%d.log", "gamelog", gameNumber);

    while (access(fileName, F_OK) != -1) {
        sprintf(fileName, "%s_%d.log", "gamelog", ++gameNumber);
    }

    FILE *logfile;
    if ((logfile = fopen(fileName, "w")) == NULL) {
        fprintf(stderr, "could not write to '%s'\n", fileName);
        exit(EXIT_FAILURE);
    }
    free(fileName);

    fprintf(logfile, "#####################################\n");
    fprintf(logfile, "########     GAME LOG     ###########\n");
    fprintf(logfile, "#####################################\n\n");
    for (int i = 0; i < records.roundCount; i++) {
        Move gameRound = records.gameLog[i];
        char *playerName = gameRound.player.name;
        char moveX = (char) ('A' + gameRound.move.x);
        int moveY = gameRound.move.y + 1;

        fprintf(logfile, "%i. %s - %c-%i\n", i + 1, playerName, moveX, moveY);
    }

    fclose(logfile);
}


void printGameLog() {

    printf("#####################################\n");
    printf("########     GAME LOG     ###########\n");
    printf("#####################################\n\n");

    for (int i = 0; i < records.roundCount; i++) {
        Move gameRound = records.gameLog[i];
        char *playerName = gameRound.player.name;
        char moveX = (char) ('A' + gameRound.move.x);
        int moveY = gameRound.move.y + 1;

        printf("%i. %s - %c-%i\n", i + 1, playerName, moveX, moveY);
    }

}