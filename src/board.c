#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "board.h"

void addpiece(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char name, char player, bool movement, int x, int y){
    chessboard[x][y].name=name;
    chessboard[x][y].player=player;
    chessboard[x][y].movement=movement;
    chessboard[x][y].x=x;
    chessboard[x][y].y=y;
}

void displaychessboard(Piece chessboard[BOARD_SIZE][BOARD_SIZE]){
    int i , j , k ;
    printf("\n");
    for (i=1; i<9;i++){
        printf("  ");
        for (k=1; k<9; k++){
            printf("+----");
        }
        printf("+");

        /* print the input format */
        if (i == 1) {
        printf("                                   ");
        } else if (i == 2) {
            printf("   |           1 - Undo           |");
        } else if (i == 3) {
            printf("   |           2 - Hints          |");
        } else if (i == 4) {
            printf("   |     A2A3 - Move A2 to A3     |");
        } else if (i == 5) {
            printf("   |    O-O - Kingside castling   |");
        } else if (i == 6) {
            printf("   |  O-O-O - Queenside castling  |");
        } else if (i == 7) {
            printf("   |         exit - Exit          |");
        } else if (i == 8) {
            printf("   |         save - Save          |");
        } else if (i == 9) {
            printf("                                   ");
        } else {
            printf("                                ");
        }

        printf("\n");
        printf(" ");
        printf("%d",abs(i-9));
        for (j=1;j<9;j++){
            printf("| %c%c ", chessboard[i][j].player,chessboard[i][j].name);
        }
        printf("|\n");
    }
    printf("  ");
    for (k=1; k<9; k++){
            printf("+----");
    }
    printf("+\n");
    for (i=0; i<8;i++){
        printf("    %c", 'A'+i);
    }
    printf("\n");
}

void initiatechessboard(Piece chessboard[BOARD_SIZE][BOARD_SIZE]){

    /* Add pieces to the chessboard */
    for (int i=1; i<9; i++){
        addpiece(chessboard, pawn, 'b', false, 2, i);
    }
    for (int i=1; i<9; i++){
        addpiece(chessboard, pawn, 'w', false, 7, i);
    }
    for (int i=1; i<9; i++){
        addpiece(chessboard, empty, ' ', false, 3, i);
    }
    for (int i=1; i<9; i++){
        addpiece(chessboard, empty, ' ', false, 4, i);
    }
    for (int i=1; i<9; i++){
        addpiece(chessboard, empty, ' ', false, 5, i);
    }
    for (int i=1; i<9; i++){
        addpiece(chessboard, empty, ' ', false, 6, i);
    }
    addpiece(chessboard, rook, 'b', false, 1,1);
    addpiece(chessboard, knight, 'b', false, 1,2);
    addpiece(chessboard, bishop, 'b', false, 1,3);
    addpiece(chessboard, queen, 'b', false, 1,4);
    addpiece(chessboard, king, 'b', false, 1,5);
    addpiece(chessboard, bishop, 'b', false, 1,6);
    addpiece(chessboard, knight, 'b', false, 1,7);
    addpiece(chessboard, rook, 'b', false, 1,8);
    addpiece(chessboard, rook, 'w', false, 8,1);
    addpiece(chessboard, knight, 'w', false, 8,2);
    addpiece(chessboard, bishop, 'w', false,8,3);
    addpiece(chessboard, queen, 'w', false, 8,4);
    addpiece(chessboard, king, 'w', false, 8,5);
    addpiece(chessboard, bishop, 'w', false, 8,6);
    addpiece(chessboard, knight, 'w', false, 8,7);
    addpiece(chessboard, rook, 'w', false, 8,8);
    en_passant[0] = 0;
    en_passant[1] = 0;
    }

void clean_board(Piece chessboard[BOARD_SIZE][BOARD_SIZE]){
    for (int i=1;i<=9;i++){
        for (int j=1; j<=9;j++){
            addpiece(chessboard, empty, ' ', false, i,j);
        }
    }
}

void save_board_state(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int en_passant[2], char turn, BoardState *state) {
    /* Save the current state of the board */
    memcpy(state->chessboard, chessboard, sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);

    state->en_passant[0] = en_passant[0];
    state->en_passant[1] = en_passant[1];
    state->turn = turn;
}

void restore_board_state(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int en_passant[2], char *turn, BoardState *state) {
    /* Restore the state of the board */
    memcpy(chessboard, state->chessboard, sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);

    en_passant[0] = state->en_passant[0];
    en_passant[1] = state->en_passant[1];
    *turn = state->turn;
}

void init_board_state_stack(BoardStateStack *stack) {
    stack->size = 0;
}

void push_board_state(BoardStateStack *stack, BoardState *state) {
    if (stack->size < MAX_UNDO_COUNT) {
        stack->states[stack->size] = *state;
        stack->size += 1;
    } 
    else {
        printf("Error: Cannot save more board states.\n");
    }
}

BoardState pop_board_state(BoardStateStack *stack) {
    if (stack->size > 0) {
        stack->size -= 1;
        return stack->states[stack->size];
    } 
    else {
        printf("Error: No board states to undo.\n");
        return stack->states[0];
    }
}
/* More functions here */