//
// Created by clement on 07/12/16.
//

#include <stdbool.h>
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


void (*handlePos)(Position position);

Position traverse(Position currentPos, Position vector, short distance);

void addNeighbour(Neighbours adj, Position pos);


bool isValidMove(Player player, Position pos) {
    Neighbours neighbours = findNeighbours(pos);
    if (neighbours.count == 0) return false; //can't place token without neighbours

    DEBUG("Start of neighbours print for pos {%d, %d}\n", pos.x, pos.y);
    ARRAYPRINT(neighbours)
    DEBUG("End of neighbours print, player %c\n", player.token);

    for (int i = 0; i < neighbours.count; ++i) {
        Position direction = neighbours.list[i];
        if (getField(direction) == player.token) continue; // only interested in potentially flippable tokens

        Position unitVector = vector(pos, direction);
        short distance = 1;
        direction = traverse(pos, unitVector, distance++);

        while (!outOfBounds(direction) && (getField(direction) != EMPTY)) {
            if (getField(direction) == player.token) return true;
            direction = traverse(pos, unitVector, distance++);
        }
    }
    return false;
}

/**
 * Move in a direction according to a unit vector
 * @param cPos
 * @param vector
 * @param dist
 * @return
 */
Position traverse(Position cPos, Position vector, short dist) {
    Position newPos = {
            .x = cPos.x + (vector.x * dist),
            .y = cPos.y + (vector.y * dist)
    };
    return newPos;
}

/**
 * Return a list of neighbours
 * @param pos
 * @return
 */
Neighbours findNeighbours(Position pos) {
    Neighbours neighbours;
    neighbours.count = 0;
    short *nCount = &(neighbours.count);

    DEBUG("Neighbours of {%d, %d}:\n", pos.x, pos.y);

    for (int i = pos.x - 1; i <= pos.x + 1; ++i) {
        for (int j = pos.y - 1; j <= pos.y + 1; ++j) {
            Position newPos = {.x = (short) i, .y = (short) j};
            DEBUG(" - {%d, %d}: ", newPos.x, newPos.y);
            if (outOfBounds(newPos) || poscmp(pos, newPos)) {
                DEBUG("neighbour - not valid%s", "\n");
                continue;
            }
            if (getField(newPos) != EMPTY) {
                DEBUG("neighbour - valid%s", "");
                neighbours.list[(*nCount)++] = newPos;
            }
            DEBUG("%s", "\n");
        }
    }
    return neighbours;
}