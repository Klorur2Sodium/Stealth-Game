#include "../include/Ranking.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

/**
 * @brief Prompts the player for their name.
 *
 * This function prints a prompt to the console asking the player to input their name.
 * The player's name is then stored in the provided character array.
 * If the player's name is longer than the maximum allowed length (MAX_NAME_LENGTH - 1),
 * it will be truncated to fit in the array.
 * @param player_name Array to store the player's name in.
 */
static void prompt_player_name(char player_name[MAX_NAME_LENGTH]) {
    printf("please input your name\n");
    fgets(player_name, MAX_NAME_LENGTH, stdin);

    /* Remove the newline character from the end of the player's name */
    strtok(player_name, "\n");
}


void put_in_scores_file(int time, int used_mana) {
    FILE *file;
    Score new_score;
    new_score.time = time;
    new_score.used_mana = used_mana;
    prompt_player_name(new_score.name);
    file = fopen(SCORES_FILE_NAME, "ab");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open file\n");
        return;
    }
    fwrite(&new_score, sizeof(Score), 1, file);
    fclose(file);
}

/**
 * @brief Returns the number of high score records in a file.
 * This function reads the high score records from the provided file and returns the number of records found.
 * The file pointer is left at the position it was at before the function was called.
 * @param file Pointer to the file to read from.
 * @return The number of high score records in the file.
 */
static int number_of_scores(FILE *file) {
    int count;
    Score holder;
    count = 0;
    while (fread(&holder, sizeof(Score), 1, file)) {
        count++;
    }
    fseek(file, 0, SEEK_SET);
    return count;
}

/**
 * @brief Allocates memory for a list of high score records.
 * This function allocates memory for a list of high score records with the given number of elements.
 * If the memory allocation fails, an error message is printed to the console.
 * @param nb_elems Number of elements to allocate memory for.
 * @return Pointer to the allocated memory.
 */
static Score *allocate_list_of_scores(int nb_elems) {
    Score *list;
    list = (Score *) malloc(nb_elems * sizeof(Score));
    if (list == NULL)
        fprintf(stderr, "Not enough memory\n");
    return list;
}


int compare_time_scores(const void *elem1, const void *elem2) {
    return ((Score *) elem1)->time - ((Score *) elem2)->time;
}


int compare_mana_scores(const void *elem1, const void *elem2) {
    return ((Score *) elem1)->used_mana - ((Score *) elem2)->used_mana;
}

/**
 * @brief Returns the minimum of two integers.
 * This function returns the minimum of the two provided integers.
 * @param a First integer to compare.
 * @param b Second integer to compare.
 * @return The minimum of the two integers.
 */
static int min(int a, int b) {
    return a < b ? a : b;
}

/**
 * @brief Displays a list of high score records sorted by a comparison function.
 * This function sorts the provided list of high score records using the provided comparison function
 * and displays the top 10 scores in the console.
 * @param list Pointer to the list of high score records to display.
 * @param nb_scores Number of elements in the list.
 * @param compare Pointer to the comparison function to use for sorting.
 * @param comparaison_name String describing the comparison function (for display purposes).
 */
static void display_list_by_comparison_function(Score *list, int nb_scores, int (*compare)(const void *, const void
*), char *comparaison_name) {
    int i;
    qsort(list, nb_scores, sizeof(Score), compare);
    printf("Sorting by %s\n", comparaison_name);
    printf("%s %-*s %-4s %s\n", "rank", MAX_NAME_LENGTH, "name", "time", "consumed mana");
    for (i = 0; i < min(10, nb_scores); i++) {
        printf("%-4d %-*s %-4d %d\n", i + 1, MAX_NAME_LENGTH, list[i].name, list[i].time, list[i].used_mana);
    }
    printf("\n");
}

int get_scores_list(Score **scores, int *nb_scores) {
    FILE *file;

    file = fopen(SCORES_FILE_NAME, "rb");
    if (file == NULL) {
        return 0;
    }
    *nb_scores = number_of_scores(file);
    *scores = allocate_list_of_scores(*nb_scores);
    fread(*scores, sizeof(Score), *nb_scores, file);
    fclose(file);
    if (*scores == NULL) return 0;

    return 1;
}

void display_high_scores() {
    Score *list = NULL;
    int nb_scores;
    if (!get_scores_list(&list, &nb_scores)) return;

    display_list_by_comparison_function(list, nb_scores, compare_time_scores, "time");
    display_list_by_comparison_function(list, nb_scores, compare_mana_scores, "mana");
    free(list);
}