#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include "../include/Terrain.h"
#include "../include/Player.h"
#include "../include/Hitbox.h"
#include "../include/Guardian.h"
#include "../include/Ranking.h"
#include "../include/Graphics.h"
#include "../include/Relic.h"
#include <stdarg.h>
/* Can later be changed to accept user input */
#define NB_GUARDIANS 5
/* Size of the array sprites */
#define NB_TILES_SPRITES 6

/**
 * @brief Loads the tiles' srites in a list.
 * 
 * @return MLV_Image** List containing the tiles' sprites.
 */
MLV_Image **init_tiles_sprites() {
    MLV_Image **sprites = (MLV_Image **)malloc(NB_TILES_SPRITES * sizeof(MLV_Image *));
    if (!sprites) {
        fprintf(stderr, "Ran out of memory.\n");
        exit(1);
    }
    sprites[0] = MLV_load_image("./res/sprites/wall.jpg");
    sprites[1] = MLV_load_image("./res/sprites/empty.jpg");
    sprites[2] = MLV_load_image("./res/sprites/mana.jpg");
    sprites[3] = MLV_load_image("./res/sprites/relic.jpg");
    sprites[4] = MLV_load_image("./res/sprites/missing-relic.jpg");
    sprites[5] = MLV_load_image("./res/sprites/entry.png");

    return sprites;
}

/**
 * @brief Loads the sounds in their respective variable.
 * 
 * @param you_lose Lose sound.
 * @param enter_panic_mode Panic mode sound.
 * @param game_music General game music.
 * @param pickup Relic pick up sound.
 */
void init_sounds(MLV_Sound **you_lose, MLV_Sound **enter_panic_mode, MLV_Sound **game_music, MLV_Sound **pickup) {
    *you_lose = MLV_load_sound("./res/sound_effects/losing/you_lose.ogg");
    *enter_panic_mode = MLV_load_sound("./res/sound_effects/panic/enter-panic-mode_short.ogg");
    *game_music = MLV_load_sound("./res/sound_effects/background/background2.ogg");
    *pickup = MLV_load_sound("./res/sound_effects/pickup/bowl_pick_up_wood.ogg");
    if (*game_music != NULL) MLV_play_sound(*game_music, 0.5F);
}

/**
 * @brief Free all the given sounds.
 * 
 * @param num_sounds Number of sounds to free.
 * @param ... Sound to free.
 */
void free_sounds(int num_sounds, ...) {
    int i;
    va_list sounds;
    va_start(sounds, num_sounds);
    for (i = 0; i < num_sounds; i++) {
        MLV_free_sound(va_arg(sounds, MLV_Sound*));
    }
    va_end(sounds);
    MLV_free_audio();
}

/**
 * @brief Calls all the function needed to generate the game.
 * 
 * @param terrain Represents the terrain.
 * @param guardians List containing all the guardians.
 * @param player Represents the player.
 */
void generate_game(Terrain *terrain, Guardian **guardians, Player *player) {
    generate_terrain(terrain);
    generate_guardians(guardians, NB_GUARDIANS, terrain);
    generate_relics(terrain);
    generate_player(player);
}

/**
 * @brief Handles all the game events like the player picking some mana up or the guardian detecting
 * a missing relic.
 * 
 * @param elapsed_time Time from the beginning of the panic mode.
 * @param guardians List containing all the guardians.
 * @param current_panic_time Current panic mode time.
 * @param panic_start_time Beginning of the panic mode.
 * @param panic Indicates if the guardians are in panic mode.
 * @param terrain Represents the terrain.
 * @param player Represents the player.
 * @param panic_sound Panic mode sound.
 * @param pickup Relic pick up sound.
 */
void game_events(long elapsed_time, Guardian *guardians, struct timespec *current_panic_time,
                 struct timespec *panic_start_time, int *panic, Terrain *terrain, Player *player,
                 MLV_Sound *panic_sound, MLV_Sound *pickup) {
    pick_up_mana(terrain, player);
    pick_up_relic(terrain, player, pickup);
    clock_gettime(CLOCK_REALTIME, current_panic_time);
    if (detected_missing_relics(terrain, guardians, NB_GUARDIANS)) {
        if (panic_sound != NULL) MLV_play_sound(panic_sound, 0.7F);
        guardians_enter_panic_mode(guardians, NB_GUARDIANS, panic_start_time, panic);
    }
    elapsed_time = (*current_panic_time).tv_sec - (*panic_start_time).tv_sec;
    if ((*panic) && elapsed_time > PANIC_MODE_DURATION)
        guardians_exit_panic_mode(guardians, NB_GUARDIANS, panic);
}

/**
 * @brief Calls all the function needed when the player wins.
 * 
 * @param time_spent Total time played.
 * @param player Structure containing data about the player.
 */
void winning_actions(int time_spent, Player *player) {
    printf("You won! your time is %d seconds, and the amount of mana you used is %d\n", time_spent,
           (*player).consumed_mana);
    MLV_stop_all_sounds();
    winning_screen();
    put_in_scores_file(time_spent, (*player).consumed_mana);
    display_high_scores();
}

/**
 * @brief Calls all the function needed when the player loses.
 * 
 * @param time_spent Total time played.
 * @param player Structure containing data about the player.
 * @param losing_sound Sound played when the player loses.
 */
void losing_actions(int time_spent, Player *player, MLV_Sound *losing_sound) {
    MLV_stop_all_sounds();
    losing_screen();
    if (losing_sound != NULL) MLV_play_sound(losing_sound, 1.0F);
    MLV_wait_seconds(5);
    printf("You lost... your time is %d seconds, and the amount of mana you used is %d\n", time_spent,
           (*player).consumed_mana);
}

/**
 * @brief Calls all the function needed to run the main game and run it.
 * 
 */
void game() {
    MLV_Keyboard_button userInput;
    int time_spent, limit_x, limit_y, panic = 1, caught = 0;
    long frametime, elapsed_time;
    double extra_time;
    Terrain terrain;
    Guardian *guardians;
    Player player;
    struct timespec end_time, new_time, current_panic_time, panic_start_time, game_start_time, game_end_time;
    MLV_Sound *you_lose, *enter_panic_mode, *game_music, *pickup;
    MLV_Image **sprites;
    elapsed_time = 0;

    if (MLV_init_audio()) {
        fprintf(stderr, "L'infrasctructure audio de la librairie MLV ne s'est pas correctement initialisé.\n");
        exit(1);
    }
    MLV_change_number_of_parallel_sounds(5);
    init_sounds(&you_lose, &enter_panic_mode, &game_music, &pickup);

    srand(time(NULL)); /* Needed for every function that needs a random number */

    generate_game(&terrain, &guardians, &player);
    sprites = init_tiles_sprites();
    clock_gettime(CLOCK_REALTIME, &game_start_time);

    /* Main loop over the frames */
    while (!caught && !player_won(&player, &terrain)) {

        /* Display of the current frame, simple function */
        /* This function calls once, and only once, MLV_update_window, for performance purposes */
        draw_window(&terrain, sprites, guardians, NB_GUARDIANS, &player);

        /* Get the time in nanoseconds at the start of the frame */
        clock_gettime(CLOCK_REALTIME, &end_time);

        /* Here we get some keyboard events */
        MLV_get_event(&userInput, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Dealing with the events */
        game_events(elapsed_time, guardians, &current_panic_time, &panic_start_time, &panic, &terrain, &player,
                    enter_panic_mode, pickup);

        /* Move the entities on the grid */
        player_movements_handler(&terrain, &player, userInput);
        move_guardians(guardians, NB_GUARDIANS);

        /* Collision detection and other mechanisms */
        check_next_collision(&terrain, player.x, player.y, player.dir, &limit_x, &limit_y, 0);
        check_entity_move(&(player.x), &(player.y), player.dir, limit_x, limit_y);
        check_guardians_collision_and_movement(&terrain, guardians, NB_GUARDIANS, &limit_x, &limit_y, panic);

        /* Checks if the player has been detected by the guardians */
        if (invisibility_spell_handler(&player, &terrain, userInput))
            caught = detected_by_guardians(&terrain, guardians, NB_GUARDIANS, player.x, player.y);

        if (!caught) MLV_update_window();

        /* Get the time in nanoseconds at the end of the frame */
        clock_gettime(CLOCK_REALTIME, &new_time);

        /* Compute the current time spent for thr current frame */
        frametime = new_time.tv_sec - end_time.tv_sec;
        frametime += (long) ((double) (new_time.tv_sec - end_time.tv_sec) / 1.0E9);

        /* Force the program to spend at least 1/60 second in total */
        extra_time = 1.0 / 60 - (double) frametime;
        if (extra_time > 0) MLV_wait_milliseconds((int) (extra_time * 1000));
    }
    clock_gettime(CLOCK_REALTIME, &game_end_time);
    time_spent = (int) (game_end_time.tv_sec - game_start_time.tv_sec);

    free(sprites);
    free_guardians(guardians);
    if (player_won(&player, &terrain)) {
        winning_actions(time_spent, &player);
    } else {
        losing_actions(time_spent, &player, you_lose);
    }
    free_sounds(4, you_lose, enter_panic_mode, game_music, pickup);
}

/**
 * @brief Handles the display of the leaderboard.
 * 
 */
void leaderboard() {
    Score *scores;
    int nb_scores;
    if (!get_scores_list(&scores, &nb_scores)) display_empty_leaderboard();
    else display_leaderboard(scores, nb_scores);
    MLV_update_window();
    MLV_wait_mouse(NULL, NULL);
}

int main(int argc, char *argv[]) {
    Button buttons[3];
    MLV_Font *default_font, *title_font;
    int mouse_x, mouse_y;
    int quit = 0;

    MLV_create_window("Sorcerer's Sanctuary", "Sorcerer's Sanctuary", WIDTH, HEIGHT);
    title_font = MLV_load_font("./res/font/upheavtt.ttf", 72);
    default_font = MLV_load_font("./res/font/upheavtt.ttf", 54);

    buttons[0] = init_button((TILE_SIZE * NB_TILES_X) / 2 - 85, 300, 170, 50, "PLAY");
    buttons[1] = init_button((TILE_SIZE * NB_TILES_X) / 2 - 185, 375, 370, 50, "LEADERBOARD");
    buttons[2] = init_button((TILE_SIZE * NB_TILES_X) / 2 - 85, 450, 170, 50, "QUIT");

    while (!quit) {
        MLV_get_mouse_position(&mouse_x, &mouse_y);
        mouse_hover_button(buttons, 3, mouse_x, mouse_y);
        display_menu(buttons, 3, title_font, default_font);
        MLV_update_window();

        switch (player_click(buttons, 3, mouse_x, mouse_y)) {
            case 1:
                game();
                MLV_draw_text(300, NB_TILES_Y * TILE_SIZE - 30, "Click anywhere on the screen to return to the menu",
                              MLV_COLOR_WHITE);
                MLV_update_window();
                MLV_wait_mouse(NULL, NULL);
                break;
            case 2:
                leaderboard();
                break;
            case 3:
                quit = 1;
                break;
            default:
                break;
        }
    }

    MLV_free_window();
    free(default_font);
    free(title_font);
    return 0;
}

/*

Added new tile sprite.
Changed winning_screen and losing_screen so they are displayed before winning and losing actions.
Removed an fprintf in Ranking.c in get_score_list*/