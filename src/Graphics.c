#include "../include/Graphics.h"

void displayTerrain(const Terrain *terrain, MLV_Image **tiles_sprites) {
    int i, j;
    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_COLOR_BLACK);
    for (i = 0; i < NB_TILES_Y; i++) {
        for (j = 0; j < NB_TILES_X; j++) {
            MLV_draw_image(tiles_sprites[terrain->tiles[i][j]], j * TILE_SIZE, i * TILE_SIZE);
        }
    }
}

/**
 * A function to display one guardian on the window.
 * @param guardian The address of a guardian structure.
 */
static void display_guardian(const Guardian *guardian) {
    MLV_draw_image(guardian->sprite, guardian->x - TILE_SIZE / 2, guardian->y - TILE_SIZE / 2);
    MLV_draw_circle(guardian->x, guardian->y, guardian->detection_distance * TILE_SIZE, guardian->color);
}

void display_guardians(const Guardian *guardians, int nb_guardians) {
    int i;
    for (i = 0; i < nb_guardians; i++) {
        display_guardian(&guardians[i]);
    }
}

void displayPlayer(const Player *player) {
    MLV_draw_image(player->sprite, player->x - TILE_SIZE / 2, player->y - TILE_SIZE / 2);
}

void display_mana(const Player *player) {
    MLV_draw_text(17, 1, "Mana : %d", MLV_COLOR_WHITE, player->mana);
}

void draw_window(const Terrain *terrain, MLV_Image **tiles_sprites, const Guardian *guardians,
                 int nb_guardians, const Player *player) {
    displayTerrain(terrain, tiles_sprites);
    display_guardians(guardians, nb_guardians);
    displayPlayer(player);
    display_mana(player);
}

Button init_button(int x, int y, int width, int height, const char *text) {
    Button new_button;

    new_button.x = x;
    new_button.y = y;
    new_button.width = width;
    new_button.height = height;
    new_button.text = (char *) malloc(sizeof(char) * strlen(text));
    strcpy(new_button.text, text);
    new_button.text_color = MLV_COLOR_WHITE;
    new_button.border_color = MLV_COLOR_WHITE;
    new_button.background_color = MLV_rgba(43, 43, 43, 255);

    return new_button;
}


void mouse_hover_button(Button *buttons, int length, int mouse_x, int mouse_y) {
    int i;
    for (i = 0; i < length; i++) {
        if (mouse_x >= buttons[i].x && mouse_x < buttons[i].x + buttons[i].width &&
            mouse_y >= buttons[i].y && mouse_y < buttons[i].y + buttons[i].height) {
            buttons[i].border_color = MLV_rgba(43, 43, 43, 255);
            buttons[i].text_color = MLV_rgba(43, 43, 43, 255);
            buttons[i].background_color = MLV_COLOR_WHITE;
        } else {
            buttons[i].border_color = MLV_COLOR_WHITE;
            buttons[i].text_color = MLV_COLOR_WHITE;
            buttons[i].background_color = MLV_rgba(43, 43, 43, 255);
        }
    }
}

/**
 * @brief Display the given button on the screen.
 * 
 * @param button Pointer to a button structure.
 * @param box_font Font of the text in the box.
 */
static void display_button(const Button *button, MLV_Font *box_font) {
    MLV_draw_text_box_with_font(button->x, button->y, button->width, button->height, button->text, box_font, 0,
                                button->border_color, button->text_color, button->background_color, MLV_TEXT_CENTER,
                                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

/**
 * @brief Displays the length buttons on the screen stored in the list buttons.
 * 
 * @param buttons List containing the buttons that the user wants to display.
 * @param length Length of the list buttons.
 * @param box_font Font of the text in the box.
 */
static void display_buttons(const Button *buttons, int length, MLV_Font *box_font) {
    int i;
    for (i = 0; i < length; i++) {
        display_button(&buttons[i], box_font);
    }
}

void display_menu(const Button *buttons, int length, MLV_Font *title_font, MLV_Font *box_font) {
    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_rgba(43, 43, 43, 255));
    MLV_draw_text_with_font(76, 110, "Sorcerer's Sanctuary", title_font, MLV_COLOR_WHITE);
    display_buttons(buttons, length, box_font);
}

int player_click(Button *buttons, int length, int mouse_x, int mouse_y) {
    int i;
    for (i = 0; i < length; i++) {
        if (mouse_x >= buttons[i].x && mouse_x < buttons[i].x + buttons[i].width &&
            mouse_y >= buttons[i].y && mouse_y < buttons[i].y + buttons[i].height &&
            !MLV_get_mouse_button_state(MLV_BUTTON_LEFT)) {
            return i + 1;
        }
    }
    return -1;
}

void winning_screen() {
    MLV_Image *totem_1, *totem_2;
    MLV_Font * font;
    totem_1 = MLV_load_image("./res/sprites/winning_screen_totem_1.png");
    totem_2 = MLV_load_image("./res/sprites/winning_screen_totem_2.png");
    font = MLV_load_font("./res/font/upheavtt.ttf", 54);

    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_rgba(43, 43, 43, 255));
    MLV_draw_text_with_font(341, (NB_TILES_Y * TILE_SIZE) / 2, "YOU WON !!!", font, MLV_COLOR_WHITE);
    MLV_draw_image(totem_1, 100, (NB_TILES_Y * TILE_SIZE) / 2 + 100);
    MLV_draw_image(totem_1, 100, (NB_TILES_Y * TILE_SIZE) / 2 - 100);
    MLV_draw_image(totem_2, (NB_TILES_X * TILE_SIZE) - 170, (NB_TILES_Y * TILE_SIZE) / 2 + 100);
    MLV_draw_image(totem_2, (NB_TILES_X * TILE_SIZE) - 170, (NB_TILES_Y * TILE_SIZE) / 2 - 100);
    MLV_update_window();

    free(totem_1);
    free(totem_2);
    free(font);
}

void losing_screen() {
    MLV_Image *golem;
    MLV_Font * font;
    golem = MLV_load_image("./res/sprites/losing_screen_golem.png");
    font = MLV_load_font("./res/font/upheavtt.ttf", 54);

    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_rgba(43, 43, 43, 255));
    MLV_draw_text_with_font(190, (NB_TILES_Y * TILE_SIZE) / 2, "YOU'VE BEEN CAUGHT :(", font, MLV_COLOR_WHITE);
    MLV_draw_image(golem, 100, (NB_TILES_Y * TILE_SIZE) / 2 + 100);
    MLV_draw_image(golem, 100, (NB_TILES_Y * TILE_SIZE) / 2 - 100);
    MLV_draw_image(golem, (NB_TILES_X * TILE_SIZE) - 170, (NB_TILES_Y * TILE_SIZE) / 2 + 100);
    MLV_draw_image(golem, (NB_TILES_X * TILE_SIZE) - 170, (NB_TILES_Y * TILE_SIZE) / 2 - 100);
    MLV_update_window();

    free(golem);
    free(font);
}

/**
 * @brief Displays the list scores on the screen, starting at the given y_position y position.
 * 
 * @param scores List containing akll the scores.
 * @param nb_scores Length of the list scores.
 * @param y_position Starting y position.
 * @param font Font.
 */
static void display_scores(Score *scores, int nb_scores, int y_position, MLV_Font *font) {
    int i = 0;

    MLV_draw_text_with_font(100, y_position + (i * 30), "%s", font, MLV_COLOR_WHITE, "rank");
    MLV_draw_text_with_font(190, y_position + (i * 30), "%s", font, MLV_COLOR_WHITE, "name");
    MLV_draw_text_with_font(400, y_position + (i * 30), "%s", font, MLV_COLOR_WHITE, "time");
    MLV_draw_text_with_font(500, y_position + (i * 30), "%s", font, MLV_COLOR_WHITE, "consumed mana");
    for (i = 1; i < nb_scores + 1; i++) {
        MLV_draw_text_with_font(100, y_position + (i * 23), "%d. ", font, MLV_COLOR_WHITE, i);
        MLV_draw_text_with_font(190, y_position + (i * 23), "%s ", font, MLV_COLOR_WHITE, scores[i - 1].name);
        MLV_draw_text_with_font(400, y_position + (i * 23), "%d ", font, MLV_COLOR_WHITE, scores[i - 1].time);
        MLV_draw_text_with_font(500, y_position + (i * 23), "%d ", font, MLV_COLOR_WHITE, scores[i - 1].used_mana);
    }
}

void display_leaderboard(Score *scores, int nb_scores) {
    MLV_Font *big_font, *smol_font;
    big_font = MLV_load_font("./res/font/upheavtt.ttf", 60);
    smol_font = MLV_load_font("./res/font/upheavtt.ttf", 23);

    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_rgba(43, 43, 43, 255));
    MLV_draw_text_with_font(50, 10, "LEADERBOARD :", big_font, MLV_COLOR_WHITE);

    /* Ranking by mana */
    MLV_draw_text_with_font(75, 80, "Ranking by mana :", smol_font, MLV_COLOR_WHITE);
    qsort(scores, nb_scores, sizeof(Score), compare_mana_scores);
    display_scores(scores, nb_scores, 105, smol_font);

    /* Ranking by score */
    MLV_draw_text_with_font(75, 375, "Ranking by time :", smol_font, MLV_COLOR_WHITE);
    qsort(scores, nb_scores, sizeof(Score), compare_time_scores);
    display_scores(scores, nb_scores, 400, smol_font);

    MLV_draw_text(300, NB_TILES_Y * TILE_SIZE - 30, "Click anywhere on the screen to return to the menu",
                  MLV_COLOR_WHITE);

    free(big_font);
    free(smol_font);
}

void display_empty_leaderboard() {
    MLV_Font *big_font;
    big_font = MLV_load_font("./res/font/upheavtt.ttf", 90);

    MLV_draw_filled_rectangle(0, 0, TILE_SIZE * NB_TILES_X, TILE_SIZE * NB_TILES_Y, MLV_rgba(43, 43, 43, 255));
    MLV_draw_text_with_font(48, (TILE_SIZE * NB_TILES_Y) / 2 - 45, "NO HIGHSCORES YET", big_font, MLV_COLOR_WHITE);
    MLV_draw_text(300, NB_TILES_Y * TILE_SIZE - 30, "Click anywhere on the screen to return to the menu",
                  MLV_COLOR_WHITE);

    free(big_font);
}