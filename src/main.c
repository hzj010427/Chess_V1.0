#include <stdio.h>
#include "constants.h"
#include "menu.h"
#include "game.h"

int main() {

    /* Local variable initialization */
    int mainMenuChoice, secondMenuChoice, thirdMenuChoice, playerColorChoice, aiMenuChoice;

    /* Main menu loop */
    while ((mainMenuChoice = main_menu()) != 2) {
        if (mainMenuChoice == 1) {

            /* Start the game */
            secondMenuChoice = second_level_menu();

            if (secondMenuChoice == 2) {
                thirdMenuChoice = third_level_menu();

                /* Human vs AI */
                if (thirdMenuChoice == 2) {
                    playerColorChoice = choose_player_color();
                    aiMenuChoice = ai_settings_menu();

                    /* update later */
                    if (start_game(thirdMenuChoice, playerColorChoice, aiMenuChoice) == 0) {
                        break;
                    }

                    /* game ends normally (update later) */
                    else {
                        continue;
                    }
                } 

                /* AI vs AI */
                else if (thirdMenuChoice == 3) {
                    aiMenuChoice = ai_settings_menu();

                    /* update later */
                    if (start_game(thirdMenuChoice, 1, aiMenuChoice) == 0) {
                        break;
                    }

                    /* game ends normally (update later) */
                    else {
                        continue;
                    }
                }

                /* Human vs Human */
                else {

                    /* user exits the game */
                    if (start_game(thirdMenuChoice, 1, 0) == 0) {
                        break;
                    }

                    /* game ends normally (update later) */
                    else {
                        continue;
                    }
                }
            }
            else if (secondMenuChoice == 3) {

                /* Load game code here */
                printf("Load game is not implemented yet!\n");
                break;
            }
        }
    }

    printf("Exiting the game...\n");

    return 0;
}


    
