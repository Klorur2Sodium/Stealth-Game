#include "../include/Player.h"

void generate_player(Player *player) {
    player->x = 1 * TILE_SIZE + TILE_SIZE / 2 + 1;
    player->y = 1 * TILE_SIZE + TILE_SIZE / 2 + 1;
    player->speed_cap = 0.9F;
    player->mana = -1;
    player->consumed_mana = 0;
    player->sprite = MLV_load_image("./res/sprites/player.png");
}


/**
 * @brief Stops the player by setting his speed to 0 and his direction to NO_DIRECTION.
 * 
 * @param player : a pointer to the player
 */
static void stop_player(Player *player) {
    player->dir = NO_DIRECTION;
    player->speed = 0;
}

/**
 * @brief Changes the direction of the player if the given new_direction is not the current
 * direction of the player. If the direction changes, the speed is reset to a tenth of the 
 * max speed.
 * 
 * @param player : a pointer to the player
 * @param new_direction : new direction of the player
 */
static void change_player_direction(Player *player, Direction new_direction) {
    if (player->dir != new_direction) {
        player->dir = new_direction;
        player->speed = 0.1F * MAX_SPEED;
    }
}

/**
 * @brief Updates the player's direction using the given new_direction
 * 
 * @param player : a pointer to the player
 * @param new_direction : new direction of the player
 */
static void update_player_direction(Player *player, Direction new_direction) {
    switch (new_direction) {
        case NO_DIRECTION:
            stop_player(player);

        default:
            change_player_direction(player, new_direction);
    }
}


/**
 * @brief Increases the speed of the player if he is not at his speed cap.
 * 
 * @param player : a pointer to the players
 */
static void increase_player_speed(Player *player) {
    if (player->dir != NO_DIRECTION && player->speed <= player->speed_cap * MAX_SPEED) {
        player->speed += 0.03F * MAX_SPEED;
    }
}

/**
 * @brief Changes the position of the player on the grid thanks to his speed and his
 * current direction.
 * 
 * @param player : a pointer to the players
 */
static void move_player(Player *player) {
    float offset_x[5] = {0.0F, 0.0F, 1.0F, -1.0F, 0.0F};
    float offset_y[5] = {-1.0F, 1.0F, 0.0F, 0.0F, 0.0F};

    player->x += (int) round((double) offset_x[player->dir] * player->speed);
    player->y += (int) round((double) offset_y[player->dir] * player->speed);
}

/**
 * @brief Transforms an input of the player into a direction.
 * 
 * @param userInput : the key the player pressed.
 * @return int : the corresponding direction
 */
static int parse_move_player(MLV_Keyboard_button userInput) {
    switch (userInput) {
        /* cases for Z, Q, S, D */
        case MLV_KEYBOARD_z:
            return UP;
        case MLV_KEYBOARD_q:
            return LEFT;
        case MLV_KEYBOARD_s:
            return DOWN;
        case MLV_KEYBOARD_d:
            return RIGHT;

            /* cases for Up, Down, Left or Right arrow */
        case MLV_KEYBOARD_UP:
            return UP;
        case MLV_KEYBOARD_DOWN:
            return DOWN;
        case MLV_KEYBOARD_LEFT:
            return LEFT;
        case MLV_KEYBOARD_RIGHT:
            return RIGHT;

        default:
            return NO_DIRECTION;
    }
}

void pick_up_mana(Terrain *terrain, Player *player) {
    if (terrain->tiles[player->y / TILE_SIZE][player->x / TILE_SIZE] == MANA) {
        terrain->tiles[player->y / TILE_SIZE][player->x / TILE_SIZE] = EMPTY;
        player->mana += 1;
    }
}

void pick_up_relic(Terrain *terrain, Player const *player, MLV_Sound *pickup) {
    if (terrain->tiles[player->y / TILE_SIZE][player->x / TILE_SIZE] == RELIC) {
        terrain->tiles[player->y / TILE_SIZE][player->x / TILE_SIZE] = MISSING_RELIC;
        if (pickup != NULL) MLV_play_sound(pickup, 1.0F);
    }
}

/**
 * @brief Returns the used mana on the board.
 * 
 * @param terrain Represents the terrain.
 * @param player Represents the player.
 */
static void use_return_one_mana(Terrain *terrain, Player *player) {
    int missing_mana_count, target_placement, i, j, current_placement;
    if (player->mana == 0) return;
    missing_mana_count = player->mana + 1;
    current_placement = 1;
    target_placement = range(1, missing_mana_count);
    for (i = 0; i < NB_TILES_Y; i++) {
        for (j = 0; j < NB_TILES_X; j++) {
            if (terrain->tiles[i][j] == EMPTY) {
                if (target_placement == current_placement) {
                    terrain->tiles[i][j] = MANA;
                    player->mana -= 1;
                    player->consumed_mana += 1;
                    return;
                }
                current_placement++;
            }
        }
    }
}

/**
 * @brief Calls nb_mana times the function use_return_one_mana().
 * 
 * @param terrain Represents the terrain.
 * @param player Represents the player.
 * @param nb_mana The number of the mana spent.
 */
static void use_return_n_mana(Terrain *terrain, Player *player, int nb_mana) {
    int i;
    for (i = 0; i < nb_mana; i++) {
        use_return_one_mana(terrain, player);
    }
}

/**
 * @brief Updates the player's speed cap according to the use of the speed spell.
 * 
 * @param player Represents the player.
 * @param terrain Represents the terrain.
 */
static void speed_spell_handler(Player *player, Terrain *terrain) {
    if (!MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) && player->mana >= SPEED_SPELL_COST) {
        player->speed_cap = 1.2F;
        use_return_n_mana(terrain, player, 2);
    } else {
        player->speed_cap = 0.9F;
        player->speed = (player->speed > player->speed_cap * MAX_SPEED) ? player->speed_cap * MAX_SPEED : player->speed;
    }
}


int invisibility_spell_handler(Player *player, Terrain *terrain, MLV_Keyboard_button userInput) {
    if (!MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) && player->mana >= INVISIBILITY_SPELL_COST) {
        if (userInput == MLV_KEYBOARD_SPACE)
            player->sprite = MLV_load_image("./res/sprites/player_invisible.png");
        use_return_one_mana(terrain, player);
        return 0;
    } else if ((userInput == MLV_KEYBOARD_SPACE && MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)) ||
               player->mana < INVISIBILITY_SPELL_COST)
        player->sprite = MLV_load_image("./res/sprites/player.png");

    return 1;
}

void player_movements_handler(Terrain *terrain, Player *player, MLV_Keyboard_button userInput) {
    Direction new_direction;
    new_direction = parse_move_player(userInput);
    if (new_direction == NO_DIRECTION)
        new_direction = player->dir;
    else if (new_direction != NO_DIRECTION && MLV_get_keyboard_state(userInput))
        new_direction = NO_DIRECTION;

    update_player_direction(player, new_direction);
    speed_spell_handler(player, terrain);
    if (player->dir != NO_DIRECTION) {
        increase_player_speed(player);
        move_player(player);
    }
}

/**
 * @brief Checks if there are some relics left.
 * 
 * @param terrain Represents the terrain.
 * @return int 0 if there are, 1 if not.
 */
static int no_relics_left(Terrain const *terrain) {
    int i, j;
    for (i = 1; i < NB_TILES_Y - 1; i++) {
        for (j = 1; j < NB_TILES_X - 1; j++) {
            if (terrain->tiles[i][j] == RELIC) return 0;
        }
    }
    return 1;
}

int player_won(Player const *player, Terrain const *terrain) {
    if (player->x <= 2.5 * TILE_SIZE && player->y <= 2.5 * TILE_SIZE && no_relics_left(terrain))
        return 1;
    return 0;
}

