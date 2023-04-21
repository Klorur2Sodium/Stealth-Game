#ifndef __HITBOX__
#define __HITBOX__

#include <stdio.h>
#include "Terrain.h"


/**
 * @brief Returns via the parameters limit_x and limit_y, the limit the entity must not cross
 * in its current tile, if it can go everywhere in its tile, the limit is set to the coordinate of
 * the map's border it is facing.
 *
 * @param terrain : the struct containing the 2d array representing the terrain
 * @param x : x coordinate of the entity
 * @param y : y coordinate of the entity
 * @param dir : direction of the entity
 * @param limit_x : x coordinate the entity must not cross
 * @param limit_y : y coordinate the entity must not cross
 * @param sensitivity : The higher the sensitivity the further the entity must be from a wall before detecting a
 * collision, 0 : right next to the wall, 1 : one tile away.
 */
void check_next_collision(const Terrain *terrain, int x, int y, int dir, int *limit_x, int *limit_y, int sensitivity);


/**
 * @brief If the entity crossed the limit, sets its position to the limit, do nothing otherwise.
 *
 * @param x : x coordinate of the entity
 * @param y : y coordinate of the entity
 * @param dir : direction of the entity
 * @param limit_x : x coordinate the entity must not cross
 * @param limit_y : y coordinate the entity must not cross
 * @return 1 in case of a collision, 0 otherwise.
 */
int check_entity_move(int *x, int *y, int dir, int limit_x, int limit_y);

#endif