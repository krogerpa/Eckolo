#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sodium.h>
#include <float.h>
#include <math.h>
#include "eckolo.h"
#include "searchMethods.h"
//gametree search am beispiel von .. .
//include time
//need to free things
char* connectPython(pythonPiece* Boardstate,pythonPiece* drawpile, pythonPiece* player_pieces,int size_of_boardstate,int size_of_drawpile, int* amount_of_pieces,int amount_of_players)
{

    //translate board
    board* Board = malloc(sizeof(board));
    Board->amount_of_players = 2;
    Board->pieces_on_board = malloc(sizeof(piece*) * 156);
    for(int i = 0; i < 156; i++)
    {
        Board->pieces_on_board[i] = calloc(156, sizeof(piece));
    }
    if(!Board->pieces_on_board)
    {

            fprintf(stderr, "error assigning memory");
//            freeall();
            exit(-1);
    }
    for (int i = 0; i < size_of_boardstate; i++)
    {
        piece tempPiece;
        tempPiece.color1 = Boardstate[i].color1;
        tempPiece.color2 = Boardstate[i].color2;
        tempPiece.color3 = Boardstate[i].color3;
        tempPiece.upside_down = Boardstate[i].upside_down;
        tempPiece.initialized = true;
        Board->pieces_on_board[Boardstate[i].xpos][Boardstate[i].ypos] = tempPiece;
    }
    //translate draw_pile
    piece* new_draw_pile = malloc(sizeof(piece) * size_of_drawpile);
    for(int i = 0; i < size_of_drawpile; i++)
    {
        piece tempPiece;
        tempPiece.color1 = drawpile[i].color1;
        tempPiece.color2 = drawpile[i].color2;
        tempPiece.color3 = drawpile[i].color3;
        tempPiece.upside_down = drawpile[i].upside_down;
        tempPiece.initialized = true;
        new_draw_pile[i] = tempPiece;
    }
    Board->draw_pile = new_draw_pile;
    Board->draw_pile_size = size_of_drawpile;
    //translate player pieces
    player* players = malloc(sizeof(player) * amount_of_players);
    int counter = 0;
    for(int i = 0; i < amount_of_players; i++ )
    {
        players[i].amount_of_pieces = amount_of_pieces[i];
        players[i].player_pieces_size = players[i].amount_of_pieces;
        players[i].player_pieces = malloc(sizeof(piece) * players[i].amount_of_pieces);
        for (int j = 0; j < players[i].amount_of_pieces; j++)
        {
            piece tempPiece;
            tempPiece.color1 = player_pieces[counter].color1;
            tempPiece.color2 = player_pieces[counter].color2;
            tempPiece.color3 = player_pieces[counter].color3;
            tempPiece.upside_down = player_pieces[counter].upside_down;
            tempPiece.initialized = true;
            players[i].player_pieces[j] = tempPiece;
            counter++;
        }
    }
    gamestate* current_gamestate = malloc(sizeof(gamestate));
    current_gamestate->board = Board;
    current_gamestate->players = players;
    //TODO change for multiple players
    current_gamestate->whose_turn = 0;
    current_gamestate->winning = -2;
    char* return_string = findNextMovePython(current_gamestate);

    return return_string;

}
char* findNextMovePython(gamestate* current_state)
{


    int whose_turn = current_state->whose_turn;
    board* boardstate = current_state->board;
    player* players = current_state->players;
    //create new tree
    node* root_node = malloc(sizeof(node));
    createNewNode(boardstate,players,whose_turn,root_node);
    root_node->node_score = 0;
    root_node->visits = 0;
    root_node->firstChild = NULL;
    root_node->nextSibling = NULL;
    root_node->parent = NULL;
    int result;
    time_t start_time = time(&start_time);

    time_t current_time = time(&current_time);
    int counter = 0;

    while((current_time-start_time) < 2) //TODO END TIME VARIABLE
    {
        node* next_node = selectNode(root_node);
        if(next_node->state->winning == -2)
        {
            expand(next_node);
        }
        if(next_node->firstChild)
        {
            next_node = next_node->firstChild;
        }
        if(next_node->state->winning == -2)
        {
        result = simulatePlayout(next_node);
        }
        else
        {
        result = next_node->state->winning;
        }
        if (result == -1) backPropagation(next_node, next_node->state->whose_turn, 0.5);
        else if (result != next_node->state->whose_turn) backPropagation(next_node, next_node->state->whose_turn, 1);
        else backPropagation(next_node, next_node->state->whose_turn, 0);
        current_time = time(&current_time);
        counter++;
    } 

    //this is a little bit hacky ...
    char* return_string = createReturnStringForPython(root_node);
    // gamestate* next_move = getMaxChild(root_node)->state;
    // gamestate* new_gamestate = copyGameState(next_move);

    freeTree(root_node);
    return return_string;

}

char * createReturnStringForPython(node* root_node)
{
    char* return_string = malloc(sizeof(char) * 156); //arbitrary large size
    node* max_node = getMaxChild(root_node);
    int counter = 0;
    node* temp_node = root_node->firstChild;
    while(temp_node != max_node)
    {
        temp_node = temp_node->nextSibling;
        counter++;
    }
    int movesCounter = 0;
    for(int i = 0; i < root_node->state->players[root_node->state->whose_turn].amount_of_pieces; i++)
    {
        MovesStruct moves = calculateMoves(root_node->state->players[root_node->state->whose_turn].player_pieces[i],root_node->state->board,i);
        if (moves.amount_of_moves > 0)
        {
            for (int j = 0; j < moves.amount_of_moves; j++)
            {
                if(movesCounter == counter)
                {

                    int len = sprintf(return_string, "%d",i); //index first
                    return_string[len] = '|'; //split character
                    int len2 = sprintf(&return_string[len+1],"%d",j); //move index
                    return_string[len + len2 + 1] = '\0';
                }
                    movesCounter ++;
            }


        }
    }
    return return_string;
}
void freeState(gamestate* state_to_not_be)
{
    for(int j = 0; j < state_to_not_be->board->amount_of_players; j++)
    {
        free(state_to_not_be->players[j].player_pieces);
    }
    freeBoard(state_to_not_be->board);
    free(state_to_not_be->board);
    free(state_to_not_be->players);
    free(state_to_not_be);

}
//frees all memory a node and associated structs are using
void freeNode(node* node_to_not_be)
{
    freeState(node_to_not_be->state);
    free(node_to_not_be);
}
void freeTree(node* root_to_not_be)
{
    if(root_to_not_be->firstChild) freeTree(root_to_not_be->firstChild);
    if(root_to_not_be->nextSibling) freeTree(root_to_not_be->nextSibling);
    freeNode(root_to_not_be);
}
//returns node with max score
node* getMaxChild(node* root_node)
{
    if(!root_node->firstChild)
    {
        return root_node;
    }

    node* temp_node = root_node->firstChild;
    double max_val = temp_node->node_score;    
    node* current_max_node = temp_node;
    while(temp_node->nextSibling)
    {
        temp_node = temp_node->nextSibling;
        if(temp_node->node_score > max_val)
        {
            max_val = temp_node->node_score;
            current_max_node = temp_node;
        }
    }
    return current_max_node;

    
}
//returns UCT Score
double getUCTScore(int total_visits, double node_score, int node_visits)
{
    if (node_visits == 0) return DBL_MAX;

    return (node_score / (double)node_visits) + 1.2 * sqrt((double)log(total_visits) / node_visits);
    //parameter?

}
//checks whether a player has won or there is a draw
//returns -1 for draw, player index of winner otherwise -2 if game is still ongoing
int checkWin(board* Boardstate, player* players)
{
    for(int i = 0; i < Boardstate->amount_of_players; i++)
    {
        if(players[i].amount_of_pieces == 0)
        {
            return i;
        }
    }
    if(Boardstate->draw_pile_size == 0) return -1;
    return -2;
}
//selects next node to be searched based on UCT Score
node* selectNode(node* base_node)
{
    if(base_node->isChanceNode)
    {
        //count chance nodes
        node* temp_node = base_node;
        int counter = 1;
        while(temp_node->nextSibling)
        {
            temp_node = temp_node->nextSibling;
            counter++;
        }
        //generate random number based on count
        uint32_t random = randombytes_uniform(counter);
        counter = 0;
        temp_node = base_node;
        //iterate through chance nodes until randomly generated number is reached
        while(counter != random)
        {
            temp_node = temp_node->nextSibling;
            counter ++;
        }
        if(!temp_node->firstChild)
        {
            fprintf(stderr, "error chance node no child");
            exit(-1);
        }
        return selectNode(temp_node->firstChild);

    }
    node* temp_node = base_node;
    double uct_score = 0;
    node* max_node = base_node;
    while (temp_node->nextSibling)
    {
        double temp_uct_score;
        temp_uct_score = getUCTScore(temp_node->parent->visits, temp_node->node_score, temp_node->visits);
        if (temp_uct_score > uct_score)
        {
            uct_score = temp_uct_score;
            max_node = temp_node;
        }
        temp_node = temp_node->nextSibling;
        if(!temp_node->nextSibling)
        {
            temp_uct_score = getUCTScore(temp_node->parent->visits, temp_node->node_score, temp_node->visits);
            if (temp_uct_score > uct_score)
            {
                uct_score = temp_uct_score;
                max_node = temp_node;
            }
        }
    }
    if(max_node->firstChild)
    {
        max_node = selectNode(max_node->firstChild);
    }
    return max_node;
}
//INPUT
//Current boardstate, current players, whose_turn it is as int
//OUTPUT
//Result of randomized playout corresponding to player number
//-1 if draw
int simulatePlayout(node* current_node)
{

    node * temp_node = malloc(sizeof(node));
    createNewNode(current_node->state->board,current_node->state->players,current_node->state->whose_turn,temp_node);
    board* boardstate = temp_node->state->board;
    player* players = temp_node->state->players;
    bool playing = true;
    int i = current_node->state->whose_turn;  

    //random game start
    while (playing)
    {
        for(; i < boardstate->amount_of_players; i++)
        {   
            bool played_move = false;
            for(int j = 0; j < players[i].amount_of_pieces; j++)
            {
                MovesStruct moves = calculateMoves(players[i].player_pieces[j], boardstate,j);
                if(moves.amount_of_moves > 0)
                {
                    played_move = true;
                    uint32_t random = randombytes_uniform(moves.amount_of_moves);
                    playMove(moves.moves[random], boardstate, &players[i]);
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

                    int draw = drawPiece(&players[i],boardstate);
                    if (draw == -1)
                    {
                        freeNode(temp_node);
                      return draw;  
                    } 
                    MovesStruct moves = calculateMoves(players[i].player_pieces[players[i].amount_of_pieces - 1], boardstate,players[i].amount_of_pieces - 1);
                    if(moves.amount_of_moves > 0)
                    {
                        played_move = true;
                        playMove(moves.moves[0], boardstate, &players[i]);
                        players[i].amount_of_pieces --;
                    }
                    free(moves.moves);
                }
                
            }
            if(players[i].amount_of_pieces == 0)
            {
                freeNode(temp_node);
                return i;
            }
        } 
        i = 0;
    }
    return -1;
}
//right now determining drawing outcomes immediately -- might change to chance nodes later
void expand(node* chosen_one)
{
    if(chosen_one->state->winning != -2) return;
    bool child_flag = false; //flag to set first_child
    //calculate moves from board state
    //create node for each move
    
    board* current_board  = chosen_one->state->board;
    player* players = chosen_one->state->players;
    int whose_turn = chosen_one->state->whose_turn;
    node* last_created;
    for (int i = 0; i < players[whose_turn].amount_of_pieces; i++)
    {
        MovesStruct moves = calculateMoves(players[whose_turn].player_pieces[i],current_board,i);
        if (moves.amount_of_moves > 0)
        {
            for(int j = 0; j < moves.amount_of_moves; j++)
            {
                node* temp_node = malloc(sizeof(node));
                createNewNode(current_board,players,whose_turn,temp_node);
                
                //create new node
                temp_node->parent = chosen_one;
                temp_node->visits = 0;
                temp_node->node_score = 0;
                temp_node->state->whose_turn ++;
                if (temp_node->state->whose_turn == temp_node->state->board->amount_of_players) temp_node->state->whose_turn = 0;
                playMove(moves.moves[j],temp_node->state->board,&temp_node->state->players[whose_turn]);
                temp_node->state->players[whose_turn].amount_of_pieces --;
                temp_node->state->winning = checkWin(temp_node->state->board,temp_node->state->players);
                if(temp_node->state->winning != -2 && temp_node->state->winning == whose_turn)
                {
                    chosen_one->node_score = -DBL_MAX;
                }


                //if first node to be created
                if(!child_flag)
                {
                    child_flag = true;
                    chosen_one->firstChild = temp_node;
                    last_created = temp_node;
                }
                else
                {
                    last_created->nextSibling = temp_node;
                    last_created = temp_node;
                }

            }
        }
        free(moves.moves);
    }
    //if no moves draw immediately - could also create node for each draw if wanted -->see chance nodes

    if(!child_flag)
    {
        node* new_node = malloc(sizeof(node));
        createNewNode(current_board,players,whose_turn,new_node);
        chosen_one->firstChild = new_node;
        new_node->parent = chosen_one;
        new_node->visits = 0;
        new_node->node_score = 0;
        current_board = new_node->state->board;
        players = new_node->state->players;
        bool expanded = false;
        while(!expanded)
        {
            int draw = drawPiece(&players[whose_turn],current_board);
           if (draw == -1)
           {
               new_node->state->winning = checkWin(new_node->state->board,new_node->state->players);
               break;
           } //TODO CHECK IF THIS IS RIGHT
           MovesStruct moves = calculateMoves(players[whose_turn].player_pieces[players[whose_turn].amount_of_pieces - 1],current_board,players[whose_turn].amount_of_pieces-1);
           if(moves.amount_of_moves > 0)
           {
               for(int i = 0; i < moves.amount_of_moves; i++)
                {
                node* temp_node = malloc(sizeof(node)); 
                createNewNode(current_board,players,whose_turn,temp_node);
                
                //create new node
                temp_node->parent = new_node;
                temp_node->visits = 0;
                temp_node->node_score = 0;
                playMove(moves.moves[i],temp_node->state->board,&temp_node->state->players[whose_turn]);
                temp_node->state->players[whose_turn].amount_of_pieces --;
                temp_node->state->whose_turn ++;
                temp_node->state->winning = checkWin(temp_node->state->board,temp_node->state->players);
                if (temp_node->state->whose_turn == temp_node->state->board->amount_of_players) temp_node->state->whose_turn = 0;
                //if first node to be created
                if(!child_flag)
                {
                    child_flag = true;
                    new_node->firstChild = temp_node;
                    last_created = temp_node;
                }
                else
                {
                    last_created->nextSibling = temp_node;
                    last_created = temp_node;
                }                
                }
                expanded = true;
           }
           free(moves.moves);

        }

    }

}
void expandWithChanceNodes(node* chosen_one)
{
    if(chosen_one->state->winning != -2) return;
    bool child_flag = false; //flag to set first_child
    //calculate moves from board state
    //create node for each move
    
    board* current_board  = chosen_one->state->board;
    player* players = chosen_one->state->players;
    int whose_turn = chosen_one->state->whose_turn;
    node* last_created;
    for (int i = 0; i < players[whose_turn].amount_of_pieces; i++)
    {
        MovesStruct moves = calculateMoves(players[whose_turn].player_pieces[i],current_board,i);
        if (moves.amount_of_moves > 0)
        {
            for(int j = 0; j < moves.amount_of_moves; j++)
            {
                node* temp_node = malloc(sizeof(node));
                createNewNode(current_board,players,whose_turn,temp_node);
                
                //create new node
                temp_node->parent = chosen_one;
                temp_node->visits = 0;
                temp_node->node_score = 0;
                temp_node->state->whose_turn ++;
                if (temp_node->state->whose_turn == temp_node->state->board->amount_of_players) temp_node->state->whose_turn = 0;
                playMove(moves.moves[j],temp_node->state->board,&temp_node->state->players[whose_turn]);
                temp_node->state->players[whose_turn].amount_of_pieces --;
                temp_node->state->winning = checkWin(temp_node->state->board,temp_node->state->players);
                if(temp_node->state->winning != -2 && temp_node->state->winning == whose_turn)
                {
                    chosen_one->node_score = -DBL_MAX;
                }


                //if first node to be created
                if(!child_flag)
                {
                    child_flag = true;
                    chosen_one->firstChild = temp_node;
                    last_created = temp_node;
                }
                else
                {
                    last_created->nextSibling = temp_node;
                    last_created = temp_node;
                }

            }
        }
        free(moves.moves);
    }
    //if no moves draw immediately - one node beeing created for each draw outcome

    if(!child_flag)
    {
        if(chosen_one->state->board->draw_pile_size == 0)
        {
            chosen_one->state->winning == -1;
            return;
        }
        createChanceNodes(chosen_one);
    }

}
//takes node for which chance nodes are to be created and creats chance nodes
void createChanceNodes(node* chosen_one)
{
    board* current_board = chosen_one->state->board;
    player* current_players = chosen_one->state->players;
    int whose_turn = chosen_one->state->whose_turn;
    bool child_created = false;   
    node* last_created;
    for(int i  = 0; i < current_board->draw_pile_size;i++)
    {
        node* new_node = malloc(sizeof(node));
        createNewNode(current_board,current_players,whose_turn,new_node);
        if (!child_created)
        {
            child_created = true;
            chosen_one->firstChild = new_node;
            last_created = new_node;
        }
        else
        {
            last_created->nextSibling = new_node;
            last_created = new_node;
        }

        new_node->parent = chosen_one;
        new_node->visits = 0;
        new_node->node_score = 0;
        new_node->isChanceNode = true;
        
        drawSpecificPiece(&new_node->state->players[whose_turn],new_node->state->board,i);
        expandWithChanceNodes(new_node);
        
    }
}

void backPropagation(node* chosen_one, int player, double score)
{
    node* temp_node = chosen_one;
    while(temp_node)
    {
        temp_node->visits++;
        
        
        //TODO CHECK IF THIS IS RIGHT
        //ALSO TODO UPDATE FOR MORE THAN 2 PLAYERS SHOULD THIS BE CORRECT
        if(temp_node->state->whose_turn == player)
        {
            temp_node->node_score += score;
        }
        temp_node = temp_node->parent;
    }
}
//maybe change to get gamestate as input
gamestate* findNextMove(gamestate* current_state)
{


    int whose_turn = current_state->whose_turn;
    board* boardstate = current_state->board;
    player* players = current_state->players;

    //create new tree
    node* root_node = malloc(sizeof(node));
    createNewNode(boardstate,players,whose_turn,root_node);
    root_node->node_score = 0;
    root_node->visits = 0;
    root_node->firstChild = NULL;
    root_node->nextSibling = NULL;
    root_node->parent = NULL;
    int result;
    time_t start_time = time(&start_time);

    time_t current_time = time(&current_time);
    int counter = 0;
    
    while((current_time - start_time) < 2) //TODO END TIME VARIABLE
    {
        node* next_node = selectNode(root_node);
        if(next_node->state->winning == -2)
        {
            expand(next_node);
        }
        if(next_node->firstChild)
        {
            next_node = next_node->firstChild;
        }
        if(next_node->state->winning == -2)
        {
        result = simulatePlayout(next_node);
        }
        else
        {
        result = next_node->state->winning;
        }
        if (result == -1) backPropagation(next_node, next_node->state->whose_turn, 0.5);
        else if (result != next_node->state->whose_turn) backPropagation(next_node, next_node->state->whose_turn, 1);
        else backPropagation(next_node, next_node->state->whose_turn, 0);
        current_time = time(&current_time);
        counter++;
    } 
    
    gamestate* next_move = getMaxChild(root_node)->state;
    gamestate* new_gamestate = copyGameState(next_move);
    // double num__non_leafs = 0;
    // double num_nodes = 0;
    // traverseTree(root_node->firstChild,&num_nodes,&num__non_leafs);
    // printf("amound non end nodes : %f \n", num_nodes);
    freeTree(root_node);
    return new_gamestate;

}
gamestate* findNextMoveWithDeterminedDraw(gamestate* current_state, int max_counter)
{


    int whose_turn = current_state->whose_turn;
    board* boardstate = current_state->board;
    player* players = current_state->players;

    //create new tree
    node* root_node = malloc(sizeof(node));
    createNewNode(boardstate,players,whose_turn,root_node);
    root_node->node_score = 0;
    root_node->visits = 0;
    root_node->firstChild = NULL;
    root_node->nextSibling = NULL;
    root_node->parent = NULL;
    int result;
    time_t start_time = time(&start_time);

    time_t current_time = time(&current_time);
    int counter = 0;
    
    while((current_time - start_time) < 2) //TODO END TIME VARIABLE
    {
        node* next_node = selectNode(root_node);
        if(next_node->state->winning == -2)
        {
            expand(next_node);
        }
        if(next_node->firstChild)
        {
            next_node = next_node->firstChild;
        }
        if(next_node->state->winning == -2)
        {
        result = simulatePlayout(next_node);
        }
        else
        {
        result = next_node->state->winning;
        }
        if (result == -1) backPropagation(next_node, next_node->state->whose_turn, 0.5);
        else if (result != next_node->state->whose_turn) backPropagation(next_node, next_node->state->whose_turn, 1);
        else backPropagation(next_node, next_node->state->whose_turn, 0);
        current_time = time(&current_time);
        counter++;
    } 
    
    gamestate* next_move = getMaxChild(root_node)->state;
    gamestate* new_gamestate = copyGameState(next_move);
    // double num__non_leafs = 0;
    // double num_nodes = 0;
    // traverseTree(root_node->firstChild,&num_nodes,&num__non_leafs);
    // printf("amound non end nodes : %f \n", num_nodes);
    freeTree(root_node);
    return new_gamestate;

}
//Copys gamestate1 into gamestate 2
gamestate* copyGameState(gamestate* original_gamestate)
{
    gamestate* new_gamestate = calloc(1,sizeof(gamestate));
    new_gamestate->board = calloc(1,sizeof(board));
    new_gamestate->players = calloc(original_gamestate->board->amount_of_players,sizeof(player));
    new_gamestate->whose_turn = original_gamestate->whose_turn;
    new_gamestate->winning = original_gamestate->winning;

    //copy board
    new_gamestate->board->draw_pile_size = original_gamestate->board->draw_pile_size;
    new_gamestate->board->draw_pile = calloc(original_gamestate->board->draw_pile_size,sizeof(piece));
    memcpy(new_gamestate->board->draw_pile,original_gamestate->board->draw_pile, original_gamestate->board->draw_pile_size * sizeof(piece));
    new_gamestate->board->pieces_on_board = calloc(156,sizeof(piece*));
    for(int i = 0; i < 156; i++)
    {
        new_gamestate->board->pieces_on_board[i] = calloc(156,sizeof(piece));
        memcpy(new_gamestate->board->pieces_on_board[i], original_gamestate->board->pieces_on_board[i], sizeof(piece) * 156);
    }
    if(!new_gamestate->board->pieces_on_board)
    {
        fprintf(stderr,"error assigning memory");
        exit(-1);
    }
    new_gamestate->board->amount_of_players = original_gamestate->board->amount_of_players;
    

    //copy players
    for(int i = 0; i < original_gamestate->board->amount_of_players; i++)
    {
    new_gamestate->players[i].amount_of_pieces = original_gamestate->players[i].amount_of_pieces;
    new_gamestate->players[i].player_pieces_size = original_gamestate->players[i].amount_of_pieces;
    new_gamestate->players[i].player_pieces = malloc(sizeof(piece) * original_gamestate->players[i].amount_of_pieces);
    memcpy(new_gamestate->players[i].player_pieces,original_gamestate->players[i].player_pieces,original_gamestate->players[i].amount_of_pieces * sizeof(piece));
    }
    return new_gamestate;
}
void createNewNode(board* current_board, player* players,int whose_turn, node* new_node)
{

    gamestate* new_gamestate = malloc(sizeof(gamestate));
    board* temp_board = malloc(sizeof(board));
    //things to copy
    player* temp_players;
    piece** pieces_on_board;
    piece* draw_pile;
    
    //allocate memory
    temp_players = malloc(sizeof(player) * current_board->amount_of_players);
    pieces_on_board = malloc(sizeof(piece*) * 156);
    for(int k = 0; k < 156; k++)
    {
        pieces_on_board[k] = calloc(156, sizeof(piece));
    }
    if(!pieces_on_board)
    {
        fprintf(stderr, "error assigning memory");
        //TODO freethings
        exit(-1);
    }
    
    //copy relevant data
    draw_pile = malloc(sizeof(piece) * current_board->draw_pile_size);
    memcpy(draw_pile, current_board->draw_pile,current_board->draw_pile_size * sizeof(piece));
    for(int k = 0; k < 156; k++)
    {
        memcpy(pieces_on_board[k],current_board->pieces_on_board[k],156 * sizeof(piece));
    }
    for(int k = 0; k < current_board->amount_of_players;k++)
    {
        piece* temp_player_pieces = malloc(players[k].amount_of_pieces * sizeof(piece));
        memcpy(temp_player_pieces,players[k].player_pieces,players[k].amount_of_pieces * sizeof(piece));
        temp_players[k].player_pieces = temp_player_pieces;
        temp_players[k].player_pieces_size = players[k].amount_of_pieces;
        temp_players[k].amount_of_pieces = players[k].amount_of_pieces;
    }

    //put it all together
    temp_board->draw_pile = draw_pile;
    temp_board->amount_of_players = current_board->amount_of_players;
    temp_board->draw_pile_size = current_board->draw_pile_size;
    temp_board->pieces_on_board = pieces_on_board;

    new_gamestate->players = temp_players;
    new_gamestate->board = temp_board;
    new_gamestate->winning = checkWin(temp_board,temp_players);

//why is this here
    // if(whose_turn == temp_board.amount_of_players - 1) new_gamestate.whose_turn = 0;
    // else new_gamestate.whose_turn = whose_turn++;
    new_gamestate->whose_turn = whose_turn;
    new_node->state = new_gamestate;
    new_node->isChanceNode = false;
    new_node->firstChild = NULL;
    new_node->nextSibling = NULL;
    new_node->parent = NULL;
}


void traverseTree(node* current_node,double* num_nodes,double* num__non_leafs)
{

    while(current_node)
    {
        // if(current_node->state->whose_turn != current_node->parent->state->whose_turn) *num_nodes = *num_nodes + 1;
        
        if(current_node->firstChild)
        {
            traverseTree(current_node->firstChild,num_nodes,num__non_leafs);
            *num__non_leafs = *num__non_leafs + 1;
        }
        if(!current_node->firstChild && current_node->state->winning == -2)
        {
           *num_nodes = *num_nodes + 1;
        }


        current_node = current_node->nextSibling;
    }

    return;
}
//currently under construction
void constructFullTree(gamestate* current_state)
{
    node* root_node = malloc(sizeof(node));
    createNewNode(current_state->board,current_state->players,current_state->whose_turn,root_node);
    root_node->visits = 0;
    root_node->node_score = 0;
    expandRecursively(root_node);
    printf("hi");
    freeTree(root_node);
}

void expandRecursively(node* current_node)
{
        expand(current_node);
        if(current_node->firstChild)
        {
            expandRecursively(current_node->firstChild);
        }
        if(current_node->nextSibling)
        {
            expandRecursively(current_node->nextSibling);
        }
    
}


gamestate* findNextMoveEPT(gamestate* current_state, int which_eval_function)
{


    int whose_turn = current_state->whose_turn;
    board* boardstate = current_state->board;
    player* players = current_state->players;

    //create new tree
    node* root_node = malloc(sizeof(node));
    createNewNode(boardstate,players,whose_turn,root_node);
    root_node->node_score = 0;
    root_node->visits = 0;
    root_node->firstChild = NULL;
    root_node->nextSibling = NULL;
    root_node->parent = NULL;
    int result;
    time_t start_time = time(&start_time);

    time_t current_time = time(&current_time);
    int counter = 0;
    
    //TODO global time variable
    while((current_time - start_time) < 2)
    {
        node* next_node = selectNode(root_node);
        if(next_node->state->winning == -2 && next_node->visits > 0)
        {
            expand(next_node);
        }
        if(next_node->firstChild)
        {
            next_node = next_node->firstChild;
        }
        if(next_node->state->winning == -2)
        {
        result = simulatePlayoutEPT(next_node,which_eval_function);
        }
        else
        {
        result = next_node->state->winning;
        }
        if (result == -1) backPropagation(next_node, next_node->state->whose_turn, 0.5);
        else if (result != next_node->state->whose_turn) backPropagation(next_node, next_node->state->whose_turn, 1);
        else backPropagation(next_node, next_node->state->whose_turn, 0);
        current_time = time(&current_time);
        counter++;
    } 
    
    gamestate* next_move = getMaxChild(root_node)->state;
    gamestate* new_gamestate = copyGameState(next_move);
    // double num__non_leafs = 0;
    // double num_nodes = 0;
    // traverseTree(root_node->firstChild,&num_nodes,&num__non_leafs);
    // printf("amound non end nodes : %f \n", num_nodes);
    freeTree(root_node);
    return new_gamestate;

}
int simulatePlayoutEPT(node* current_node,int which_eval_function)
{

    node * temp_node = malloc(sizeof(node));
    createNewNode(current_node->state->board,current_node->state->players,current_node->state->whose_turn,temp_node);
    board* boardstate = temp_node->state->board;
    player* players = temp_node->state->players;
    bool playing = true;
    int i = current_node->state->whose_turn;  
    int counter = 0;
    //random game start
    while (playing && counter < 4)
    {
        for(; i < boardstate->amount_of_players; i++)
        {   
            bool played_move = false;
            for(int j = 0; j < players[i].amount_of_pieces; j++)
            {
                MovesStruct moves = calculateMoves(players[i].player_pieces[j], boardstate,j);
                if(moves.amount_of_moves > 0)
                {
                    played_move = true;
                    uint32_t random = randombytes_uniform(moves.amount_of_moves);
                    playMove(moves.moves[random], boardstate, &players[i]);
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

                    int draw = drawPiece(&players[i],boardstate);
                    if (draw == -1)
                    {
                        freeNode(temp_node);
                      return draw;  
                    } 
                    MovesStruct moves = calculateMoves(players[i].player_pieces[players[i].amount_of_pieces - 1], boardstate,players[i].amount_of_pieces - 1);
                    if(moves.amount_of_moves > 0)
                    {
                        played_move = true;
                        playMove(moves.moves[0], boardstate, &players[i]);
                        players[i].amount_of_pieces --;
                    }
                    free(moves.moves);
                }
                
            }
            if(players[i].amount_of_pieces == 0)
            {
                freeNode(temp_node);
                return i;
            }
        } 
        counter++;
        i = 0;
    }
    float evaluation = evaluate_gamestate(temp_node->state,which_eval_function);
//if eval is equal check for available moves, if none->not advantaged, if moves ->advantaged
    if (evaluation == 0)
    {
        for(int i = 0; i < temp_node->state->players[temp_node->state->whose_turn].amount_of_pieces; i++)
        {
            MovesStruct moves = calculateMoves(temp_node->state->players[temp_node->state->whose_turn].player_pieces[i],boardstate,i);
            if (moves.amount_of_moves > 0)
            {
                evaluation ++;
            }
            free(moves.moves);
        }
    }
    int whose_turn = temp_node->state->whose_turn;
    freeNode(temp_node);
    if (evaluation == 0) return -1;
    if (evaluation > 0) return whose_turn;
    whose_turn ++;
    if (whose_turn > 1) whose_turn = 0;
    return whose_turn;
}
float evaluate_gamestate(gamestate* current_gamestate, int which_eval_function)
{
    //definitely doesnt work for more than 2 players
    //different eval functions here
    //eval_func 0 : #pieces player_not_to_move - #pieces player_to_move
    //eval func 1: (#pieces player_not_to_move_with_no_available_moves + parameter * #pieces player_not_to_move_with_available_move) - (#pieces player_to_move_with_no_available_move + parameter * #pieces player_to_move_with_available_move) 

    switch(which_eval_function)
    {
        case (0): return ((current_gamestate->players[1].amount_of_pieces - current_gamestate->players[0].amount_of_pieces) * (1 - (2 * current_gamestate->whose_turn)));
        case (1): return evaluation_function(current_gamestate,0.1);
        case (2): return evaluation_function(current_gamestate,0.3);
        case (3): return evaluation_function(current_gamestate,0.5);
        case (4): return evaluation_function(current_gamestate,0.7);
        case (5): return evaluation_function(current_gamestate,0.9);
    }
    
    return 0;
}
float evaluation_function(gamestate* current_gamestate, float weighting_parameter)
{
    float player0_pieces_with_moves = 0;
    for (int i = 0; i < current_gamestate->players[0].amount_of_pieces; i++)
    {
        MovesStruct moves = calculateMoves(current_gamestate->players[0].player_pieces[i],current_gamestate->board,i);
        if (moves.amount_of_moves > 0) player0_pieces_with_moves ++;
        free(moves.moves);
    }
    float player1_pieces_with_moves = 0;
    for (int i = 0; i < current_gamestate->players[1].amount_of_pieces; i++)
    {
        MovesStruct moves = calculateMoves(current_gamestate->players[1].player_pieces[i],current_gamestate->board,i);
        if (moves.amount_of_moves > 0) player1_pieces_with_moves ++;
        free(moves.moves);
    }
    return (float)(((float)(current_gamestate->players[1].amount_of_pieces - player1_pieces_with_moves) + (player1_pieces_with_moves * weighting_parameter) - ((float)(current_gamestate->players[0].amount_of_pieces - player0_pieces_with_moves) + (player0_pieces_with_moves * weighting_parameter))) * (1 - (2 * current_gamestate->whose_turn)));
}
void shuffleDrawList(board* boardstate)
{

    for(int i = boardstate->draw_pile_size; i >= 0; i--)
    {
        uint32_t random = randombytes_uniform(boardstate->draw_pile_size+1);
        changeValues(&boardstate->draw_pile[i], &boardstate->draw_pile[random]);
    }
}
void changeValues(piece* piece1, piece* piece2)
{
    piece temp = *piece1;
    *piece1 = *piece2;
    *piece2 = temp;
}
int main(int argc, const char*argv[]){

}