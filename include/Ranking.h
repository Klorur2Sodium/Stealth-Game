#ifndef __RANKING__
#define __RANKING__

#define MAX_NAME_LENGTH (15 + 1)
#define SCORES_FILE_NAME "./Scores"

/**
 * @brief Represents the score of a player with his name, the time he took to win and 
 * the mana he consumed.
 * 
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    int time;
    int used_mana;
} Score;

/**
 * @brief Stores a new high score record.
 * This function prompts the player for their name and stores a new high score record
 * with the provided time and used mana values in the file SCORES_FILE_NAME.
 * If the file cannot be opened for writing, an error message is printed to the console.
 * @param time Time taken to complete the game.
 * @param used_mana Amount of mana used in the game.
 */
void put_in_scores_file(int time, int used_mana);

/**
 * @brief Displays the high scores.
 * This function reads the high score records from the file SCORES_FILE_NAME,
 * sorts them by time and used mana, and displays the top 10 scores in the console.
 * If the file cannot be opened for reading, an error message is printed to the console.
 */
void display_high_scores();

/**
 * @brief Get the list of scores from the Scores file.
 * 
 * @param scores Pointer to the list containing the scores.
 * @param nb_scores Number of scores. 
 * @return int 0 if scores is NULL, 1 if not.
 */
int get_scores_list(Score **scores, int *nb_scores);

/**
 * @brief Comparison function for sorting high score records by used mana.
 * This function compares two high score records based on the amount of mana used in the game.
 * It is intended to be used as a comparator function for qsort().
 * @param elem1 Pointer to the first high score record to compare.
 * @param elem2 Pointer to the second high score record to compare.
 * @return A negative value if the amount of mana used in the game in the first record is less than the amount in the second,
 * a positive value if it is greater, and 0 if they are equal.
 */
int compare_mana_scores(const void *elem1, const void *elem2);

/**
 * @brief Comparison function for sorting high score records by time.
 * This function compares two high score records based on the time taken to complete the game.
 * It is intended to be used as a comparator function for qsort().
 * @param elem1 Pointer to the first high score record to compare.
 * @param elem2 Pointer to the second high score record to compare.
 * @return A negative value if the time taken to complete the game in the first record is less than the time in the second,
 * a positive value if it is greater, and 0 if they are equal.
*/
int compare_time_scores(const void *elem1, const void *elem2);

#endif
