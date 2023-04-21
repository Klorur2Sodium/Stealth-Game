#include "../include/Hitbox.h"
#include <math.h>


/**
 * @brief Checks if there is a possible collision with the tile that are upright and upleft. If
 * there is one stores the limit in limit_y.
 * 
 * @param terrain Pointer to a terrain structure.
 * @param ind_x x index of the tile on which the entity is.
 * @param ind_y y index of the tile on which the entity is.
 * @param x x position of the entity
 * @param limit_y y limit the entity must not pass. 
 * @param sensitivity Detection distance
 */
static void check_up(const Terrain *terrain, int ind_x, int ind_y, int x, int *limit_y, int sensitivity) {
    if (terrain->tiles[ind_y - (1 + sensitivity)][ind_x - (1 + sensitivity)] == WALL &&
        x < ind_x * TILE_SIZE + TILE_SIZE / 2) {
        *limit_y = ind_y * TILE_SIZE + sqrt(pow(TILE_SIZE / 2, 2) - pow(x - ind_x * TILE_SIZE, 2));
    }
    if (terrain->tiles[ind_y - (1 + sensitivity)][ind_x + (1 + sensitivity)] == WALL &&
        x > (ind_x * TILE_SIZE) + (TILE_SIZE / 2)) {
        *limit_y = ind_y * TILE_SIZE + sqrt(pow(TILE_SIZE / 2, 2) - pow((ind_x + 1) * TILE_SIZE - x, 2));
    }
}

/**
 * @brief Checks if there is a possible collision with the tile that are downright and downleft. If
 * there is one stores the limit in limit_y.
 * 
 * @param terrain Pointer to a terrain structure.
 * @param ind_x x index of the tile on which the entity is.
 * @param ind_y y index of the tile on which the entity is.
 * @param x x position of the entity
 * @param limit_y y limit the entity must not pass. 
 * @param sensitivity Detection distance
 */
static void check_down(const Terrain *terrain, int ind_x, int ind_y, int x, int *limit_y, int sensitivity) {
    if (terrain->tiles[ind_y + (1 + sensitivity)][ind_x - (1 + sensitivity)] == WALL &&
        x < ind_x * TILE_SIZE + TILE_SIZE / 2) {
        *limit_y = (ind_y + 1) * TILE_SIZE - sqrt(pow(TILE_SIZE / 2, 2) - pow(x - ind_x * TILE_SIZE, 2));
    }
    if (terrain->tiles[ind_y + (1 + sensitivity)][ind_x + (1 + sensitivity)] == WALL &&
        x > (ind_x * TILE_SIZE) + (TILE_SIZE / 2)) {
        *limit_y = (ind_y + 1) * TILE_SIZE - sqrt(pow(TILE_SIZE / 2, 2) - pow((ind_x + 1) * TILE_SIZE - x, 2));
    }
}

/**
 * @brief Checks if there is a possible collision with the tile that are upleft and downleft. If
 * there is one stores the limit in limit_y.
 * 
 * @param terrain Pointer to a terrain structure.
 * @param ind_x x index of the tile on which the entity is.
 * @param ind_y y index of the tile on which the entity is.
 * @param y y position of the entity
 * @param limit_x x limit the entity must not pass. 
 * @param sensitivity Detection distance
 */
static void check_left(const Terrain *terrain, int ind_x, int ind_y, int y, int *limit_x, int sensitivity) {
    if (terrain->tiles[ind_y - (1 + sensitivity)][ind_x - (1 + sensitivity)] == WALL &&
        y < ind_y * TILE_SIZE + TILE_SIZE / 2) {
        *limit_x = ind_x * TILE_SIZE + sqrt(pow(TILE_SIZE / 2, 2) - pow(y - (ind_y * TILE_SIZE), 2));
    }
    if (terrain->tiles[ind_y + (1 + sensitivity)][ind_x - (1 + sensitivity)] == WALL &&
        y > (ind_y * TILE_SIZE) + (TILE_SIZE / 2)) {
        *limit_x = ind_x * TILE_SIZE + sqrt(pow(TILE_SIZE / 2, 2) - pow((ind_y + 1) * TILE_SIZE - y, 2));
    }
}

/**
 * @brief Checks if there is a possible collision with the tile that are upright and downright. If
 * there is one stores the limit in limit_y.
 * 
 * @param terrain Pointer to a terrain structure.
 * @param ind_x x index of the tile on which the entity is.
 * @param ind_y y index of the tile on which the entity is.
 * @param y y position of the entity.
 * @param limit_x x limit the entity must not pass. 
 * @param sensitivity Detection distance.
 */
static void check_right(const Terrain *terrain, int ind_x, int ind_y, int y, int *limit_x, int sensitivity) {
    if (terrain->tiles[ind_y - (1 + sensitivity)][ind_x + (1 + sensitivity)] == WALL &&
        y < ind_y * TILE_SIZE + TILE_SIZE / 2) {
        *limit_x = (ind_x + 1) * TILE_SIZE - sqrt(pow(TILE_SIZE / 2, 2) - pow(y - (ind_y * TILE_SIZE), 2));
    }
    if (terrain->tiles[ind_y + (1 + sensitivity)][ind_x + (1 + sensitivity)] == WALL &&
        y > (ind_y * TILE_SIZE) + (TILE_SIZE / 2)) {
        *limit_x = (ind_x + 1) * TILE_SIZE - sqrt(pow(TILE_SIZE / 2, 2) - pow((ind_y + 1) * TILE_SIZE - y, 2));
    }
}

/**
 * @brief Checks if there is a tile in front of the entity and sets a limit if there is a tile.
 *
 * @param terrain : the struct containing the 2d array representing the terrain.
 * @param ind_x : x index of the tile in which the entity is.
 * @param ind_y : y index of the tile in which the entity is.
 * @param dir : direction of the entity.
 * @param limit_x : x coordinate the entity must not cross.
 * @param limit_y : y coordinate the entity must not cross.
 * @param sensitivity : The higher the sensitivity the further the entity must be from a wall before detecting a
 * collision, 0 : right next to the wall, 1 : one tile away.
 */
static void
check_tile_ahead(const Terrain *terrain, int ind_x, int ind_y, int dir, int *limit_x, int *limit_y, int sensitivity) {
    int offset[2];
    offset[0] = -(1 + sensitivity);
    offset[1] = 1 + sensitivity;

    if ((dir == 0 || dir == 1) && terrain->tiles[ind_y + offset[dir]][ind_x] == WALL) {
        *limit_y = ind_y * TILE_SIZE + TILE_SIZE / 2;
    } else if ((dir == 2 || dir == 3) && terrain->tiles[ind_y][ind_x - offset[dir - 2]] == WALL) {
        *limit_x = ind_x * TILE_SIZE + TILE_SIZE / 2;
    }
}

/**
 * @brief Resets the limits to 0.
 * 
 * @param limit_x : x coordinate the entity must not cross
 * @param limit_y : y coordinate the entity must not cross
 */
static void reset_limits(int *limit_x, int *limit_y) {
    *limit_x = 0;
    *limit_y = 0;
}


void check_next_collision(const Terrain *terrain, int x, int y, int dir, int *limit_x, int *limit_y, int sensitivity) {
    /* Coordinates of the tile in which the entity is */
    int ind_x, ind_y;

    reset_limits(limit_x, limit_y);
    get_tile_index_from_coords(x, y, &ind_x, &ind_y);

    if (sensitivity) {
        check_next_collision(terrain, x, y, dir, limit_x, limit_y, 0);
        /* If a collision is detected, the function stops */
        if (!(*limit_x == 0 && *limit_y == 0) &&
            !(*limit_x == NB_TILES_X * TILE_SIZE && *limit_y == NB_TILES_Y * TILE_SIZE)) {
            return;
        }
        reset_limits(limit_x, limit_y);
    }

    switch (dir) {
        case 0:
            check_up(terrain, ind_x, ind_y, x, limit_y, sensitivity);
            break;
        case 1:
            check_down(terrain, ind_x, ind_y, x, limit_y, sensitivity);
            break;
        case 2:
            check_right(terrain, ind_x, ind_y, y, limit_x, sensitivity);
            break;
        case 3:
            check_left(terrain, ind_x, ind_y, y, limit_x, sensitivity);
            break;
        default:
            break;
    }
    check_tile_ahead(terrain, ind_x, ind_y, dir, limit_x, limit_y, sensitivity);

    *limit_x = (*limit_x) ? *limit_x : (dir == 1 || dir == 2) ? NB_TILES_X * TILE_SIZE : 0;
    *limit_y = (*limit_y) ? *limit_y : (dir == 1 || dir == 2) ? NB_TILES_Y * TILE_SIZE : 0;
}


int check_entity_move(int *x, int *y, int dir, int limit_x, int limit_y) {
    int direction[5] = {0, 1, 1, 0, 2};
    switch (direction[dir]) {
        case 0:
            if (*x <= limit_x) {
                *x = limit_x;
                return 1;
            }
            if (*y <= limit_y) {
                *y = limit_y;
                return 1;
            }
            break;
        case 1:
            if (*x >= limit_x) {
                *x = limit_x;
                return 1;
            }
            if (*y >= limit_y) {
                *y = limit_y;
                return 1;
            }
        default:
            return 0;
    }
    return 0;
}