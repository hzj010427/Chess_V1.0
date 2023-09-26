#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include "board.h"
#include "menu.h"
#include "constants.h"
#include "chess_functions.h"

int generate_all_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int row, int col, int (*valid_moves)[2]) {

    /* local variable initialization */
    int move_count = 0;

    /* dr and dc are the row and column offsets for the piece */
    int dr, dc, starting_row;

    /* printf("%d%d\n", row, col); */

    /* update the valid_moves array based on the piece type */
    switch (chessboard[row][col].name) {

        /* pawn moves */
        case 'P':
            /* if the pawn is white, it moves up the board, otherwise it moves down the board */
            dr = (chessboard[row][col].player == 'w') ? -1 : 1;
            if (is_valid_square(row + dr, col) && chessboard[row + dr][col].name == empty) {
                valid_moves[move_count][0] = row + dr;
                valid_moves[move_count][1] = col;
                move_count++;

                /* if the pawn is on its starting row, it can move two squares */
                starting_row = (chessboard[row][col].player== 'w') ? 7 : 2;
                if (row == starting_row && is_valid_square(row + 2 * dr, col) && chessboard[row + 2 * dr][col].name == empty) {
                    valid_moves[move_count][0] = row + 2 * dr;
                    valid_moves[move_count][1] = col;
                    move_count++;
                } 
            }

            /* if the pawn can capture a piece diagonally, add the capture move */
            for (dc = -1; dc <= 1; dc += 2) {

                /* if the square is valid and contains an enemy piece, add the move */
                if (is_valid_square(row + dr, col + dc) && chessboard[row + dr][col + dc].name != empty && chessboard[row + dr][col + dc].player != chessboard[row][col].player) {
                    valid_moves[move_count][0] = row + dr;
                    valid_moves[move_count][1] = col + dc;
                    move_count++;
                }
            }

            /* if the pawn can capture en passant, add the capture move */
            if (is_valid_square(row + dr, col - 1) && en_passant[0] == row + dr && en_passant[1] == col - 1) {
                valid_moves[move_count][0] = row + dr;
                valid_moves[move_count][1] = col - 1;
                move_count++;
            }

            if (is_valid_square(row + dr, col + 1) && en_passant[0] == row + dr && en_passant[1] == col + 1) {
                valid_moves[move_count][0] = row + dr;
                valid_moves[move_count][1] = col + 1;
                move_count++;
            }

            break;

        /* rook moves */
        case 'R':
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    
                    /* dr or dc equal to zero but can not equal to zero at the same time */
                    if (dr * dc == 0 && (dr != 0 || dc != 0)) {
                        move_count = add_line_moves(chessboard, row, col, dr, dc, valid_moves, move_count);
                    }
                }
            }
            break;

        /* knight moves */
        case 'N':
            for (dr = -2; dr <= 2; dr++) {
                for (dc = -2; dc <= 2; dc++) {
                    if (abs(dr) + abs(dc) == 3) {
                        move_count = add_single_move(chessboard, row, col, row + dr, col + dc, valid_moves, move_count);
                    }
                }
            }
            break;

        /* bishop moves */
        case 'B':
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    if (dr * dc != 0) {
                        move_count = add_line_moves(chessboard, row, col, dr, dc, valid_moves, move_count);
                    }
                }
            }
            break;

        /* queen moves */
        case 'Q':
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    if (dr != 0 || dc != 0) {
                        move_count = add_line_moves(chessboard, row, col, dr, dc, valid_moves, move_count);
                    }
                }
            }
            break;

        /* king moves */
        case 'K':
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    if (dr != 0 || dc != 0) {
                        move_count = add_single_move(chessboard, row, col, row + dr, col + dc, valid_moves, move_count);
                    }
                }
            }
            break;

        default:
            return move_count;
    }

    return move_count;
}

int generate_valid_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int row, int col, int (*valid_moves)[2]) {

    /* local variables */
    int move_count = generate_all_moves(chessboard, row, col, valid_moves);
    int temp_row, temp_col;
    Piece temp_chessboard[BOARD_SIZE][BOARD_SIZE];

    /* Check if the move puts the king in check */
    for (int i = 0; i < move_count; i++) {

        memcpy(temp_chessboard, chessboard, sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);

        temp_row = valid_moves[i][0];
        temp_col = valid_moves[i][1];

        temp_chessboard[temp_row][temp_col] = temp_chessboard[row][col];
        addpiece(temp_chessboard, empty, ' ', false, row, col);

        /* if the move puts the king in check, remove the move from the list */
        if (is_check(temp_chessboard, chessboard[row][col].player)) {
            move_count--;

            /* move the last move to the current position */
            if (i < move_count) {
                valid_moves[i][0] = valid_moves[move_count][0];
                valid_moves[i][1] = valid_moves[move_count][1];
            }

            /* decrement i to check the new move in the current position */
            i--;
        }
    }

    return move_count;
}

/* returns 1 if the move is valid, 0 otherwise */
int is_valid_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, char turn) {

    /* local variable initialization */
    /* valid_moves is a 2D array of integers, where each row is a move and the first column is the row and the second column is the column */
    int valid_moves[MAX_MOVES][2];
    int move_count;

    /* check if the starting square is empty */
    if (chessboard[from_row][from_col].name == empty) {
        return 0;
    }

    /* check if the move is out of bounds */
    if (to_row < 0 || to_row >= BOARD_SIZE || to_col < 0 || to_col >= BOARD_SIZE) {
        return 0;
    }

    /* check if the move is to the same square */
    if (from_row == to_row && from_col == to_col) {
        return 0;
    }

    /* check turn */
    if (chessboard[from_row][from_col].player != turn) {
        return 0;
    }

    /* Check if the move is a castling move */
    if (is_castling_move(chessboard, from_row, from_col, to_row, to_col, turn)) {
        return 1;
    }

    move_count = generate_valid_moves(chessboard, from_row, from_col, valid_moves);

    /* check if the move is in the valid_moves array */ 
    for (int i = 0; i < move_count; i++) {
        if (to_row == valid_moves[i][0] && to_col == valid_moves[i][1]) {
            return 1;
        }
    }

    return 0;
}

/* returns 1 if the move is valid, 0 otherwise */
int move_piece(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char *move, char turn) {

    /* local variable initialization */
    int from_row, from_col, to_row, to_col, rook_col, rook_to_col;
    char promoted_piece;

    /* convert the move to row and column numbers */
    from_col = abs(move[0] - 'A') + 1;
    from_row = abs(BOARD_SIZE - (move[1] - '0'));
    to_col = abs(move[2] - 'A') + 1;
    to_row = abs(BOARD_SIZE - (move[3] - '0'));

    /* if the move is valid, move the piece */
    if (is_valid_move(chessboard, from_row, from_col, to_row, to_col, turn)) {

        /* if the move is a castling move, move the rook */
        if (is_castling_move(chessboard, from_row, from_col, to_row, to_col, turn)) {

            /* move the rook */
            rook_col = to_col > from_col ? (BOARD_SIZE - 1) : 1;
            rook_to_col = to_col > from_col ? to_col - 1 : to_col + 1;

            chessboard[from_row][rook_to_col] = chessboard[from_row][rook_col];
            chessboard[from_row][rook_to_col].movement++;
            addpiece(chessboard, empty, ' ', false, from_row, rook_col);
        }

        /* if the move is en passant, remove the captured pawn */
        if (chessboard[from_row][from_col].name == 'P' && abs(to_col - from_col) == 1 && chessboard[to_row][to_col].name == empty) {
            addpiece(chessboard, empty, ' ', false, to_row + (turn == 'w' ? 1 : -1), to_col);
        }

        /* if the move is a pawn's double move, update the en_passant array */
        if (chessboard[from_row][from_col].name == 'P' && abs(from_row - to_row) == 2) {
            en_passant[0] = (from_row + to_row) / 2;
            en_passant[1] = to_col;
           
        }
        else {
            /* update the en_passant array */
            en_passant[0] = 0;
            en_passant[1] = 0;
        }

        /* Check for pawn promotion */
        if (chessboard[from_row][from_col].name == 'P' && ((turn == 'w' && to_row == 1) || (turn == 'b' && to_row == BOARD_SIZE - 1))) {

            /* promote the pawn */
            promoted_piece = promote_pawn();
            chessboard[from_row][from_col].name = promoted_piece;
        }

        /* move the piece */
        chessboard[to_row][to_col] = chessboard[from_row][from_col];
        /* increment the movement of the piece */
        chessboard[to_row][to_col].movement++;
        addpiece(chessboard, empty , ' ', false, from_row, from_col);

        return 1;
    }

    /* if the move is invalid */
    else {
        return 0;
    }
}

/* returns 1 if the square is valid, 0 otherwise */
int is_valid_square(int row, int col) {
    return row > 0 && row < BOARD_SIZE && col > 0 && col < BOARD_SIZE;
}

/* adds a single move to the valid_moves array */
int add_single_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, int (*valid_moves)[2], int move_count) {
    if (is_valid_square(to_row, to_col)) {

        /* if the destination square is empty or contains an enemy piece, add the move */
        if (chessboard[to_row][to_col].name == empty || chessboard[to_row][to_col].player != chessboard[from_row][from_col].player) {
            valid_moves[move_count][0] = to_row;
            valid_moves[move_count][1] = to_col;
            move_count++;
        }
    }
    return move_count;
}

/* adds all moves in a straight line to the valid_moves array */
int add_line_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int dr, int dc, int (*valid_moves)[2], int move_count) {
    int row = from_row + dr;
    int col = from_col + dc;

    /* add moves until a piece is encountered */
    while (is_valid_square(row, col)) {
        if (chessboard[row][col].name == empty) {
            valid_moves[move_count][0] = row;
            valid_moves[move_count][1] = col;
            move_count++;
        } 
        
        /* if an enemy piece is encountered, add the move and stop */
        else {
            if (chessboard[row][col].player != chessboard[from_row][from_col].player) {
                valid_moves[move_count][0] = row;
                valid_moves[move_count][1] = col;
                move_count++;
            }
            break;
        }

        row += dr;
        col += dc;
    }
    return move_count;
}

/* returns 1 if the castling move is valid, 0 otherwise */
int is_castling_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, char player) {

    /* local variables */
    int rook_col, direction, col;

    /* If the king is in check, castling is not allowed */
    if (is_check(chessboard, player)) {
        return 0;
    }

    /* Check if the move is a valid castling move for the king */
    if (chessboard[from_row][from_col].name != 'K' || chessboard[from_row][from_col].player != player || chessboard[from_row][from_col].movement != 0 || to_row != from_row) {
        return 0;
    }

    /* Determine the direction of the castling move (1 for kingside, -1 for queenside) */
    direction = (to_col > from_col) ? 1 : -1;

    /* Determine the column of the corresponding rook */
    rook_col = (direction == 1) ? (BOARD_SIZE - 1) : 1;

    /* Check if the rook is in the correct position and has not moved */
    if (chessboard[from_row][rook_col].name != 'R' || chessboard[from_row][rook_col].player != player || chessboard[from_row][rook_col].movement != 0) {
        return 0;
    }

	/* Check if the squares between the king and the rook are empty */
	col = from_col + direction;
	while (col != rook_col) {
		if (chessboard[from_row][col].name != empty) {
			return 0;
		}

		col += direction;
	}

    /* Create a temporary chessboard and copy the original chessboard into it */
    Piece temp_chessboard[BOARD_SIZE][BOARD_SIZE];
    memcpy(temp_chessboard, chessboard, sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);

    /* Check if the king is not moving through or into a square attacked by the opponent */
    col = from_col;
    while (col != to_col) {
        addpiece(temp_chessboard, empty, ' ', false, from_row, col);
        addpiece(temp_chessboard, 'K', player, true, from_row, col + direction);

        if (is_check(temp_chessboard, player)) {
            return 0;
        }

        col += direction;
    }

    return 1;
}

/* returns 1 if the king is in check, 0 otherwise */ 
int is_check(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player) {

    /* local variable initialization */
    int king_row, king_col;
    int valid_moves[MAX_MOVES][2];
    int move_count;

    /* find the king */
    for (int row = 1; row < 9; row++) {
        for (int col = 1; col < 9; col++) {
            if (chessboard[row][col].name == 'K' && chessboard[row][col].player == player) {
                king_row = row;
                king_col = col;
            }
        }
    }

    /* check if any enemy piece can capture the king */
    for (int row = 1; row < 9; row++) {
        for (int col = 1; col < 9; col++) {

            /* if the square contains an enemy piece, generate its moves */
            if (chessboard[row][col].player != player && chessboard[row][col].player != ' ') {
                move_count = generate_all_moves(chessboard, row, col, valid_moves);

                /* check if any of the generated moves is the king's square */
                for (int i = 0; i < move_count; i++) {
                    if (valid_moves[i][0] == king_row && valid_moves[i][1] == king_col) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

/* returns 1 if the king is in checkmate, 0 otherwise */
int is_checkmate(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player) {
    int valid_moves[MAX_MOVES][2];
    int move_count;

    /* If the king is not in check, the game is not over */
    if (!is_check(chessboard, player)) {
        return 0;
    } else {
        for (int row = 1; row < BOARD_SIZE; row++) {
            for (int col = 1; col < BOARD_SIZE; col++) {
                if (chessboard[row][col].player == player) {
                    move_count = generate_valid_moves(chessboard, row, col, valid_moves);

                    /* If there are valid moves, the game is not over */
                    if (move_count > 0) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

/* returns 1 if the king is in stalemate, 0 otherwise */
int is_stalemate(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player) {
    int valid_moves[MAX_MOVES][2];
    int move_count;

    /* if the king is in check, it is not a stalemate */
    if (is_check(chessboard, player)) {
        return 0;
    }

    else {
        for (int row = 1; row < 9; row++) {
            for (int col = 1; col < 9; col++) {
                if (chessboard[row][col].player == player) {
                    move_count = generate_valid_moves(chessboard, row, col, valid_moves);

                    /* If there are valid moves, the game is not over */
                    if (move_count > 0) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

void get_random_hint(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn) {
    int random_move[4];
    if (generate_random_move(chessboard, turn, random_move)) {
        printf("Hint: Move the piece at %c%c to %c%c\n",
               random_move[1] + 'A' - 1, BOARD_SIZE - random_move[0] + '0',
               random_move[3] + 'A' - 1, BOARD_SIZE - random_move[2] + '0');
    } 
    
    else {
        printf("There are no valid moves for the current player.\n");
    }
}

/* returns 1 if a random move was generated, 0 otherwise */
int generate_random_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn, int *random_move) {

    /* local variable initialization */
    int piece_valid_moves[MAX_MOVES][2];
    int player_pieces[MAX_MOVES][2];
    int total_piece_count = 0;
    int tried_pieces_count = 0;
    int random_piece_index, move_count, start_row, start_col, random_move_index;

    /* Seed the random number generator */
    srand(time(NULL));

    /* Iterate through the entire board and find the current player's pieces */
    for (int row = 1; row < BOARD_SIZE; row++) {
        for (int col = 1; col < BOARD_SIZE; col++) {
            if (chessboard[row][col].player == turn) {
                player_pieces[total_piece_count][0] = row;
                player_pieces[total_piece_count][1] = col;
                total_piece_count++;
            }
        }
    }

    /* If there are no player pieces, return 0 */
    if (total_piece_count == 0) {
        return 0;
    }

    /* Choose a random player piece and generate its valid moves */
    random_piece_index = rand() % total_piece_count;
    start_row = player_pieces[random_piece_index][0];
    start_col = player_pieces[random_piece_index][1];

    move_count = generate_valid_moves(chessboard, start_row, start_col, piece_valid_moves);

    while (tried_pieces_count < total_piece_count) {

        /* Choose a random player piece and generate its valid moves */
        random_piece_index = rand() % total_piece_count;
        start_row = player_pieces[random_piece_index][0];
        start_col = player_pieces[random_piece_index][1];

        move_count = generate_valid_moves(chessboard, start_row, start_col, piece_valid_moves);

        /* If there are valid moves for the chosen piece, break the loop */
        if (move_count > 0) {
            break;
        }

        tried_pieces_count++;
    }

    /* If no piece has a valid move, return 0 */
    if (tried_pieces_count == total_piece_count) {
        return 0;
    }

    /* Choose a random valid move and fill the random_move array */
    random_move_index = rand() % move_count;
    random_move[0] = start_row;
    random_move[1] = start_col;
    random_move[2] = piece_valid_moves[random_move_index][0];
    random_move[3] = piece_valid_moves[random_move_index][1];

    return 1;
}

/* More functions here */
