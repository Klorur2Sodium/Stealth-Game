#include "../include/Relic.h"


void generate_relics(Terrain *terrain) {
    int i, j, count;
    for (count = 0; count < RELICS_COUNT; count++) {
        get_valid_placement(&i, &j, terrain);
        terrain->tiles[i][j] = RELIC;
    }
}


int detected_missing_relics(Terrain *terrain, Guardian *guardian, int nb_guardians) {
    int i, j;
    for (i = 0; i < NB_TILES_Y; i++) {
        for (j = 0; j < NB_TILES_X; j++) {
            if (terrain->tiles[i][j] == MISSING_RELIC &&
                detected_by_guardians(terrain, guardian, nb_guardians, j * TILE_SIZE + TILE_SIZE / 2,
                                      i * TILE_SIZE + TILE_SIZE / 2)) {
                terrain->tiles[i][j] = EMPTY;
                return 1;
            }

        }
    }
    return 0;
}