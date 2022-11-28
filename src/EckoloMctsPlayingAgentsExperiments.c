#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sodium.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include "eckolo.h"
#include "searchMethods.h"

    //use older glibc version
int mctsEptVsRandom(gamestate* current_state,int which_eval_function)
{
    int result;
    
    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveEPT(current_state,which_eval_function);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int monteWithChanceNodesVsRandom(gamestate* current_state)
{
    int result;
    
    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) 
                {
                    freeState(current_state);
                    return draw;
                }
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveWithChanceNodes(current_state);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int monteWithChanceNodesWithVariableRuntimeVsRandom(gamestate* current_state, int counter)
{
    int result;
    
    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) 
                {
                    freeState(current_state);
                    return draw;
                }
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveWithChanceNodesVariableRuntime(current_state, counter);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int RandomVsRandom(gamestate* current_state)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }
        if(current_state->whose_turn == 0)
        {
            printf("-------------------------\n\n");
        }
        printf("Whose turn: %d, ",current_state->whose_turn);
//        for(int  i = 0; i < current_state->board->amount_of_players; i++)
//        {
//        printf(" P%d: %d", i+1, current_state->players[i].amount_of_pieces);
//        }
        printf("\n");
        if(current_state->whose_turn == 0)
        {
            playRandomMove(current_state);
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
        
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsVsRandomWithDifferentUCTValues(gamestate* current_state,double constant)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveVariableConstant(current_state, constant);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsEPTVsMctsEPTChance(gamestate* current_state,int which_eval_function)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveEPT(current_state, which_eval_function);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            new_state = findNextMoveEPTChance(current_state,1,1);
            freeState(current_state);
            current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}


int mctsEPTVsMcts(gamestate* current_state,int which_eval_function)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveEPT(current_state, which_eval_function);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            new_state = findNextMove(current_state);
            freeState(current_state);
            current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsEPTChanceVsMctsChance(gamestate* current_state,int which_eval_function)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveEPTChance(current_state, 1, 10);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            new_state = findNextMoveWithChanceNodes(current_state);
            freeState(current_state);
            current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsEPTChanceVsRandom(gamestate* current_state,int which_eval_function, int max_counter)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveEPTChance(current_state, which_eval_function, max_counter);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsChancevsMcts(gamestate* current_state)
{
    int result;

    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawPiece(&current_state->players[current_state->whose_turn],current_state->board);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveWithChanceNodes(current_state);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            new_state = findNextMove(current_state);
            freeState(current_state);
            current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
}
int mctsVsRandomWithDeterminedDraw(gamestate* current_state, int max_counter)
{
   int result;
    
    gamestate* new_state;
    //random game start
    while (current_state->winning == -2)
    {
        bool possible_move = false;
        //draw first if necessary
        for(int i = 0; i < current_state->players[current_state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[i],current_state->board,i);
            if(moves.amount_of_moves > 0)

            {
                free(moves.moves);
                possible_move = true;
                break;
            }
            free(moves.moves);
        }
        if(!possible_move)
        {
            while(!possible_move)
            {

                int draw = drawSpecificPiece(&current_state->players[current_state->whose_turn],current_state->board,current_state->board->draw_pile_size - 1);
                if (draw == -1) return draw;
                MovesStruct moves = calculateMoves(current_state->players[current_state->whose_turn].player_pieces[current_state->players[current_state->whose_turn].amount_of_pieces - 1], current_state->board,current_state->players[current_state->whose_turn].amount_of_pieces - 1);
                if(moves.amount_of_moves > 0)
                {
                    possible_move = true;
                }
                free(moves.moves);
            }
                                 
        }

        if(current_state->whose_turn == 0)
        {
            new_state = findNextMoveWithDeterminedDraw(current_state, max_counter);
            freeState(current_state);
            current_state = new_state;
        }
        else
        {
            playRandomMove(current_state);
            // new_state = findNextMove(current_state);
            // freeState(current_state);
            // current_state = new_state;
        }
    }
    result = current_state->winning;


    freeState(current_state);
    return result;
   
}

void randomVsRandomExperiment()
{    
    int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int draw_number = 0;
        int player1 = 0;
        int player2 = 0;
        int player3 = 0;
        int player4 = 0;
        int player5 = 0;
        int player6 = 0;
        int player7 = 0;
        int player8 = 0;

        for(int i = 0; i < 10000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            int result = RandomVsRandom(current_gamestate);
            switch(result)
            {
            case 0: player1++; break;
            case 1: player2++; break;
            case 2: player3++; break;
            case 3: player4++; break;
            case 4: player5++; break;
            case 5: player6++; break;
            case 6: player7++; break;
            case 7: player8++; break;
            default: draw_number++; break;
            }
            printf("The intermediate_result is : %d step: %d score: Random: %d, Random: %d \n",result,i,player1,player2);

            
        }
        printf("\nResults are:Draws:%d, Random1 %d, Random2: %d, Random3: %d, Random4: %d, Random5 %d, Random6: %d, Random7: %d, Random8: %d\n\n",draw_number, player1,player2,player3,player4,player5,player6,player7,player8);
        printf("-------------------------------------------------\n\n");
    }
}
void randomVsRandomGameTreeSize()
{    
    int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int draw_number = 0;
        int player1 = 0;
        int player2 = 0;
        int player3 = 0;
        int player4 = 0;
        int player5 = 0;
        int player6 = 0;
        int player7 = 0;
        int player8 = 0;
        
        int amount_of_moves = 0;
        int amount_of_draws = 0;
        int amount_of_draw_events = 0;
        int amount_of_move_events = 0;
        int gamelength = 0;
        for(int i = 0; i < 10000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            int result = 0;
            while(current_gamestate->winning == -2)
            {
                bool possible_move = false;
        //draw first if necessary
                for(int i = 0; i < current_gamestate->players[current_gamestate->whose_turn].amount_of_pieces; i++)
                {
                    MovesStruct moves = calculateMoves(current_gamestate->players[current_gamestate->whose_turn].player_pieces[i],current_gamestate->board,i);
                    if(moves.amount_of_moves > 0)

                    {
                        amount_of_moves += moves.amount_of_moves;
                        possible_move = true;      
                    }
                free(moves.moves);
                }
                if(!possible_move)
                {
                    while(!possible_move)
                    {
                        amount_of_draws += current_gamestate->board->draw_pile_size;
                        amount_of_draw_events ++;
                        int draw = drawPiece(&current_gamestate->players[current_gamestate->whose_turn],current_gamestate->board);
                        if (draw == -1)
                        {
                            current_gamestate->winning = -1;
                            break;
                        }
                        MovesStruct moves = calculateMoves(current_gamestate->players[current_gamestate->whose_turn].player_pieces[current_gamestate->players[current_gamestate->whose_turn].amount_of_pieces - 1], current_gamestate->board,current_gamestate->players[current_gamestate->whose_turn].amount_of_pieces - 1);
                        if(moves.amount_of_moves > 0)
                        {
                            possible_move = true;
                        }
                        free(moves.moves);

                    }
                                 
                }
                else
                {
                    amount_of_move_events ++;
                    playRandomMove(current_gamestate);
                }
            }
            switch(result)
            {
            case 0: player1++; break;
            case 1: player2++; break;
            case 2: player3++; break;
            case 3: player4++; break;
            case 4: player5++; break;
            case 5: player6++; break;
            case 6: player7++; break;
            case 7: player8++; break;
            default: draw_number++; break;
            }


            
        }
        printf("\nResults are:Draw_amount:%d, Draw_event_amount %d, Move_amount: %d, Move_event_amount: %d\n\n",amount_of_draws,amount_of_draw_events,amount_of_moves,amount_of_move_events);
        printf("-------------------------------------------------\n\n");
    }
}
void mctsVsMctsEpt()
{
    int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 1000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsEPTVsMcts(current_gamestate,1);
            if(result == 0) player1++;
            if(result == 1) player2++;
            printf("hi %d\n",i);
            
        }
        printf("\nResults are: MctsEPT %d, Mcts: %d:\n\n",player1,player2);
        printf("-------------------------------------------------\n\n");
    }
}

void mctsEPTChanceVsMctsEpt()
{
   int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 500; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsEPTVsMctsEPTChance(current_gamestate,1);
            if(result == 0) player1++;
            if(result == 1) player2++;
            
        }
        printf("\nResults are: MctsEPT %d, MctsEPTChance: %d:\n\n",player1,player2);
        printf("-------------------------------------------------\n\n");
    }
}
void mctsChanceVsMctsExperiment()
{
   int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 1000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsChancevsMcts(current_gamestate);
            if(result == 0) player1++;
            if(result == 1) player2++;
            printf("hi %d \n", i);


        }
        printf("\nResults are: MctsChance %d, Mcts: %d:\n\n",player1,player2);
        printf("-------------------------------------------------\n\n");
    }
}


void mctsEPTChanceVsmctsChance()
{
   int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 500; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsEPTChanceVsMctsChance(current_gamestate,1);
            if(result == 0) player1++;
            if(result == 1) player2++;
            printf("hi %d \n",i);

            
        }
        printf("\nResults are: MctsEPTChance %d, MctsChance: %d:\n\n",player1,player2);
        printf("-------------------------------------------------\n\n");
    }
}

void mctsChanceVSRandom()
{
    //use older glibc version

    
    int player_number = 2;
    int whose_turn = 0;
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 1000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = monteWithChanceNodesVsRandom(current_gamestate);
            if(result == 0) player1++;
            if(result == 1) player2++;
            printf("The intermediate_result is : %d step: %d score: MCTSChance: %d, Random: %d \n",result,i,player1,player2);

            
        }
        printf("\nResults are: MCTSChance %d, Random: %d\n\n",player1,player2);
        printf("-------------------------------------------------\n\n");
    }
}
void mctsChanceWithVariableRuntimeCounterVSRandom()
{
    //use older glibc version

    
    int player_number = 2;
    int whose_turn = 0;
    int counter[] = {100};
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 500; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = monteWithChanceNodesWithVariableRuntimeVsRandom(current_gamestate,counter[j]);
            if(result == 0) player1++;
            if(result == 1) player2++;
            printf("The intermediate_result is : %d step: %d score: Mcts: %d, Random: %d \n",result,i,player1,player2);

            
        }
        printf("\nResults are: MCTSChance %d, Random: %d, number of iterations: %d\n\n",player1,player2,counter[j]);
        printf("-------------------------------------------------\n\n");
    }
}

void mctsEPTWithVariableRuntimeCounterVSRandom()
{
    //use older glibc version

    
    int player_number = 2;
    int whose_turn = 0;
    int counter[] = {2,10,100,500,1000,2000};
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 1; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 5; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsEptVsRandom(current_gamestate,counter[j]);
            if(result == 0) player1++;
            if(result == 1) player2++;
            //printf("The intermediate_result is : %d step: %d score: MctsEPT: %d, Random: %d \n",result,i,player1,player2);



            
        }
        printf("\nResults are: MCTSChance %d, Random: %d, Counter: %d\n\n",player1,player2,counter[j]);
        printf("-------------------------------------------------\n\n");
    }
}

void mctsEPTChanceWithVariableRuntimeCounterVSRandom()
{
    //use older glibc version

    
    int player_number = 2;
    int whose_turn = 0;
    int counter[] = {2,10,100,500,1000,2000};
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 6; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 1000; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsEPTChanceVsRandom(current_gamestate,1,counter[j]);
            if(result == 0) player1++;
            if(result == 1) player2++;
            //printf("The intermediate_result is : %d step: %d score: Mcts: %d, Random: %d \n",result,i,player1,player2);



            
        }
        printf("\nResults are: MCTSChanceEPT %d, Random: %d, Runtime: %d\n\n",player1,player2,counter[j]);
        printf("-------------------------------------------------\n\n");
    }
}

void mctsVSRandomWithDeterminedDrawExperiment()
{
        int player_number = 2;
    int whose_turn = 0;
    int counter [] = {1,10,100,500,1000,2000,4000};
    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 7; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 100; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            shuffleDrawList(current_gamestate->board);
            int result = mctsVsRandomWithDeterminedDraw(current_gamestate,counter[j]);
            if(result == 0) player1++;
            if(result == 1) player2++;

            
        }
        printf("\nResults are: MCTS %d, Random: %d, runtime: %d \n\n",player1,player2,counter[j]);
        printf("-------------------------------------------------\n\n");
    }
}
void mctsVsRandomWithVariableConstantForUCT()
{
   int player_number = 2;
    int whose_turn = 0;
    double constant[] = {1,10,100,500,1000,2000};

    //simulate 2000 games for all mcts eval functions vs random player
    for(int j = 0; j < 6; j++)
    {
        int player1 = 0;
        int player2 = 0;
        for(int i = 0; i < 1; i++)
        {
            board* Board = malloc(sizeof(board));
            player* players = malloc(sizeof(player) * player_number);
            piece* pieces = malloc(sizeof(piece) * 76);
            createPieces(pieces);
            int number_of_pieces = distributePieces(player_number,pieces,players);
            initBoard(number_of_pieces, pieces, player_number,Board);
            gamestate* current_gamestate = malloc(sizeof(gamestate));
            current_gamestate->board = Board;
            current_gamestate->players = players;
            current_gamestate->whose_turn = whose_turn;
            current_gamestate->winning = -2;
            whose_turn++;
            if(whose_turn > 1) whose_turn = 0;
            int result = mctsVsRandomWithDifferentUCTValues(current_gamestate, constant[j]);
            if(result == 0) player1++;
            if(result == 1) player2++;

            
        }
        
        printf("\nResults are: Mcts %d, Random: %d:,Iterations %f\n\n ",player1,player2,constant[j]);
        printf("-------------------------------------------------\n\n");
}
}
int main(int argc, const char*argv[]){
mctsVsMctsEpt();
}