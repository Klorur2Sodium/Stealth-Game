#ifndef __PLAYER__
#define __PLAYER__

#define INVISIBILITY_SPELL_COST 1
#define SPEED_SPELL_COST 2

#include <MLV/MLV_all.h>
#include "Terrain.h"

/**
* Represents the 4 directions in which
* the player can go
*/
typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NO_DIRECTION
} Direction;


/**
 * @brief Represents a Player with its position, velocity, mana, direction etc...
 */
typedef struct {
    int x;
    int y;
    int mana;
    float speed;
    float speed_cap;
    Direction dir;
    int consumed_mana;
    MLV_Image *sprite;
} Player;

/**
 * @brief Initializes a player at the coordinates (23, 23).
 *
 * @return Player
 */
void generate_player(Player *player);

/**
 * @brief Handles the movements of the player
 *
 * @param terrain: The terrain.
 * @param player: The player you want to move.
 * @param userInput: The key that the user pressed.
 */
void player_movements_handler(Terrain *terrain, Player *player, MLV_Keyboard_button userInput);

/**
 * A function that determines if player has won.
 * @param player A pointer to the player in question.
 * @param terrain A pointer to the terrain structure.
 * @return 1 or 0.
 */
int player_won(Player const *player, Terrain const *terrain);

/**
 * A function that allows the player to pick up mana if present.
 * @param terrain Pointer to a terrain structure.
 * @param player Pointer to the player in question.
 */
void pick_up_mana(Terrain *terrain, Player *player);

/**
 * A function that allows the player to pick up a relic if present.
 * @param terrain Pointer to a terrain structure.
 * @param player Pointer to the player in question.
 */
void pick_up_relic(Terrain *terrain, Player const *player, MLV_Sound *pickup);

/**
 * @brief Indicates if the player is currently under invisible mode and updates his mana and sprite
 * consequently.
 * 
 * @param player Represents the player.
 * @param terrain Represents the terrain.
 * @param userInput Keyboard button which the user used.
 * @return int 0 if under invisible mode, 1 if yes.
 */
int invisibility_spell_handler(Player *player, Terrain *terrain, MLV_Keyboard_button userInput);

#endif