from random import randrange
from piece import *
#generating all possible pieces
#INPUT:///
#OUTPUT:List of all Pieces
def createPieces():
    list = []
    for i in range(0,6):
        list.append((i,i,i))
        for j in range(i,6):
            for k in range(i+1,6):
                list.append((i,j,k))
    return list

#Function for distributing pieces at the start of the game
#INPUT: (List of Pieces, Amount of Players)
#OUTPUT: List of Lists with the pieces for each player as objects of class "Piece"
def distribute_pieces(piece_list, player_number,screen):

    #create empty list for all players
    player_pieces = [[]for i in range(player_number)]
    
    #fill list with random pieces
    for i in range(6):
        for j in range(player_number):
            random = randrange(len(piece_list))
            piece = piece_list.pop(random)
            player_pieces[j].append(Piece(piece[0],piece[1],piece[2],screen,0,0))
    return player_pieces
