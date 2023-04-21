#include "../include/Terrain.h"
#include <stdlib.h>

/**
 * @brief Fill the array tiles in terrain with the Tile tile between
 * the index [i_init, j_init] and [max_iter_i, max_iter_j]
 * 
 */
static void fill_array_Terrain(Terrain *terrain, int i_init, int j_init, int max_iter_i, int max_iter_j, Tile tile) {
    int i, j;
    for (i = i_init; i < max_iter_i; i++) {
        for (j = j_init; j < max_iter_j; j++) {
            terrain->tiles[i][j] = tile;
        }
    }
}

/**
 * Initializes the terrain by adding outer walls and empty walls inside.
 * @param terrain A pointer to a terrain structure.
 */
static void init_terrain(Terrain *terrain) {
    fill_array_Terrain(terrain, 0, 0, NB_TILES_Y, NB_TILES_X, WALL);
    fill_array_Terrain(terrain, 1, 1, NB_TILES_Y - 1, NB_TILES_X - 1, MANA);
    fill_array_Terrain(terrain, 1, 1, 3, 3, ENTRY);
}


int range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/**
 * A function to add a horizontal wall to the terrain starting from minX included to maxX included, the function adds
 * a random gap of length 3 on one end of the wall.
 * @param terrain The terrain struct.
 * @param minX The column at which to start building the wall.
 * @param maxX The column at which to finish building the wall.
 * @param y the horizontal line where the wall will be built.
 */
static void addHWall(Terrain *terrain, int minX, int maxX, int y) {
    int beacon, dx;
    beacon = range(0, 1); /* 0 or 1 */
    for (dx = minX + (beacon * 3); dx <= maxX - (!beacon * 3); dx++) {
        terrain->tiles[y][dx] = WALL;
    }
}

/**
 * A function to add a vertical wall to the terrain starting from minY included to maxY included, the function adds
 * a random gap of length 3 on one end of the wall.
 * @param terrain The terrain struct.
 * @param minY The line at which to start building the wall.
 * @param maxY The line at which to finish building the wall.
 * @param x the vertical column where the wall will be built.
 */
static void addVWall(Terrain *terrain, int minY, int maxY, int x) {
    int beacon, dy;
    beacon = range(0, 1); /* 0 or 1 */
    for (dy = minY + (beacon * 3); dy <= maxY - (!beacon * 3); dy++) {
        terrain->tiles[dy][x] = WALL;
    }
}

/**
 * A function that generates the maze-like inner walls of a given terrain.
 * @param terrain A pointer to a terrain struct.
 * @param starting_x The starting horizontal coordinate of the chamber/subsection of the terrain.
 * @param starting_y The starting vertical coordinate of the chamber/subsection of the terrain.
 * @param maxX The ending horizontal coordinate of the chamber/subsection of the terrain.
 * @param maxY The ending vertical coordinate of the chamber/subsection of the terrain.
 */
static void generate_inner_walls(Terrain *terrain, int starting_x, int starting_y, int maxX, int maxY) {
    int wallX, wallY;

    if (maxX - starting_x > maxY - starting_y) {
        if (maxX - starting_x < 2 * MIN_SIDE + 1) return;
        if (maxX - starting_x < 4 * MIN_SIDE)
            if (range(0, 1)) return; /* 0 or 1 */
        wallX = range(starting_x + MIN_SIDE, maxX - MIN_SIDE);  /* We divide the maxX */
        addVWall(terrain, starting_y, maxY, wallX);
        /* generating the inner walls in the two separate chambers */
        generate_inner_walls(terrain, starting_x, starting_y, wallX - 1, maxY);
        generate_inner_walls(terrain, wallX + 1, starting_y, maxX, maxY);
    } else {
        if (maxY - starting_y < 2 * MIN_SIDE + 1) return;
        if (maxY - starting_y < 4 * MIN_SIDE)
            if (range(0, 1)) return; /* 0 or 1 */
        wallY = range(starting_y + MIN_SIDE, maxY - MIN_SIDE);  /* We divide the maxY */
        addHWall(terrain, starting_x, maxX, wallY);
        /* generating the inner walls in the two separate chambers */
        generate_inner_walls(terrain, starting_x, starting_y, maxX, wallY - 1);
        generate_inner_walls(terrain, starting_x, wallY + 1, maxX, maxY);
    }
}


/**
 * A function to fully generate a terrain.
 * @param terrain A pointer to a terrain struct.
 */
void generate_terrain(Terrain *terrain) {
    init_terrain(terrain);
    generate_inner_walls(terrain, 1, 1, NB_TILES_X - 2, NB_TILES_Y - 2);
}

/**
 * A function that checks if the coordinate i, j in the chambre have a wall within 3 spaces on both sides of the
 * given orientation
 * @param i Line.
 * @param j Column.
 * @param orientation A char, v (for vertical) or h (for horizontal).
 * @param terrain An address to a terrain structure.
 * @return Boolean, 1 or 0.
 */
static int in_VH_entrance(int i, int j, char orientation, Terrain const *terrain) {
    int a, b, res;
    res = 0;
    /* Check both sides */
    for (b = 0; b < 2; b++) {
        /* Check the three adjacent spaces */
        for (a = 1; a <= 3; a++) {
            /* (b * -2 + 1) for 1 or -1 (up or down / left and right) */
            if ((orientation == 'v' && terrain->tiles[i + (a * (b * -2 + 1))][j] == WALL) ||
                (orientation == 'h' && terrain->tiles[i][j + (a * (b * -2 + 1))] == WALL)) {
                res++;
                /* Stop checking */
                break;
            }
        }
    }
    return res == 2 ? 1 : 0;
}

/**
 * A function that checks if the coordinate i, j in the chamber is positioned on a chamber entrance.
 * @param i Line.
 * @param j Column.
 * @param terrain An address to a terrain structure.
 * @return Boolean, 1 or 0.
 */
static int in_chambre_entrance(int i, int j, Terrain const *terrain) {
    return in_VH_entrance(i, j, 'v', terrain) || in_VH_entrance(i, j, 'h', terrain);
}


void get_valid_placement(int *i, int *j, Terrain const *terrain) {
    int beacon;
    do {
        beacon = range(0, 1); /* 0 or 1 */
        *i = range(20 * beacon, NB_TILES_Y - 1);
        *j = range(20 * !beacon, NB_TILES_X - 1);
    } while (terrain->tiles[*i][*j] != MANA || in_chambre_entrance(*i, *j, terrain));
}

void get_tile_index_from_coords(int coord_x, int coord_y, int *ind_x, int *ind_y) {
    *ind_x = coord_x / TILE_SIZE;
    *ind_y = coord_y / TILE_SIZE;
}

double round(double x) {
    if (x < 0.0)
        return (int) (x - 0.5);
    else
        return (int) (x + 0.5);
}