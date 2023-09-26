#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include "constants.h"

/* global variables */
int en_passant[2];

typedef struct {
    char name;
    char player;
    int movement;
    int x;
    int y;
} Piece;

enum pieces {
    rook='R',
    pawn='P',
    knight='N',
    king='K',
    queen='Q',
    bishop='B',
    empty=' '
};

typedef struct {
    Piece chessboard[BOARD_SIZE][BOARD_SIZE];
    int en_passant[2];
    char turn;
} BoardState;

typedef struct {
    /* can only store MAX_UNDO_COUNT moves */
    BoardState states[MAX_UNDO_COUNT];
    int size;
} BoardStateStack;

/* More data structures here */

void addpiece(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char name, char player, bool movement, int x, int y);

void displaychessboard(Piece chessboard[BOARD_SIZE][BOARD_SIZE]);

void initiatechessboard(Piece chessboard[BOARD_SIZE][BOARD_SIZE]);

void clean_board(Piece chessboard[BOARD_SIZE][BOARD_SIZE]);

void save_board_state(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int en_passant[2], char turn, BoardState *state);

void restore_board_state(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int en_passant[2], char *turn, BoardState *state);

void init_board_state_stack(BoardStateStack *stack);

void push_board_state(BoardStateStack *stack, BoardState *state);

BoardState pop_board_state(BoardStateStack *stack);

/* More functions here */

#endif

/* EOF */
