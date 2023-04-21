#ifndef __RELIC__
#define __RELIC__

#include "Terrain.h"
#include "Guardian.h"

#define RELICS_COUNT 3

/**
 * A function to generate relics on the terrain.
 * @param terrain A pointer to a terrain structure.
 */
void generate_relics(Terrain *terrain);

/**
 * A simple, non-optimal function that tests if one of the guardians has detected a missing relic.
 * @param terrain Pointer to a terrain struct.
 * @param guardian Pointer to a list of guardians.
 * @param nb_guardians Number of guardians in the list.
 * @return 1 or 0.
 */
int detected_missing_relics(Terrain *terrain, Guardian *guardian, int nb_guardians);

#endif
