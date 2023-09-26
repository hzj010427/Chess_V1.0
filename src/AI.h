#ifndef AI_H
#define AI_H

#include "board.h"
#include "constants.h"

void ai_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn, int ai_difficulty, char *move);

int score(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col);

#endif 