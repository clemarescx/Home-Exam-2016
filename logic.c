//
// Created by clement on 07/12/16.
//

#include "debug.h"

#include "board.h"
#include "player.h"
#include "logic.h"


#define poscmp(pos1, pos2)       ((pos1.x == pos2.x) && (pos1.y == pos2.y))
#define vector(start, direction) { .x = direction.x - start.x, .y = direction.y - start.y }
#define outOfBounds(pos)        isOutOfBounds(pos.x, pos.y)
#define getField(pos)           getField(gameBoard, pos.x, pos.y)


extern Board gameBoard;


Position updatePosition(Position currentPos, Position vector, short distance);

Neighbours findValidNeighbours(Position pos, Player player);

int traverse(Position cPos, Position direction, Player player, int(*handlePosition)(Position pos, Player _player));

int fieldIsPlayerToken(Position pos, Player player);


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

int fieldIsPlayerToken(Position pos, Player player) {
    return (getField(pos) == player.token);
}

int flipField(Position pos, Player player) {
    int isPlayerToken = fieldIsPlayerToken(pos, player);

    if (!isPlayerToken) {
        gameBoard.fields[pos.x][pos.y] = player.token;
    }
    return isPlayerToken;
}

int flipDirection(Position cPos, Position direction, Player player) {
    gameBoard.fields[cPos.x][cPos.y] = player.token;

    int score = traverse(cPos, direction, player, &flipField);

    return score;
}

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

/**
 * Move in a direction according to a unit vector
 * @param cPos
 * @param vector
 * @param dist
 * @return
 */
Position updatePosition(Position cPos, Position vector, short dist) {
    Position newPos = {
            .x = cPos.x + (vector.x * dist),
            .y = cPos.y + (vector.y * dist)
    };
    return newPos;
}

/**
 * Return a list of valid neighbours
 * @param pos
 * @return
 */
Neighbours findValidNeighbours(Position pos, Player player) {
    Neighbours neighbours;
    neighbours.count = 0;
    short *nCount = &(neighbours.count);

    DEBUG("Valid neighbours of {%d, %d}:\n", pos.x, pos.y);

    for (int i = pos.x - 1; i <= pos.x + 1; ++i) {
        for (int j = pos.y - 1; j <= pos.y + 1; ++j) {
            Position newPos = {.x = (short) i, .y = (short) j};
            if (outOfBounds(newPos) || poscmp(pos, newPos)) {   //skip if outside the board or is current position
                DEBUG("neighbour - not valid%s", "\n");
                continue;
            }
            if (getField(newPos) != EMPTY && getField(newPos) != player.token) {
                if (traverse(pos, newPos, player, &fieldIsPlayerToken)) {
                    neighbours.list[(*nCount)++] = newPos;
                    DEBUG(" - {%d, %d}: %c ", newPos.x, newPos.y, (char) gameBoard.fields[newPos.x][newPos.y]);
                    DEBUG("neighbour - valid%s", "");
                    DEBUG("%s", "\n");

                }
            }
        }
    }
    return neighbours;
}