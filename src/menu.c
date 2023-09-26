#include <stdio.h>
#include <string.h>
#include "menu.h"

int main_menu() {
    int choice;
    do {
        printf("Welcome to Chess!\n");
        printf("1. Start new game\n");
        printf("2. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 2) {
            printf("Invalid choice. Please enter a valid choice.\n");
        }
    } while (choice < 1 || choice > 2);

    return choice;
}

int second_level_menu() {
    int choice;
    do {
        printf("\nSecond Level Menu:\n");
        printf("1. Back to Main Menu\n");
        printf("2. Choose Game Mode\n");
        printf("3. Load Game\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter a valid choice.\n");
        }
    } while (choice < 1 || choice > 3);

    return choice;
}

int third_level_menu() {
    int choice;
    do {
        printf("\nThird Level Menu:\n");
        printf("1. Player vs Player\n");
        printf("2. Player vs AI\n");
        printf("3. AI vs AI\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter a valid choice.\n");
        }
    } while (choice < 1 || choice > 3);

    return choice;
}

int choose_player_color() {
    int choice;
    do {
        printf("\nChoose Player Color:\n");
        printf("1. White\n");
        printf("2. Black\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 2) {
            printf("Invalid choice. Please enter a valid choice.\n");
        }
    } while (choice < 1 || choice > 2);

    return choice;
}

int ai_settings_menu() {
    int choice;
    do {
        printf("\nChoose AI difficulty:\n");
        printf("1. Easy\n");
        printf("2. Hard\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 2) {
            printf("Invalid choice. Please enter a valid choice.\n");
        }
    } while (choice < 1 || choice > 2);

    return choice;
}

char promote_pawn() {

    /* declare variables */
    char choice;
    char input[256];

    /* print the menu */
    printf("Pawn promotion!\n");
    printf("Choose a piece to promote to:\n");
    printf("  Q - Queen\n");
    printf("  R - Rook\n");
    printf("  B - Bishop\n");
    printf("  N - Knight\n");

    while (1) {

        printf("Enter your choice: ");

        /* flush the input buffer */
            fflush(stdout);
            input[0] = '\0';
            
        /* read input */
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0;

            /* check for no input */
            if (strlen(input) == 0) {
                printf("Error: No input provided.\n");
                continue;
            }

            /* check for invalid input length */
            else if (strlen(input) != 1) {
                printf("Error: Invalid input format. Please enter a valid move in the correct format.\n");
                continue;
            }

            /* check for invalid input */
            else if (input[0] != 'Q' && input[0] != 'R' && input[0] != 'B' && input[0] != 'N') {
                printf("Error: Invalid choice. Please choose Q, R, B, or N.\n");
                continue;
            }

            /* valid input */
            else {
                choice = input[0];
                break;
            }
        }
    }

    printf("P has been promoted to %c!\n", choice);

    return choice;
}
