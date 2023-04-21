#ifndef __TERRAIN__
#define __TERRAIN__

#define NB_TILES_X 60  /* Number of tiles horizontally */
#define NB_TILES_Y 45 /* Number of tiles vertically */
#define TILE_SIZE 16 /* Size of a tile */
#define ENTITY_DIAMETER TILE_SIZE
#define MIN_SIDE 9 /* A constant used by the terrain generation algorithm */
#define MAX_SPEED 3.0F

/** 
 * @brief Tile represents all five states in which a
 * tile of the terrain can be
 */
typedef enum {
    WALL,
    EMPTY,
    MANA,
    RELIC,
    MISSING_RELIC,
    ENTRY
} Tile;

/**
 * @brief Terrain contains the array that represents
 * all the tiles on it
 */
typedef struct {
    Tile tiles[NB_TILES_Y][NB_TILES_X];
} Terrain;


/**
 * A function that generates the terrain in its entirety.
 * @param terrain A pointer to a Terrain structure
 */
void generate_terrain(Terrain *terrain);


/**
 * A function to get a randomly generated number between min and max, both included.
 * @param min Minimum, included
 * @param max Maximum, included
 * @return
 */
int range(int min, int max);


/**
 * A function that gets a valid placement (an empty space that isn't in a chambre entrance) on the terrain where a
 * guardian or a relic can be placed.
 * @param i The valid line integer address.
 * @param j The valid column integer address.
 * @param terrain The terrain upon which the guardians shall be placed.
 */
void get_valid_placement(int *i, int *j, Terrain const *terrain);

/**
 * @brief Get the tile index from the entity's coordinates
 * 
 * @param coord_x : x coordinate of the entity 
 * @param coord_y : y coordinate of the entity 
 * @param ind_x : x index of the tile in which the entity is
 * @param ind_y : y index of the tile in which the entity is
 */
void get_tile_index_from_coords(int coord_x, int coord_y, int *ind_x, int *ind_y);


/**
 * @brief Round a floating point number to the nearest integer.
 *
 * This function rounds a floating point number to the nearest integer using the
 * following algorithm: if the fractional part of the number is greater than or
 * equal to 0.5, the number is rounded up to the nearest integer; otherwise, it
 * is rounded down.
 *
 * @param x The floating point number to be rounded.
 * @return The nearest integer to the input number.
 */
double round(double number);

#endif
