#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "AI.h"
#include "chess_functions.h"
#include "constants.h"
#include "board.h"

void ai_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn, int ai_difficulty, char *move) {

    int random_move[5], best_move[5], num_moves, to_row, to_col, move_score;
    int best_score = INT_MIN;
    int valid_moves[BOARD_SIZE * BOARD_SIZE][2];

    switch (ai_difficulty) {

        /* easy AI */
        case 1: 

            /* generate a random move */
            if (generate_random_move(chessboard, turn, random_move)) {
                move[0] = random_move[1] + 'A' - 1;
                move[1] = BOARD_SIZE - random_move[0] + '0';
                move[2] = random_move[3] + 'A' - 1;
                move[3] = BOARD_SIZE - random_move[2] + '0';
                move[4] = '\0';

                /* Check if the AI pawn reaches the promotion row */
                if (chessboard[random_move[0]][random_move[1]].name == 'P' && ((turn == 'w' && random_move[2] == 1) || (turn == 'b' && random_move[2] == BOARD_SIZE - 1))) {

                    /* Promote the pawn to a queen */
                    chessboard[random_move[0]][random_move[1]].name = 'Q';
                    printf("P has been promoted to Q\n");
                }

                move_piece(chessboard, move, turn);
                printf("%s\n", move);

            }

            break;

        /* hard AI */
        case 2:

            /* generate the best move */
            for (int from_row = 0; from_row < BOARD_SIZE; from_row++) {
                for (int from_col = 0; from_col < BOARD_SIZE; from_col++) {
                    if (chessboard[from_row][from_col].player == turn) {

                        /* generate all valid moves for the current piece */
                        num_moves = generate_valid_moves(chessboard, from_row, from_col, valid_moves);

                        for (int move_index = 0; move_index < num_moves; move_index++) {

                            to_row = valid_moves[move_index][0];
                            to_col = valid_moves[move_index][1];
                            move_score = score(chessboard, from_row, from_col, to_row, to_col);

                            /* check if the move is better than the current best move */
                            if (move_score > best_score) {
                                best_score = move_score;
                                best_move[0] = from_row;
                                best_move[1] = from_col;
                                best_move[2] = to_row;
                                best_move[3] = to_col;
                            }
                        }
                    }
                }
            }

            /* convert the best move to a string */
            move[0] = best_move[1] + 'A' - 1;
            move[1] = BOARD_SIZE - best_move[0] + '0';
            move[2] = best_move[3] + 'A' - 1;
            move[3] = BOARD_SIZE - best_move[2] + '0';
            move[4] = '\0';

            /* Check if the AI pawn reaches the promotion row */
            if (chessboard[best_move[0]][best_move[1]].name == 'P' && ((turn == 'w' && best_move[2] == 1) || (turn == 'b' && best_move[2] == BOARD_SIZE - 1))) {
                chessboard[best_move[0]][best_move[1]].name = 'Q';
                printf("P has been promoted to Q\n");
            }

            move_piece(chessboard, move, turn);
            printf("%s\n", move);

            break;
    }
}

int score(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col)
{
	switch (chessboard[from_row][from_col].name) {

		case 'P':

			if(chessboard[to_row][to_col].name == 'P'){
				return 1;
			}

			else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
			}

			else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
			}

			else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
			}

			else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
			}

			else{
				return 0;
			}
			break;

		case 'N':

                        if(chessboard[to_row][to_col].name == 'P'){
                                return 1;
                        }

                        else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
                        }

                        else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
                        }

                        else{
                                return 0;
                        }
			break;
			
		case 'B':

                        if(chessboard[to_row][to_col].name == 'P'){
                                return 1;
                        }

                        else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
                        }

                        else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
                        }

                        else{
                                return 0;
                        }
			break;

		case 'R':

                        if(chessboard[to_row][to_col].name == 'P'){
                                return 1;
                        }

                        else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
                        }

                        else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
                        }

                        else{
                                return 0;
                        }
			break;

		case 'Q':

                        if(chessboard[to_row][to_col].name == 'P'){
                                return 1;
                        }

                        else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
                        }

                        else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
                        }

                        else{
                                return 0;
                        }
			break;

		case 'K':

                        if(chessboard[to_row][to_col].name == 'P'){
                                return 1;
                        }

                        else if(chessboard[to_row][to_col].name == 'N'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'B'){
                                return 2;
                        }

                        else if(chessboard[to_row][to_col].name == 'R'){
                                return 4;
                        }

                        else if(chessboard[to_row][to_col].name == 'Q'){
                                return 8;
                        }

                        else{
                                return 0;
                        }
			break;

		default:
			return 0;
	}
}