#ifndef __eckolo__
#define __eckolo__
#include <stddef.h>

typedef struct piece{
    int color1;
    int color2;
    int color3;
    bool upside_down;
    bool initialized;
}piece;

typedef struct player{
    size_t player_pieces_size;
    piece * player_pieces;
    size_t amount_of_pieces;
}player;
typedef struct node node;


//board state struct
typedef struct board{
    int xboard_size;
    int yboard_size;
    int draw_pile_size;
    piece ** pieces_on_board;
    int amount_of_players;
    piece * draw_pile;
}board;

typedef struct move{
    piece piece_to_be_played;
    int index;
    int xpos;
    int ypos;
}move;

typedef struct MovesStruct{
    int amount_of_moves;
    move* moves;
    int moves_size;
}MovesStruct;

// * FUNCTIONS * //
void checkExpandBoard(board* Board, move Move);
int drawSpecificPiece(player* Player, board* Board, int piece_index);
void createPieces(piece* Pieces);
int distributePieces(int amount_of_players, piece* Pieces, player* Players);
void initBoard(int number_of_pieces, piece* draw_pile,int amount_of_players,board* Board);
void removePiece(piece* Pieces, int index, int size);
void freeBoard(board* Board);
MovesStruct calculateMoves(piece Piece, board* Board,int index);
void addMoves(int x, int y, MovesStruct* moves, bool upsideflag, int color1, int color2, int color3, piece** board_state, int index);
//void FREEALL
bool checkCollisions(piece Piece, piece** board_state,int x,int y);
int drawPiece(player* Player, board* Board);
int playGame(int number_of_players, board* board, player* players);
void playMove(move Move, board* Board, player* Player);
#endif