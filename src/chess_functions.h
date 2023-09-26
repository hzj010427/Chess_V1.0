#ifndef CHESS_FUNCTIONS_H
#define CHESS_FUNCTIONS_H

#include <stdbool.h>
#include "constants.h"
#include "board.h"

/**
 * A function to move a piece on the board
 *  
 * @param chessboard The board onto which the pieces are being added
 * @param move The move to be made
 * @param turn The color of the player making the move
 * @return 1 if the move was successful, 0 otherwise
 * @note 
 * The move_piece function moves a chess piece on the chessboard based on the provided move string (e.g., "A2A4") and the current player's turn. 
 * It first converts the move string into row and column numbers. 
 * Then, it checks if the move is valid using the is_valid_move function. 
 * If the move is valid, the function proceeds with the following steps:
 * If the move is a castling move, it moves the rook accordingly.
 * If the move is an en passant capture, it removes the captured pawn.
 * If the move is a pawn's double move, it updates the en_passant array.
 * If the move results in a pawn promotion, it promotes the pawn to the selected piece.
 * Finally, it moves the piece to the target square, increments its movement counter, and empties the source square.
 */  

int move_piece(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char *move, char turn);

/**
 * A function to generate all possible moves for a specific piece
 * 
 * @param chessboard The board onto which the pieces are being added
 * @param row The row of the piece
 * @param col The column of the piece
 * @param valid_moves The array of valid moves
 * @return The number of moves
 * @note
 * The generate_all_moves function generates all legal moves for a given chess piece based on the provided chessboard and piece position (row and col). 
 * It stores the resulting moves in a 2D array called valid_moves and returns the number of valid moves. 
 * The function differentiates between different chess pieces (such as pawn, rook, knight, bishop, queen, and king) using a switch statement and calculates possible moves based on the piece type.
 */
int generate_all_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int row, int col, int (*valid_moves)[2]);

/**
 * A function to generate all valid moves for a piece considering check
 * 
 * @param chessboard The reference board
 * @param row The row of the piece
 * @param col The column of the piece
 * @param valid_moves The array of valid moves
 * @return The number of valid moves
 * @note 
 * The generate_valid_moves function generates valid moves for a given chess piece based on the provided chessboard and piece position (row and col). 
 * It first calls the generate_all_moves function to get all the possible moves for the piece. 
 * Then, it checks if any of these moves would put the king in check (an invalid move). 
 * If so, it removes that move from the list of valid moves. Finally, it returns the updated number of valid moves. 
 * The function uses a temporary chessboard to check the moves' validity without affecting the original chessboard state.
 */
int generate_valid_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int row, int col, int (*valid_moves)[2]);

/**
 * A function to check if a move is valid
 * 
 * @param chessboard The reference board
 * @param from_row The row of the source
 * @param from_col The column of the source
 * @param to_row The row of the destination
 * @param to_col The column of the destination
 * @param turn The color of the player making the move
 * @return 1 if the move is valid, 0 otherwise
 * @note
 * The function first checks for basic conditions like empty starting square, out of bounds move, move to the same square, and correct turn. 
 * Then, it checks for castling moves using the is_castling_move function. 
 * Next, it calls the generate_valid_moves function to obtain a list of valid moves for the piece.
 * Finally, it checks if the intended move is present in the list of valid moves. 
 * If it is, the function returns 1 (indicating a valid move), and if not, it returns 0 (indicating an invalid move).
 */
int is_valid_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, char turn);

/**
 * A function to check if a square is valid
 * 
 * @param row The row of the square
 * @param col The column of the square
 * @return 1 if the square is valid, 0 otherwise
 */
int is_valid_square(int row, int col);

/**
 * A function to add a single move to the array of valid moves
 * 
 * @param chessboard The reference board
 * @param from_row The row of the source
 * @param from_col The column of the source
 * @param to_row The row of the destination
 * @param to_col The column of the destination
 * @param valid_moves The array of valid moves
 * @param turn The color of the player making the move
 * @return The number of valid moves
 * @note
 * The add_single_move function checks if a move from the source square (from_row, from_col) to the destination square (to_row, to_col) is valid, 
 * and if so, adds the move to the valid_moves array. It also updates the move_count accordingly.
 */
int add_single_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, int (*valid_moves)[2], int move_count);

/**
 * A function to add a line of moves to the array of valid moves
 * 
 * @param chessboard The reference board
 * @param from_row The row of the source
 * @param from_col The column of the source
 * @param dr The row offset
 * @param dc The column offset
 * @param valid_moves The array of valid moves
 * @param turn The color of the player making the move
 * @return The number of valid moves
 * @note
 * The add_line_moves function calculates and adds valid moves in a straight line for a given piece on the chessboard. 
 * The function updates the destination square by adding the direction offsets (dr, dc) in each iteration. 
 * Finally, it returns the updated move_count.
 */
int add_line_moves(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int dr, int dc, int (*valid_moves)[2], int move_count);

/**
 * A function to check if a move satisfies the conditions for castling
 * 
 * @param chessboard The reference board
 * @param from_row The row of the source
 * @param from_col The column of the source
 * @param to_row The row of the destination
 * @param to_col The column of the destination
 * @param player The color of the player making the move
 * @return 1 if the move is a castling move, 0 otherwise
 */
int is_castling_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], int from_row, int from_col, int to_row, int to_col, char player);

/**
 * A function to check if king is in check
 * 
 * @param chessboard The reference board
 * @param player The color of the player
 * @return 1 if the king is in check, 0 otherwise
 */
int is_check(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player);

/**
 * A function to check if king is in checkmate
 * 
 * @param chessboard The reference board
 * @param player The color of the player
 * @return 1 if the king is in checkmate, 0 otherwise
 */
int is_checkmate(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player);

/**
 * A function to check if king is in stalemate
 * 
 * @param chessboard The reference board
 * @param player The color of the player
 * @return 1 if the king is in stalemate, 0 otherwise
 */
int is_stalemate(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char player);

/* returns 1 if a random move was generated, 0 otherwise */
int generate_random_move(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn, int *random_move);

void get_random_hint(Piece chessboard[BOARD_SIZE][BOARD_SIZE], char turn);

/* More functions here */

#endif

/* EOF */
