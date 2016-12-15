//
// Created by clement on 07/12/16.
//
#include <stdio.h>

#include "debug.h"

#include "board.h"
#include "player.h"
#include "logic.h"


#define poscmp(pos1, pos2)       ((pos1.x == pos2.x) && (pos1.y == pos2.y))
#define vector(start, direction) { .x = direction.x - start.x, .y = direction.y - start.y }
#define outOfBounds(pos)        isOutOfBounds(pos.x, pos.y)
#define getField(pos)           getField(gameBoard, pos.x, pos.y)


extern Board gameBoard;

// Functions
Neighbours findValidNeighbours(Position inputPos, Player player);

Position updatePosition(Position inputPos, Position vector, short distance);

int traverse(Position cPos, Position direction, Player player, int(*handlePosition)(Position pos, Player _player));

/// used by traverse()
int fieldIsPlayerToken(Position pos, Player player);
int flipField(Position pos, Player player);
///


/**
 * Fills *positions with all valid moves available for player;
 * Return the amount of positions added this way;
 */
int getValidMoves(Player player, Position *positions) {
    int validCount = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Position cPos = {i, j};
            if (getField(cPos) == EMPTY) {
                Neighbours currentNeighbours = findValidNeighbours(cPos, player);
                if (currentNeighbours.count > 0) positions[validCount++] = cPos;
            }
        }
    }
    return validCount;
}

/*
 * Flips all tokens in a direction
 */
int flipDirection(Position cPos, Position direction, Player player) {
    gameBoard.fields[cPos.x][cPos.y] = player.token;

    int score = traverse(cPos, direction, player, &flipField);

    return score;
}

/*
 * Iterates and calls a function on each field in a given direction
 * until an empty field or the board's limits are met;
 */
int traverse(Position cPos, Position direction, Player player, int(*handlePosition)(Position, Player)) {
    Position unitVector = vector(cPos, direction);
    short distance = 1;
    Position newPos = updatePosition(cPos, unitVector, distance++);

    while (!outOfBounds(newPos) && (getField(newPos) != EMPTY)) {
        if ((*handlePosition)(newPos, player)) {
            return distance;
        }
        newPos = updatePosition(cPos, unitVector, distance++);
    }
    return 0;
}


// checks if the given position is player.token
int fieldIsPlayerToken(Position pos, Player player) {
    return (getField(pos) == player.token);
}

// flips a token
int flipField(Position pos, Player player) {
    int isPlayerToken = fieldIsPlayerToken(pos, player);

    if (!isPlayerToken) {
        gameBoard.fields[pos.x][pos.y] = player.token;
    }
    return isPlayerToken;
}


/**
 * return a position according to a unit vector + distance from inputPos
 */
Position updatePosition(Position inputPos, Position vector, short distance) {
    Position newPos = {
            .x = inputPos.x + (vector.x * distance),
            .y = inputPos.y + (vector.y * distance)
    };
    return newPos;
}


/**
 * Return a list of valid neighbours
 */
Neighbours findValidNeighbours(Position inputPos, Player player) {

    Neighbours neighbours;
    neighbours.count = 0;
    short *nCount = &(neighbours.count);

    for (int i = inputPos.x - 1; i <= inputPos.x + 1; ++i) {
        for (int j = inputPos.y - 1; j <= inputPos.y + 1; ++j) {
            Position newPos = {.x = (short) i, .y = (short) j};
            if (outOfBounds(newPos) || poscmp(inputPos, newPos)) {   //skip if outside the board or is current position
                continue;
            }
            if (getField(newPos) != EMPTY && getField(newPos) != player.token) {
                if (traverse(inputPos, newPos, player, &fieldIsPlayerToken)) {
                    neighbours.list[(*nCount)++] = newPos;
                }
            }
        }
    }
    return neighbours;
}