#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "constants.h"
#include "board.h"
#include "menu.h"
#include "chess_functions.h"
#include "AI.h"

int start_game(int game_mode, int player_color, int ai_difficulty) {

    /* flush the input buffer */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    /* local variable initialization */
    Piece chessboard[9][9];
    BoardStateStack board_state_stack;
    BoardState current_board_state;
    BoardState previous_board_state;
    char turn = 'w';
    char input[256];
    char ai_input[5];
    
    /* initialize the board state stack */
    init_board_state_stack(&board_state_stack);

    /* initialize the chessboard */
    initiatechessboard(chessboard);
    displaychessboard(chessboard);

    while (1) {

        /* AI vs AI */
        if (game_mode == 3) {

            /* Implement AI move generation here based on ai_difficulty */
            printf("%s AI's move: ", turn == 'w' ? "White" : "Black");

            /* AI generates a move */
            ai_move(chessboard, turn, ai_difficulty, ai_input);

            displaychessboard(chessboard);
                    
            /* change the turn */
            turn = (turn == 'w') ? 'b' : 'w';

            /* check for checkmate */
            if (is_checkmate(chessboard, turn)) {
                printf("%s AI is checkmated. %s AI wins!\n", turn == 'w' ? "White" : "Black", turn == 'w' ? "Black" : "White");
                return 1;
            }
            
            /* check for stalemate */
            if (is_stalemate(chessboard, turn)) {
                printf("Stalemate! The game is a draw.\n");
                return 1;
            }

            /* check for check */
            if (is_check(chessboard, turn)) {
                printf("%s AI is in check!\n", turn == 'w' ? "White" : "Black");
            }

            printf("\n");
        }

        /* Human vs AI */
        else if (game_mode == 2) {
            if ((turn == 'w' && player_color == 1) || (turn == 'b' && player_color == 2)) {

                /* Human player's turn */
                printf("%s player's move: ", turn == 'w' ? "White" : "Black");

                /* flush the input buffer */
                fflush(stdout);
                input[0] = '\0';

                /* read input */
                if (fgets(input, sizeof(input), stdin) != NULL) {

                    /* remove newline, if present */
                    input[strcspn(input, "\n")] = 0;

                    /* check for no input */
                    if (strlen(input) == 0) {
                        printf("Error: No input provided. Please enter a valid move.\n");
                        continue;
                    }

                    /* check for undo input */
                    else if (strcmp(input, "1") == 0) {
                            if (board_state_stack.size >= 1) {

                                /* Pop the last board state */
                                previous_board_state = pop_board_state(&board_state_stack);
                                
                                /* Restore the board state */
                                restore_board_state(chessboard, en_passant, &turn, &previous_board_state);
                            } 
                            else {
                                printf("Error: Not enough board states to undo.\n");
                                continue;
                            }

                            displaychessboard(chessboard);
                        continue;
                    }

                    /* check for hints input */
                    else if (strcmp(input, "2") == 0) {
                        get_random_hint(chessboard, turn);
                        continue;
                    }

                    /* check for castling input "O-O-O" and "O-O" */
                    else if (strcmp(input, "O-O-O") == 0 || strcmp(input, "O-O") == 0) {

                        /* castling input convert to king moves */
                        if (strcmp(input, "O-O") == 0) {
                            if (turn == 'w') {
                                strcpy(input, "E1G1");
                            } 
                            else {
                                strcpy(input, "E8G8");
                            }
                        } 
                        else if (strcmp(input, "O-O-O") == 0) {
                            if (turn == 'w') {
                                strcpy(input, "E1C1");
                            } 
                            else {
                                strcpy(input, "E8C8");
                            }
                        }
                    }

                    /* check for exit command */
                    else if (strcmp(input, "exit") == 0) {
                        printf("User exited the game.\n");
                        return 0;
                    }

                    /* check for save command */
                    else if (strcmp(input, "save") == 0) {
                        printf("User saved the game.\n");
                        return 0;
                    }

                    /* check for invalid input length */
                    else if (strlen(input) != 4) {
                        printf("Error: Invalid move. Please try again.\n");
                        continue;
                    }

                    /* save the board state */
                    save_board_state(chessboard, en_passant, turn, &current_board_state);

                    /* valid input, try to move the piece */
                    if (move_piece(chessboard, input, turn)) {

                        /* push the board state onto the stack */
                        push_board_state(&board_state_stack, &current_board_state);

                        displaychessboard(chessboard);
                        
                        /* change the turn */
                        turn = (turn == 'w') ? 'b' : 'w';

                        /* check for checkmate */
                        if (is_checkmate(chessboard, turn)) {
                            printf("%s AI is checkmated. %s player wins!\n", turn == 'w' ? "White" : "Black", turn == 'w' ? "Black" : "White");
                            return 1;
                        }
                        
                        /* check for stalemate */
                        if (is_stalemate(chessboard, turn)) {
                            printf("Stalemate! The game is a draw.\n");
                            return 1;
                        }

                        /* check for check */
                        if (is_check(chessboard, turn)) {
                            printf("%s AI is in check!\n", turn == 'w' ? "White" : "Black");
                        }

                        printf("\n");
                    }
                    else {
                        printf("Error: Invalid move. Please try again.\n");
                    }

                }
                else {
                    printf("Error: Input error. Please try again.\n");
                }
            } 
            else {

                /* AI's turn */
                printf("%s AI's move: ", turn == 'w' ? "White" : "Black");

                /* AI generates a move */
                ai_move(chessboard, turn, ai_difficulty, ai_input);

                displaychessboard(chessboard);

                /* change the turn */
                turn = (turn == 'w') ? 'b' : 'w';

                /* check for checkmate */
                if (is_checkmate(chessboard, turn)) {
                    printf("%s player is checkmated. %s AI wins!\n", turn == 'w' ? "White" : "Black", turn == 'w' ? "Black" : "White");
                    return 1;
                }

                /* check for stalemate */
                if (is_stalemate(chessboard, turn)) {
                    printf("Stalemate! The game is a draw.\n");
                    return 1;
                }

                /* check for check */
                if (is_check(chessboard, turn)) {
                    printf("%s player is in check!\n", turn == 'w' ? "White" : "Black");
                }
            }
        }

        /* Human vs Human */
        else {

            printf("%s player's move: ", turn == 'w' ? "White" : "Black");

            /* flush the input buffer */
            fflush(stdout);
            input[0] = '\0';

            /* read input */
            if (fgets(input, sizeof(input), stdin) != NULL) {

                /* remove newline, if present */
                input[strcspn(input, "\n")] = 0;

                /* check for no input */
                if (strlen(input) == 0) {
                    printf("Error: No input provided. Please enter a valid move.\n");
                    continue;
                }

                /* check for undo input */
                else if (strcmp(input, "1") == 0) {
                        if (board_state_stack.size >= 1) {

                            /* Pop the last board state */
                            previous_board_state = pop_board_state(&board_state_stack);
                            
                            /* Restore the board state */
                            restore_board_state(chessboard, en_passant, &turn, &previous_board_state);
                        } 
                        else {
                            printf("Error: Not enough board states to undo.\n");
                            continue;
                        }

                        displaychessboard(chessboard);
                    continue;
                }

                /* check for hints input */
                else if (strcmp(input, "2") == 0) {
                    get_random_hint(chessboard, turn);
                    continue;
                }

                /* check for castling input "O-O-O" and "O-O" */
                else if (strcmp(input, "O-O-O") == 0 || strcmp(input, "O-O") == 0) {

                    /* castling input convert to king moves */
                    if (strcmp(input, "O-O") == 0) {
                        if (turn == 'w') {
                            strcpy(input, "E1G1");
                        } 
                        else {
                            strcpy(input, "E8G8");
                        }
                    } 
                    else if (strcmp(input, "O-O-O") == 0) {
                        if (turn == 'w') {
                            strcpy(input, "E1C1");
                        } 
                        else {
                            strcpy(input, "E8C8");
                        }
                    }
                }

                /* check for exit command */
                else if (strcmp(input, "exit") == 0) {
                    printf("User exited the game.\n");
                    return 0;
                }

                /* check for save command */
                else if (strcmp(input, "save") == 0) {
                    printf("User saved the game.\n");
                    return 0;
                }

                /* check for invalid input length */
                else if (strlen(input) != 4) {
                    printf("Error: Invalid move. Please try again.\n");
                    continue;
                }

                /* save the board state */
                save_board_state(chessboard, en_passant, turn, &current_board_state);

                /* valid input, try to move the piece */
                if (move_piece(chessboard, input, turn)) {

                    /* push the board state onto the stack */
                    push_board_state(&board_state_stack, &current_board_state);

                    displaychessboard(chessboard);
                    
                    /* change the turn */
                    turn = (turn == 'w') ? 'b' : 'w';

                    /* check for checkmate */
                    if (is_checkmate(chessboard, turn)) {
                        printf("%s player is checkmated. %s player wins!\n", turn == 'w' ? "White" : "Black", turn == 'w' ? "Black" : "White");
                        return 1;
                    }
                    
                    /* check for stalemate */
                    if (is_stalemate(chessboard, turn)) {
                        printf("Stalemate! The game is a draw.\n");
                        return 1;
                    }

                    /* check for check */
                    if (is_check(chessboard, turn)) {
                        printf("%s player is in check!\n", turn == 'w' ? "White" : "Black");
                    }

                    printf("\n");
                }
                else {
                    printf("Error: Invalid move. Please try again.\n");
                }

            }
            else {
                printf("Error: Input error. Please try again.\n");
            }
        }
    }
}

