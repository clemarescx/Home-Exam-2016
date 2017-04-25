//
// Created by clement on 16/03/17.
//

#include <stdlib.h>
#include <stdbool.h>
#include "AI_Logic.h"

const int MCrange = 100;
extern Board gBoard;
Board simulationBoard;

Player player, opponent;

int getMoveRank(Position *pos, Player *player, Player *opponent);

Player simulate(Position *startingMove, Player player, Player opponent);

void playRound(Position *pPosition, Player *pPlayer, Player *opponent, PositionList *flippable);


Position findBestMove(Player player, Player opponent, Position *validMoves, int validMovesCount) {
    int bestMoveRank = 0;
    Position bestMove = {.x = -1, .y = -1};

    for (int i = 0; i < validMovesCount; i++) {
        Position currentMove = validMoves[i];
        int currentMoveRank = getMoveRank(&currentMove, &player, &opponent);

        if (currentMoveRank > bestMoveRank) {
            bestMove = currentMove;
            bestMoveRank = currentMoveRank;
        }
    }

    return bestMove;
}

///////
int getMoveRank(Position *pos, Player *player, Player *opponent) {

    int moveRank = 0;
    for (int i = 0; i < MCrange; i++) {
        Player winner = simulate(pos, *player, *opponent);
        if (player->token == winner.token)
            moveRank++;
    }

    return moveRank;
}

//////
Player simulate(Position *startingMove, Player player, Player opponent) {
    Player winner,
            *currentPlayer,
            *previousPlayer;

    simulationBoard = gBoard;

    //getInput(startingMove);
    PositionList flippable = getFlippableTokens(startingMove, &player, &simulationBoard);
    playRound(startingMove, &player, &opponent, &flippable);

    previousPlayer = &player;
    currentPlayer = &opponent;

    int moveCount;
    //    bool gameOver = false;
    Position *validMoves;


    while (true) {
        validMoves = (Position *) calloc(32, sizeof(Position));

        moveCount = getValidMoves(currentPlayer, validMoves, &simulationBoard);

        if (moveCount == 0) {
            if (currentPlayer == previousPlayer) {
                winner = (player.score > opponent.score) ? player : opponent;
                break;
            }
            currentPlayer = (currentPlayer == &player) ? &opponent : &player;
            continue;
        }

        int randIndex = rand() % moveCount;
        Position chosenMove = validMoves[randIndex];
        flippable = getFlippableTokens(&chosenMove, currentPlayer, &simulationBoard);
        playRound(&chosenMove, currentPlayer, &opponent, &flippable);

        /*if (getValidMoves(&opponent, validMoves, &simulationBoard) > 0) {
            currentPlayer = (currentPlayer == &player) ? &opponent : &player;
            continue;
        }*/

        previousPlayer = currentPlayer;
        currentPlayer = (currentPlayer == &player) ? &opponent : &player;

        /*if (getValidMoves(currentPlayer, validMoves, &simulationBoard) > 0) {
            continue;
        }*/
    }

    free(validMoves);
    validMoves = NULL;

    return winner;
}

//////
void playRound(Position *pPosition, Player *pPlayer, Player *opponent, PositionList *flippable) {

    int flippedCount = 0;

    for (int i = 0; i < flippable->count; ++i) {
        Position direction = flippable->list[i];
        flippedCount += flipDirection(pPosition, &direction, pPlayer, &simulationBoard);
    }

    pPlayer->score += flippedCount; // include the current move
    opponent->score -= flippedCount;
}


