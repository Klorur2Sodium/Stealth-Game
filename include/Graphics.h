#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <MLV/MLV_all.h>

#include "Terrain.h"
#include "Guardian.h"
#include "Player.h"
#include "Ranking.h"

#define HEIGHT (TILE_SIZE * NB_TILES_Y) /* Number of pixel vertically */
#define WIDTH (TILE_SIZE * NB_TILES_X) /* Number of pixel horizontally */

/**
 * @brief Stores all the information about a button.
 * 
 */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    char *text;
    MLV_Color text_color;
    MLV_Color border_color;
    MLV_Color background_color;
} Button;

/**
 * @brief Displays the given terrain on the window.
 *
 * @param terrain The terrain you want to display on the window.
 */
void displayTerrain(const Terrain *terrain, MLV_Image **tiles_sprites);


/**
 * @brief Displays the given guardian on the window.
 * @param guardian The guardian to be displayed.
 * @param nb_guardians The number of guardians.
 */
void display_guardians(const Guardian *guardians, int nb_guardians);

/**
 * @brief Displays the given player on the window
 *
 * @param player: The player you want to display on the window
 */
void displayPlayer(const Player *player);


/**
 * @brief A function that displays the entirety of the game.
 * @param terrain The address of the terrain.
 * @param guardians The guardians array.
 * @param nb_guardians The number of guardians on the terrain.
 * @param player The address of the player.
 */
void draw_window(const Terrain *terrain, MLV_Image **tiles_sprites, const Guardian *guardians,
                 int nb_guardians, const Player *player);

/**
 * @brief A function that displays the main menu.
 * @param buttons A list containing all the clicking buttons.
 * @param length Length of the list buttons.
 * @param title_font Font of the game's title.
 * @param box_font Font of the boxes.
 */
void display_menu(const Button *buttons, int length, MLV_Font *title_font, MLV_Font *box_font);

/**
 * @brief 
 * 
 * @param x x position of the button on the screen.
 * @param y y position of the button on the screen.
 * @param width width of the button.
 * @param height height of the button.
 * @param text text written on the button.
 * @return Button The struct containing all the info needed about a button.
 */
Button init_button(int x, int y, int width, int height, const char *text);

/**
 * @brief Changes the colors of a button if the mouse hovers over it.
 * 
 * @param button A list containing all the clicking buttons.
 * @param length Length of the list buttons.
 * @param mouse_x x position of the mouse on the screen.
 * @param mouse_y y position of the mouse on the screen.
 */
void mouse_hover_button(Button *button, int length, int mouse_x, int mouse_y);

/**
 * @brief Returns an int that indicates on what button the player clicked.
 * 
 * @param buttons A list containing all the clicking buttons.
 * @param length Length of the list buttons.
 * @param mouse_x x position of the mouse on the screen.
 * @param mouse_y y position of the mouse on the screen.
 * @return int indicates on what button the player clicked
 */
int player_click(Button *buttons, int length, int mouse_x, int mouse_y);

/**
 * @brief A function that displays the leaderboard.
 * 
 * @param scores List of the players' scores.
 * @param nb_scores Length oh the list scores.
 */
void display_leaderboard(Score *scores, int nb_scores);

/**
 * @brief A function that displays "No highscores yet" on the screen.
 * 
 */
void display_empty_leaderboard();

/**
 * @brief Displays the victory message on the screen.
 * 
 */
void winning_screen();

/**
 * @brief Displays the defeat message on the screen.
 * 
 */
void losing_screen();

#endif