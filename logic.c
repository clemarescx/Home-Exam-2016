//
// Created by clement on 07/12/16.
//

#include "board.h"
#include "player.h"
#include "logic.h"


extern Board gameBoard;

// Functions

Position updatePosition(Position *inputPos, Position *unitVector, short distance);

int traverse(Position *currentPosition, Position *direction, Player *player,
             int(*handlePosition)(Position *pos, Player *_player));

/// used by traverse()
int fieldIsPlayerToken(Position *pos, Player *player);

int flipField(Position *pos, Player *player);
///


/**
 * Fills *positions with all valid moves available for player;
 * Return the amount of positions added this way;
 */
int getValidMoves(Player *player, Position *possibleMoves) {
    int validMovesCount = 0;
    Position currentPosition;

    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j) {
            currentPosition = {.x = i, .y = j};
            if (getField(&gameBoard, &currentPosition) == EMPTY) {
                PositionList flippable = getFlippableTokens(&currentPosition, player);
                if (flippable.count > 0)
                    possibleMoves[validMovesCount++] = currentPosition;
            }
        }

    return validMovesCount;
}

/*
 * Flips all tokens in a direction
 */
int flipDirection(Position *currentPosition, Position *direction, Player *player) {
    gameBoard.fields[currentPosition->x][currentPosition->y] = player->token;

    int score = traverse(currentPosition, direction, player, &flipField);

    return score;
}

/*
 * Iterates and calls a function on each field in a given direction
 * until an empty field or the board's limits are met;
 */
int
traverse(Position *currentPosition, Position *direction, Player *player, int(*handlePosition)(Position *, Player *)) {

    Position unitVector = {
            .x = direction->x - currentPosition->x,
            .y = direction->y - currentPosition->y
    };

    short distance = 1;

    Position newPos = updatePosition(currentPosition, &unitVector, distance++);

    while (!outOfBounds(newPos) && (getField(&gameBoard, &newPos) != EMPTY)) {
        if ((*handlePosition)(&newPos, player)) {
            return distance;
        }
        newPos = updatePosition(currentPosition, &unitVector, distance++);
    }
    return 0;
}


// checks if the given position is player.token
int fieldIsPlayerToken(Position *pos, Player *player) {
    return (getField(&gameBoard, pos) == player->token);
}

// flips a token
int flipField(Position *pos, Player *player) {
    int isPlayerToken = fieldIsPlayerToken(pos, player);

    if (!isPlayerToken) {
        gameBoard.fields[pos->x][pos->y] = player->token;
    }
    return isPlayerToken;
}


/**
 * return a position according to a unit vector + distance from inputPos
 */
Position updatePosition(Position *inputPos, Position *unitVector, short distance) {
    Position newPos = {
            .x = inputPos->x + (unitVector->x * distance),
            .y = inputPos->y + (unitVector->y * distance)
    };

    return newPos;
}


PositionList getAdjacentTokens(Position *pos) {
    PositionList adjacent;
    adjacent.count = 0;

    for (int i = pos->x - 1; i <= pos->x + 1; ++i)
        for (int j = pos->y - 1; j <= pos->y + 1; ++j) {
            Position neighbourPos = {
                    .x = (short) i,
                    .y = (short) j
            };
            if (outOfBounds(neighbourPos) || poscmp(pos, &neighbourPos))    //skip if outside the board or is current position
                continue;

            if (getField(&gameBoard, &neighbourPos) != EMPTY)
                adjacent.list[adjacent.count++] = neighbourPos;
        }
    return adjacent;
}


/**
 * Return a list of valid neighbours
 */
PositionList getFlippableTokens(Position *inputPos, Player *player) {

    PositionList flippableDirections;
    flippableDirections.count = 0;

    PositionList neighbours = getAdjacentTokens(inputPos);

    while (neighbours.count--) {
        Position currentNeighbour = neighbours.list[neighbours.count];
        if (getField(&gameBoard, &currentNeighbour) != player->token) {
            int score = traverse(inputPos, &currentNeighbour, player, &fieldIsPlayerToken);
            if (score > 0)
                flippableDirections.list[flippableDirections.count++] = currentNeighbour;
        }
    }

    return flippableDirections;
}

