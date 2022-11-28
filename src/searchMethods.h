#ifndef __searchMethods__
#define __searchMethods
#include "eckolo.h"
typedef struct gamestate{
    //tbd
    int winning; //contains info if game is over - -2 for running -1 for draw , winning player otherwise
    board* board;
    player* players;
    int whose_turn;
    //other things
}gamestate;
typedef struct node{
    double node_score;
    int visits;
    gamestate* state;
    bool isChanceNode;
    node* parent;
    node* firstChild;
    node* nextSibling;
    int draw_number;
}node;
typedef struct tree{
    node *root;
}tree;
typedef struct pythonPiece{
    int color1;
    int color2;
    int color3;
    int xpos;
    int ypos;
    bool upside_down;
}pythonPiece;
node* selectChanceNode(node* base_node);
gamestate* findNextMoveWithChanceNodes(gamestate* current_state);
int simulatePlayoutWithDeterminedDraw(node* current_node);
void expandWithDeterminedDraw(node* chosen_one);
void shuffleDrawList(board* boardstate);
void changeValues(piece* piece1, piece* piece2);
void createChanceNodes(node* chosen_one);
int playMonte(int number_of_players, board* board, player* players);
int checkWin(board* boardstate, player* players);
char* connectPython(pythonPiece* Boardstate,pythonPiece* drawpile, pythonPiece* player_pieces,int size_of_boardstate,int size_of_drawpile, int* amount_of_pieces,int amount_of_players);
node* selectNode(node* base_node);
node* getMaxChild(node* root_node);
void createNewNode(board* boardstate, player* players,int whose_turn,node* new_node);
double getUCTScore(int total_visits,double node_score, int node_visits);
int simulatePlayout(node* current_node);
void backPropagation(node* chosen_one, int player,double score);
void expand(node* chosen_one);
void expandWithChanceNodes(node* chosen_one);
gamestate* findNextMove(gamestate* current_state);
void freeState(gamestate* state_to_not_be);
void freeNode(node* node_to_not_be);
void freeTree(node* root_to_not_be);
gamestate* findNextMoveWithDeterminedDraw(gamestate* current_state, int max_counter);
gamestate* copyGameState(gamestate* original_gamestate);
void playRandomMove(gamestate* current_gamestate);
void expandRecursively(node* current_node);
void constructFullTree(gamestate* current_state);
void traverseTree(node* current_node,double* num_nodes,double* num_leafs);
gamestate* findNextMoveEPT(gamestate* current_state, int which_eval_function);
int simulatePlayoutEPT(node* current_node,int which_eval_function);
float evaluate_gamestate(gamestate* current_gamestate,int which_eval_function);
float evaluation_function(gamestate* current_gamestate, float weighting_parameter);
char* findNextMovePython(gamestate* current_state);
char * createReturnStringForPython(node* root_node);
int RandomVsRandom(gamestate* current_state);
node* selectNodeVariableConstant(node* base_node,double constant);
double getUCTScoreVariableConstant(int total_visits, double node_score, int node_visits, double constant);
gamestate* findNextMoveVariableConstant(gamestate* current_state,double constant);
gamestate* findNextMoveWithChanceNodesVariableRuntime(gamestate* current_state, int max_counter);
gamestate* findNextMoveEPTChance(gamestate* current_state, int which_eval_function, int max_counter);


#endif