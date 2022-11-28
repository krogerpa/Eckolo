
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <sodium.h>
#include "eckolo.h"


//takes a move and executes it
void playMove(move Move, board* Board, player* Player)
{
    Board->pieces_on_board[Move.xpos][Move.ypos] = Move.piece_to_be_played;
    checkExpandBoard(Board, Move);
    removePiece(Player->player_pieces,Move.index,Player->amount_of_pieces);

}
//makes board larger when piece is placed on "edge" of the board
void checkExpandBoard(board* Board, move Move)
{
    if(Move.xpos - 2 == 0)
    {
        Board->xboard_size += 5;
        piece** new_piece_matrix = malloc(sizeof(piece*) * Board->xboard_size);
        for(int i = 0; i < Board->xboard_size; i++)
        {
            new_piece_matrix[i] = calloc(Board->yboard_size, sizeof(piece));
            if(i > 4)
            {
                memcpy(new_piece_matrix[i],Board->pieces_on_board[i-5], sizeof(piece) * (Board->yboard_size - 5));
            }
        }
    for(int i = 0; i < Board->xboard_size - 5; i++)
    {
        free(Board->pieces_on_board[i]);
    }
    free(Board->pieces_on_board);
    Board->pieces_on_board = new_piece_matrix; 
  
    }

    if(Move.xpos + 2 == Board->xboard_size)
    {
        Board->xboard_size += 5;
        Board->pieces_on_board = realloc(Board->pieces_on_board, sizeof(piece*) * Board->xboard_size);
        if(!Board->pieces_on_board)
            {
                fprintf(stderr,"error reassigning memory");
                //freall();
                exit(-1);
            }
        for(int i = Board->xboard_size - 5; i < Board->xboard_size; i++)
        {
            Board->pieces_on_board[i] = calloc(Board->yboard_size, sizeof(piece));
        }

    }
    if(Move.ypos - 2 == 0)
    {
        Board->yboard_size += 5;
        piece** new_piece_matrix = malloc(sizeof(piece*) * Board->xboard_size);
        for(int i = 0; i < Board->xboard_size; i++)
        {
            new_piece_matrix[i] = calloc(Board->yboard_size, sizeof(piece));
            if(!new_piece_matrix[i])
            {
                fprintf(stderr,"error reassigning memory");
                //freall();
                exit(-1);
            }
            for(int j = 5; j < Board->yboard_size; j++)
            {
                new_piece_matrix[i][j] = Board->pieces_on_board[i][j-5];
            }
            
        }
    for(int i = 0; i < Board->xboard_size; i++)
    {   
        free(Board->pieces_on_board[i]);
    }
    free(Board->pieces_on_board);
    Board->pieces_on_board = new_piece_matrix; 
   
    }
    if(Move.ypos + 2 == Board->yboard_size)
    {
        Board->yboard_size += 5;
        for(int i = 0; i < Board->xboard_size; i++)
        {
            Board->pieces_on_board[i] = realloc(Board->pieces_on_board[i],Board->yboard_size * sizeof(piece));
            if(!Board->pieces_on_board[i])
            {
                fprintf(stderr,"error reassigning memory");
                //freall();
                exit(-1);
            }
            memset(&Board->pieces_on_board[i][Board->yboard_size - 5],0,sizeof(piece) * 5);
            
            }

    }
}

//doesnt remove the piece nor reduces the draw_pile size
int drawSpecificPiece(player* Player, board* Board, int piece_index)
{
    if(Board->draw_pile_size == 0)
    {
        return -1;
    }
    if(Player->player_pieces_size == Player->amount_of_pieces)
    {
        Player->player_pieces_size *= 2;
        Player->player_pieces = realloc(Player->player_pieces,Player->player_pieces_size * sizeof(piece));
        if(!Player->player_pieces)
        {
            fprintf(stderr,"error assigning memory");
            exit(-1);
        }

    }
    Player->player_pieces[Player->amount_of_pieces] = Board->draw_pile[piece_index];
    Player->player_pieces[Player->amount_of_pieces] = Board->draw_pile[piece_index];
    Player->amount_of_pieces++;
    removePiece(Board->draw_pile,piece_index,Board->draw_pile_size);
    Board->draw_pile_size --;
    return 0;

}

//draws a random piece
int drawPiece(player* Player, board* Board)
{
    if(Board->draw_pile_size == 0)
    {
        return -1;
    }
    uint32_t random = randombytes_uniform(Board->draw_pile_size);
    if(Player->player_pieces_size == Player->amount_of_pieces)
    {
        Player->player_pieces_size *= 2;
        Player->player_pieces = realloc(Player->player_pieces,Player->player_pieces_size * sizeof(piece));
        if(!Player->player_pieces)
        {
            fprintf(stderr,"error assigning memory");
            exit(-1);
        }

    }
    Player->player_pieces[Player->amount_of_pieces] = Board->draw_pile[random];
    Player->amount_of_pieces++;
    removePiece(Board->draw_pile,random,Board->draw_pile_size);
    Board->draw_pile_size --;
    return 0;

}
int playGame(int number_of_players, board* board, player* players)
{
    bool playing = true;

    //random game start
    while (playing)
    {
        for(int i = 0; i < number_of_players; i++)
        {
            bool played_move = false;
            for(int j = 0; j < players[i].amount_of_pieces; j++)
            {
                MovesStruct moves = calculateMoves(players[i].player_pieces[j], board,j);
                if(moves.amount_of_moves > 0)
                {
                    played_move = true;
                    playMove(moves.moves[0], board, &players[i]);
                    players[i].amount_of_pieces --;
                    free(moves.moves);
                    break;
                }
                free(moves.moves);
            }
            if(!played_move)
            {
                while(!played_move)
                {

                    int draw = drawPiece(&players[i],board);
                    if (draw == -1) return draw;
                    MovesStruct moves = calculateMoves(players[i].player_pieces[players[i].amount_of_pieces - 1], board,players[i].amount_of_pieces - 1);
                    if(moves.amount_of_moves > 0)
                    {
                        played_move = true;
                        playMove(moves.moves[0], board, &players[i]);
                        players[i].amount_of_pieces --;
                    }
                    free(moves.moves);
                }
                break;
            }
            if(players[i].amount_of_pieces == 0)
            {
                return i;
            }
        }
    }
    return -2;
}
//initializes the board given as input
void initBoard(int number_of_pieces, piece* draw_pile, int amount_of_players,board* Board)
{

    Board->amount_of_players = amount_of_players;
    Board->draw_pile = draw_pile;
    Board->draw_pile_size = number_of_pieces;
    Board->xboard_size = 10;
    Board->yboard_size = 10;

    Board->pieces_on_board = malloc(sizeof(piece*) * Board->xboard_size);
    for(int i = 0; i < Board->xboard_size; i++)
    {
        Board->pieces_on_board[i] = calloc(Board->yboard_size, sizeof(piece));
    }
    if(!Board->pieces_on_board)
    {
            fprintf(stderr, "error assigning memory");
//            freeall();
            exit(-1);
    }
//draw first piece
    uint32_t random = randombytes_uniform(Board->draw_pile_size);
    Board->pieces_on_board[5][5] = Board->draw_pile[random];
    removePiece(Board->draw_pile,random,Board->draw_pile_size);
    Board->draw_pile_size --;

}
void freeBoard(board* Board)
{

    for(int i = 0; i < Board->xboard_size; i++)
    {
        free(Board->pieces_on_board[i]);
    }
    free(Board->pieces_on_board);
    free(Board->draw_pile);
}
//Function to create all pieces
//There is 76 different pieces in the game
void createPieces(piece* Pieces)
{

    int counter = 0;
    for (int i = 0; i <= 5; i++)
    {
        Pieces[counter].color1 = i;
        Pieces[counter].color2 = i;
        Pieces[counter].color3 = i;
        Pieces[counter].upside_down = false;
        Pieces[counter].initialized = true;
        counter ++;
        for (int j = i; j <= 5; j++)
        {
            for (int k = i+1; k<=5; k++)
            {
                Pieces[counter].color1 = i;
                Pieces[counter].color2 = j;
                Pieces[counter].color3 = k;
                Pieces[counter].upside_down = false;
                Pieces[counter].initialized = true;
                counter ++;
            }
        }
    }

}

//Function to randomly distribute pieces between players
//Returns numbers of Pieces left in the draw pile
int distributePieces(int amount_of_players, piece* Pieces, player* players)
{
    uint32_t random;
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */

        exit(-1);
    }
    int number_of_pieces = 76;
    //intialize players
    for (int i = 0; i < amount_of_players;i++)
    {
        players[i].amount_of_pieces = 6;
        players[i].player_pieces_size = 6;

        players[i].player_pieces = malloc(sizeof(piece) * players[i].amount_of_pieces);

    }
    for (int i = 0; i < amount_of_players; i++)
    {
        for(int j = 0; j <= 5; j++)
        {
            random = randombytes_uniform(number_of_pieces);
            players[i].player_pieces[j] = Pieces[random];
            removePiece(Pieces,random,number_of_pieces);
            number_of_pieces --;
        }
    }
    return number_of_pieces;
}
void removePiece(piece* Pieces, int index, int size)

{
    for (int i = index; i < size - 1; i++)
    {
        Pieces[i] = Pieces[i+1];
    }
}
//calculates available moves for given piece. consistantly mixed up x and y axis. Maybe refactor. Confusing, but at least consistantly so 
MovesStruct calculateMoves(piece Piece, board* Board,int index)
{
    piece** board_state = Board->pieces_on_board;
    //init moves struct
    MovesStruct available_moves;
    available_moves.amount_of_moves = 0;
    available_moves.moves_size = 3;
    available_moves.moves = malloc(sizeof(move) * available_moves.moves_size);

    for(int i = 0; i < Board->xboard_size; i++)
    {
        for(int j = 0; j < Board->yboard_size; j++)
        {
            if (board_state[i][j].initialized != false) //if there is a piece
            {
                piece current_piece = board_state[i][j];
                bool upsideflag = true;
                bool top_flag = false;
                bool bottom_flag = false;
                bool left_flag = false;
                bool right_flag = false;
                //check for free space on the board
                if (current_piece.upside_down == true)
                {
                    upsideflag = false;
                    if (!board_state[i][j+1].initialized) right_flag = true;
                    if(!board_state[i][j-1].initialized) left_flag = true;
                    if(!board_state[i-1][j].initialized) top_flag = true;
                }
                else
                {
                    if (!board_state[i][j+1].initialized) right_flag = true;
                    if (!board_state[i][j-1].initialized) left_flag = true;
                    if (!board_state[i+1][j].initialized) bottom_flag = true;
                }
                if (top_flag)
                {
                    if (current_piece.color2 == Piece.color1 && current_piece.color3 == Piece.color3)addMoves(i-1,j,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color2 == Piece.color2 && current_piece.color3 == Piece.color1)addMoves(i-1,j,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color2 == Piece.color3 && current_piece.color3 == Piece.color2)addMoves(i-1,j,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);
                }
                if (left_flag && upsideflag)
                {
                    if (current_piece.color1 == Piece.color1 && current_piece.color2 == Piece.color3)addMoves(i,j-1,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color1 == Piece.color2 && current_piece.color2 == Piece.color1)addMoves(i,j-1,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color1 == Piece.color3 && current_piece.color2 == Piece.color2)addMoves(i,j-1,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);
                }
                if (left_flag && !upsideflag)
                {
                    if (current_piece.color1 == Piece.color3 && current_piece.color2 == Piece.color2)addMoves(i,j-1,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color1 == Piece.color1 && current_piece.color2 == Piece.color3)addMoves(i,j-1,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color1 == Piece.color2 && current_piece.color2 == Piece.color1)addMoves(i,j-1,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);

                }
                if (right_flag && upsideflag)
                {
                    if (current_piece.color2 == Piece.color2 && current_piece.color3 == Piece.color1)addMoves(i,j+1,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color2 == Piece.color3 && current_piece.color3 == Piece.color2)addMoves(i,j+1,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color2 == Piece.color1 && current_piece.color3 == Piece.color3)addMoves(i,j+1,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);
                }
                if (right_flag && !upsideflag)
                {
                    if (current_piece.color1 == Piece.color1 && current_piece.color3 == Piece.color2)addMoves(i,j+1,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color1 == Piece.color2 && current_piece.color3 == Piece.color3)addMoves(i,j+1,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color1 == Piece.color3 && current_piece.color3 == Piece.color1)addMoves(i,j+1,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);
                }
                if (bottom_flag)
                {
                    if (current_piece.color1 == Piece.color2 && current_piece.color3 == Piece.color3)addMoves(i+1,j,&available_moves,upsideflag,Piece.color1,Piece.color2,Piece.color3,board_state,index);
                    if (current_piece.color1 == Piece.color3 && current_piece.color3 == Piece.color1)addMoves(i+1,j,&available_moves,upsideflag,Piece.color2,Piece.color3,Piece.color1,board_state,index);
                    if (current_piece.color1 == Piece.color1 && current_piece.color3 == Piece.color2)addMoves(i+1,j,&available_moves,upsideflag,Piece.color3,Piece.color1,Piece.color2,board_state,index);
                }
            }
        }

    }
    return available_moves;
}
void addMoves(int x, int y , MovesStruct* moves, bool upsideflag, int color1, int color2, int color3, piece** board_state,int index)
{
    //check for color mismatches on board first

    //rotate Piece
    piece new_piece;
    new_piece.color1 = color1;
    new_piece.color2 = color2;
    new_piece.color3 = color3;
    new_piece.initialized = true;
    new_piece.upside_down = upsideflag;
    if (!checkCollisions(new_piece,board_state,x,y)) return ;
    move move;
    move.xpos = x;
    move.ypos = y;
    move.piece_to_be_played = new_piece;
    move.index = index;

    //add move to MovesStruct
    if(moves->amount_of_moves == moves->moves_size)
    {
        moves->moves_size = moves->moves_size * 2;
        moves->moves = realloc(moves->moves, moves->moves_size * sizeof(move));
        if(!moves->moves)
        {
        fprintf(stderr, "error assigning memory");
//      freeall();
        exit(-1);
        }
    }
    moves->moves[moves->amount_of_moves] = move;
    moves->amount_of_moves ++;
}
//checks for collisions --> consistantly messed up x axis vs y axis. Maybe refactor. Confusing.
bool checkCollisions(piece Piece, piece** board_state,int x, int y)
{


    if(Piece.upside_down)
    {
        //check for color1
        if(board_state[x][y-1].initialized && board_state[x][y-1].color3 != Piece.color1) return false;
        if(board_state[x+1][y-1].initialized && board_state[x+1][y-1].color3 != Piece.color1) return false;
        if(board_state[x+1][y].initialized && board_state[x+1][y].color2 != Piece.color1) return false;
        if(board_state[x+1][y+1].initialized && board_state[x+1][y+1].color2 != Piece.color1) return false;
        if(board_state[x][y+1].initialized && board_state[x][y+1].color1 != Piece.color1) return false;

        //check for color2
        if(board_state[x][y-1].initialized && board_state[x][y-1].color2 != Piece.color2) return false;
        if(board_state[x][y-2].initialized && board_state[x][y-2].color3 != Piece.color2) return false;
        if(board_state[x-1][y-2].initialized && board_state[x-1][y-2].color3 != Piece.color2) return false;
        if(board_state[x-1][y-1].initialized && board_state[x-1][y-1].color1 != Piece.color2) return false;
        if(board_state[x-1][y].initialized && board_state[x-1][y].color1 != Piece.color2) return false;

        ///check for color3
        if(board_state[x][y+1].initialized && board_state[x][y+1].color2 != Piece.color3) return false;
        if(board_state[x][y+2].initialized && board_state[x][y+2].color2 != Piece.color3) return false;
        if(board_state[x-1][y+2].initialized && board_state[x-1][y+2].color1 != Piece.color3) return false;
        if(board_state[x-1][y+1].initialized && board_state[x-1][y+1].color1 != Piece.color3) return false;
        if(board_state[x-1][y].initialized && board_state[x-1][y].color3 != Piece.color3) return false;
    }
    else
    {
        //check for color1
        if(board_state[x][y-1].initialized && board_state[x][y-1].color1 != Piece.color1) return false;
        if(board_state[x][y-2].initialized && board_state[x][y-2].color3 != Piece.color1) return false;
        if(board_state[x+1][y-2].initialized && board_state[x+1][y-2].color3 != Piece.color1) return false;
        if(board_state[x+1][y-1].initialized && board_state[x+1][y-1].color2 != Piece.color1) return false;
        if(board_state[x+1][y].initialized && board_state[x+1][y].color2 != Piece.color1) return false;

        //check for color2
        if(board_state[x][y-1].initialized && board_state[x][y-1].color3 != Piece.color2) return false;
        if(board_state[x-1][y-1].initialized && board_state[x-1][y-1].color3 != Piece.color2) return false;
        if(board_state[x-1][y].initialized && board_state[x-1][y].color1 != Piece.color2) return false;
        if(board_state[x-1][y+1].initialized && board_state[x-1][y+1].color1 != Piece.color2) return false;
        if(board_state[x][y+1].initialized && board_state[x][y+1].color2 != Piece.color2) return false;

        //check for color3
        if(board_state[x+1][y].initialized && board_state[x+1][y].color3 != Piece.color3) return false;
        if(board_state[x+1][y+1].initialized && board_state[x+1][y+1].color2 != Piece.color3) return false;
        if(board_state[x+1][y+2].initialized && board_state[x+1][y+2].color2 != Piece.color3) return false;
        if(board_state[x][y+2].initialized && board_state[x][y+2].color1 != Piece.color3) return false;
        if(board_state[x][y+1].initialized && board_state[x][y+1].color1 != Piece.color3) return false;



    }
    return true;
}
