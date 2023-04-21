#ifndef __GUARDIAN__
#define __GUARDIAN__

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <MLV/MLV_all.h>
#include <stdlib.h>

#include "Terrain.h"
#include "Player.h"
#include "Hitbox.h"

#define UPPER_SPEED_MODIFIER 0.8
#define LOWER_SPEED_MODIFIER 0.3
#define DETECTION_DISTANCE 4
#define PANIC_DETECTION_DISTANCE 6
#define PANIC_MODE_DURATION 30

/**
 * @brief struct containing the information about the guardian like its position, speed,
 * detection distance, etc...
 */
typedef struct {
    int x, y;
    int detection_distance;
    float speed;
    MLV_Image *sprite;
    Direction dir;
    MLV_Color color;
} Guardian;


/**
 * A function that takes the address of an uninitialized array of Guardians and allocates the necessary space for it
 * and initializes it.
 * @param guardians The address of an array of Guardians.
 * @param nb_guardians The numbers of guardians to be generated.
 * @param terrain The terrain on which they will be generated.
 */
void generate_guardians(Guardian **guardians, int nb_guardians, Terrain const *terrain);

/**
 * A function that takes in an array of guardians and determines if one of the guardians has detected the player.
 * @param terrain The address of the game terrain.
 * @param guardians The array of guardians in question.
 * @param nb_guardians The number of guardians in the array.
 * @param x The x coordinate of the entity to test for detection.
 * @param y The y coordinate of the entity to test for detection.
 * @return 1 if the player has been detected, 0 otherwise.
 */
int detected_by_guardians(Terrain const *terrain, Guardian const *guardians, int nb_guardians, int x, int y);


/**
 * A function to free the memory used to allocate the guardians.
 * @param guardians List of guardians.
 */
void free_guardians(Guardian *guardians);

struct timespec;

/**
 * This function puts all guardians in panic mode. It also sets start_time (the time when the panic mode has been
 * activated) to now (to the current time), to indicate that it has been 0 seconds since panic mode has been activated.
 * @param guardians The list of guardians in question.
 * @param nb_guardians The number of guardians in the list.
 * @param panic_start_time The time in which the panic mode has been activated.
 * @param panic Integer that tells in panic mode is on or off.
 */
void guardians_enter_panic_mode(Guardian *guardians, int nb_guardians, struct timespec *panic_start_time, int *panic);

/**
 * A function turns off panic mode for all guardians.
 * @param guardians The list of guardians.
 * @param nb_guardians The number of guardians in the list.
 * @param panic Integer that tells in panic mode is on or off.
 */
void guardians_exit_panic_mode(Guardian *guardians, int nb_guardians, int *panic);

/**
 * A function that moves all guardians on the terrain.
 * @param guardians The list of guardians.
 * @param nb_guardians The number of guardians in the list.
 */
void move_guardians(Guardian *guardian, int nb_guardians);

/**
 * A function that checks if any of the guardians has collided with a wall, and if so takes the proper action.
 * @param terrain Pointer to a terrain structure.
 * @param guardians The list of guardians.
 * @param nb_guardians The number of guardians in the list.
 * @param limit_x : x coordinate the entity must not cross.
 * @param limit_y : y coordinate the entity must not cross.
 * @param panic Integer that tells in panic mode is on or off.
 */
void check_guardians_collision_and_movement(Terrain *terrain, Guardian *guardians, int nb_guardians, int *limit_x, int
*limit_y, int panic);


#endif
