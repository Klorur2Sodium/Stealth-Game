#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <math.h>
#include "../include/Guardian.h"


/**
 * A function that dynamically allocates an array of nb_guardians and returns it.
 * @param nb_guardians The number of guardians to be generated.
 * @return The allocated array.
 */
static Guardian *allocate_guardians(int nb_guardians) {
    Guardian *guardians;
    guardians = (Guardian *) malloc(sizeof(Guardian) * nb_guardians);
    if (guardians == NULL) exit(EXIT_FAILURE);
    return guardians;
}

/**
 * A function to free the list of guardians.
 * @param guardians The list of guardians.
 */
void free_guardians(Guardian *guardians) {
    free(guardians);
    guardians = NULL;
}

/**
 * @brief Get a random speed for the guardian.
 * 
 * @return float a random speed.
 */
static float get_random_speed() {
    return ((float) range(LOWER_SPEED_MODIFIER * 10, UPPER_SPEED_MODIFIER * 10) * MAX_SPEED) / 10;
}


void generate_guardians(Guardian **guardians, int nb_guardians, Terrain const *terrain) {
    int k, i, j;
    *guardians = allocate_guardians(nb_guardians);
    for (k = 0; k < nb_guardians; k++) {
        get_valid_placement(&i, &j, terrain);
        (*guardians)[k].x = j * TILE_SIZE + TILE_SIZE / 2;
        (*guardians)[k].y = i * TILE_SIZE + TILE_SIZE / 2;
        (*guardians)[k].detection_distance = DETECTION_DISTANCE;
        /* Macros are just for me, I hate having "Naked" values */
        (*guardians)[k].speed = get_random_speed();
        (*guardians)[k].sprite = MLV_load_image("./res/sprites/guardian.png");
        (*guardians)[k].dir = range(0, 3);
        (*guardians)[k].color = MLV_COLOR_BLUE;

    }
}

/**
 * @brief Returns the smallest number between a and b.
 * 
 * @param a A number.
 * @param b A number.
 * @return int a if a < b, b if not.
 */
static int min(int a, int b) {
    return a < b ? a : b;
}

/**
 * @brief Returns the biggest number between a and b.
 * 
 * @param a A number.
 * @param b A number.
 * @return int a if a > b, b if not.
 */
static int max(int a, int b) {
    return a > b ? a : b;
}

/**
 * A function that takes a guardian and a player and determines if the player has been detected by the guardian.
 * @param terrain The terrain upon which the guardian and the plyer exist.
 * @param guardian The address of the guardian struct in question.
 * @param x The x coordinate of the entity to test for detection.
 * @param y The y coordinate of the entity to test for detection.
 * @return 1 if the player has been detected, 0 otherwise.
 */
static int detected_by_guardian(Terrain const *terrain, Guardian const *guardian, int x, int y) {
    int a, b;
    double pa, pb, ya, xb, x0, y0, x1, y1;
    x0 = (double) guardian->x / TILE_SIZE;
    y0 = (double) guardian->y / TILE_SIZE;
    x1 = (double) x / TILE_SIZE;
    y1 = (double) y / TILE_SIZE;

    /* Safe zone */
    if (x < 2 * TILE_SIZE + TILE_SIZE / 2 && y < 2 * TILE_SIZE + TILE_SIZE / 2) return 0;

    /* Checking if the player is out of the guardian's detection range */
    if (sqrt(pow(guardian->x - x, 2) + pow(guardian->y - y, 2)) >=
        (guardian->detection_distance * TILE_SIZE + ENTITY_DIAMETER / 2.0 - 2))
        return 0;

    /* Checking vertical lines intersections */
    for (a = min(floor(x0), floor(x1)); a <= max(floor(x0), floor(x1)); a++) {
        pa = (a - x0) / (x1 - x0);
        if (!(0 <= pa && pa <= 1)) continue;
        ya = pa * y1 + (1 - pa) * y0;
        if (terrain->tiles[(int) floor(ya)][a - 1] == WALL || terrain->tiles[(int) floor(ya)][a] == WALL) return 0;
    }

    /* Checking horizontal lines intersections */
    for (b = min(floor(y0), floor(y1)); b <= max(floor(y0), floor(y1)); b++) {
        pb = (b - y0) / (y1 - y0);
        if (!(0 <= pb && pb <= 1)) continue;
        xb = pb * x1 + (1 - pb) * x0;
        if (terrain->tiles[b - 1][(int) floor(xb)] == WALL || terrain->tiles[b][(int) floor(xb)] == WALL) return 0;
    }
    return 1;
}

/**
 * @brief Changes the direction and speed of a guardian. Called when it encounters a wall.
 * 
 * @param guardian Pointer to a guardian structure.
 * @param panic Number indication if the guardian is in panic mode.
 */
static void ricochet(Guardian *guardian, int panic) {
    int new_dir;
    new_dir = range(0, 2); /* 0 to 2 */
    new_dir = new_dir == guardian->dir ? 3 : new_dir;
    guardian->dir = new_dir;
    if (!panic) guardian->speed = get_random_speed();
}

/**
 * @brief Has a 1/50 chance of changing the direction and speed of a guardian.
 * 
 * @param guardian Pointer to a guardian structure.
 * @param panic Number indication if the guardian is in panic mode.
 */
static void maybe_update_guardian_dir_speed(Guardian *guardian, int panic) {
    /* 1 to 49 */
    if (range(0, 49)) return;
    /* 0 */
    guardian->dir = range(0, 3); /* 0 to 3 */
    if (!panic) guardian->speed = get_random_speed();
}

/**
 * @brief Changes the position of a guardian based on his speed.
 * 
 * @param guardian Pointer to a guardian structure.
 */
static void move_guardian(Guardian *guardian) {
    float offset_x[] = {0.0F, 0.0F, 1.0F, -1.0F};
    float offset_y[] = {-1.0F, 1.0F, 0.0F, 0.0F};

    guardian->x += (int) round((double) (offset_x[guardian->dir] * guardian->speed));
    guardian->y += (int) round((double) offset_y[guardian->dir] * guardian->speed);
}

void move_guardians(Guardian *guardian, int nb_guardians) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        move_guardian(&guardian[i]);
    }
}

/**
 * A function that checks if any of the guardians has collided with a wall, and if so takes the proper action.
 * @param terrain Pointer to a terrain structure.
 * @param guardians Pointer to a guardian.
 * @param limit_x : x coordinate the entity must not cross.
 * @param limit_y : y coordinate the entity must not cross.
 * @param panic Integer that tells in panic mode is on or off.
 */
static void check_guardian_collision_and_move(Terrain *terrain, Guardian *guardian, int *limit_x, int *limit_y, int
panic) {
    int collision;
    check_next_collision(terrain, guardian->x, guardian->y, guardian->dir, limit_x, limit_y, panic);
    collision = check_entity_move(&(guardian->x), &(guardian->y), guardian->dir, *limit_x, *limit_y);
    if (collision) ricochet(guardian, panic);
    else maybe_update_guardian_dir_speed(guardian, panic);
}


void check_guardians_collision_and_movement(Terrain *terrain, Guardian *guardians, int nb_guardians, int *limit_x, int
*limit_y, int panic) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        check_guardian_collision_and_move(terrain, &guardians[i], limit_x, limit_y, panic);
    }
}

/**
 * @brief Modifies the stats and appearance of a guardian so he is in panic mode.
 * 
 * @param guardian Pointer to a guardian structure.
 */
static void guardian_enter_panic_mode(Guardian *guardian) {
    guardian->speed = MAX_SPEED;
    guardian->detection_distance = PANIC_DETECTION_DISTANCE;
    guardian->sprite = MLV_load_image("./res/sprites/angry-guardian.png");
    guardian->color = MLV_COLOR_RED;
}

void guardians_enter_panic_mode(Guardian *guardians, int nb_guardians, struct timespec *panic_start_time, int *panic) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        guardian_enter_panic_mode(&guardians[i]);
    }

    clock_gettime(CLOCK_REALTIME, panic_start_time);
    *panic = 1;
}

/**
 * @brief Modifies the stats and appearance of a guardian so he is no longer in panic mode.
 * 
 * @param guardian Pointer to a guardian structure.
 */
static void guardian_exit_panic_mode(Guardian *guardian) {
    guardian->speed = get_random_speed();
    guardian->detection_distance = DETECTION_DISTANCE;
    guardian->sprite = MLV_load_image("./res/sprites/guardian.png");
    guardian->color = MLV_COLOR_BLUE;
}

void guardians_exit_panic_mode(Guardian *guardians, int nb_guardians, int *panic) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        guardian_exit_panic_mode(&guardians[i]);
    }
    *panic = 0;
}

int detected_by_guardians(Terrain const *terrain, Guardian const *guardians, int nb_guardians, int x, int y) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        if (detected_by_guardian(terrain, &guardians[i], x, y)) {
            return 1;
        }
    }
    return 0;
}