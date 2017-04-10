//
// Created by clement on 16/03/17.
//

#include <stdlib.h>
#include <stdbool.h>
#include "AI_Logic.h"

const int MCrange = 100;
extern Board gameBoard;
Board simulationBoard;

Player player, opponent;

void playRound(Position *pPosition, Player *pPlayer, Player *opponent, PositionList *flippable) {

    int flippedCount = 0;

    for (int i = 0; i < flippable->count; ++i) {
        Position direction = flippable->list[i];
        flippedCount += flipDirection(pPosition, &direction, pPlayer) - 2;
    }

    pPlayer->score += flippedCount + 1; // include the current move
    opponent->score -= flippedCount;

}

Player simulate(Position *startingMove, Player player, Player opponent) {
    Player winner, *currentPlayer, *tmp;

    //getInput(startingMove);
    PositionList flippable = getFlippableTokens(startingMove, &player);
    playRound(startingMove, &player, &opponent, &flippable);

    currentPlayer = &opponent;
    tmp = &player;

    int moveCount;
    bool gameOver = false;
    Position *validMoves = (Position *) calloc(32, sizeof(Position));

    while (!gameOver) {

        moveCount = getValidMoves(currentPlayer, validMoves);
        Position chosenMove = validMoves[rand() % moveCount];
        flippable = getFlippableTokens(&chosenMove, currentPlayer);
        playRound(&chosenMove, currentPlayer, &opponent, &flippable);

        if (getValidMoves(&opponent, validMoves) > 0) {
            currentPlayer = (currentPlayer == &player) ? &opponent : &player;
            continue;
        }

        if (getValidMoves(currentPlayer, validMoves) > 0) {
            continue;
        }

        winner = (player.score > opponent.score) ? player : opponent;
        gameOver = true;
    }

    free(validMoves);
    validMoves = NULL;

    return winner;
}

int getMoveRank(Position *pos, Player *player, Player *opponent, Board simBoard) {

    int moveRank = 0;
    for (int j = 0; j < MCrange; j++) {
        Player winner = simulate(pos, *player, *opponent);
        if (player->token == winner.token)
            moveRank++;
    }

    return moveRank;
}


Position findBestMove(Player *pPlayer, Player *pOpponent, Position *validMoves, int validMovesCount) {
    player = *pPlayer;
    opponent = *pOpponent;
    int bestMoveRank = 0;
    Position bestMove;
    simulationBoard = gameBoard;

    for (int i = 0; i < validMovesCount; i++) {
        Position currentMove = validMoves[i];
        int currentMoveRank = getMoveRank(&currentMove, &player, &opponent, simulationBoard);

        if (currentMoveRank > bestMoveRank) {
            bestMove = currentMove;
            bestMoveRank = currentMoveRank;
        }
    }
    /**
     *
 * - get all valid moves for Player
 * - for each move
 * -    for 100 times
 * -        simulate entire game randomly with this move
 * -            ...(describe how)
 * -        if Player wins, increase move weight
 * - return move with max weight
 *
 */
    return bestMove;
}
