from random import randrange

from more_itertools import first
from constants import *
from piece import *
#class for playing board
class Board(object):
    def __init__(self, display, piece_list):
        
        self._display = display
        self._piece_list = piece_list
        
        #draw first Piece
        random_number = randrange(0, len(piece_list))
        temp_piece = piece_list.pop(random_number)
        first_piece = Piece(temp_piece[0],temp_piece[1],temp_piece[2],self._display,window_resolution[0]/2,window_resolution[1]/2)
        first_piece.setBoardPosition((77,77)) #First Piece in middle of the board
        #board representation
        self._board_state = [[0 for i in range(156)]for k in range(156)]
        self._board_state[77][77] = first_piece
    def getPieceList(self):
        return self._piece_list
    
    def getBoardState(self):
        return self._board_state
    
    def insertPiece(self,piece: Piece):
        self._board_state[piece._board_position[0]][piece._board_position[1]] = piece
    #draws all pieces currently in play
    def drawBoard(self):
        for i in range(len(self._board_state)):
            for k in self._board_state[i]:
                if k != 0:
                    k.printPiece()

    def drawPiece(self,player_pieces):
        if len(self._piece_list) == 0:
            print("its a draw")
            return 0
        random_number = randrange(0,len(self._piece_list))
        piece = self._piece_list.pop(random_number)
        player_pieces.append(Piece(piece[0],piece[1],piece[2],self._display,0,0))
        return 1
    
    def printBoardState(self):
        for i in range(72,80):
            for k in self._board_state[i]:
                if k != 0:
                    print(1,end='')
                else:
                    print(0,end='')
            print("")
    
    def moveCamera(self,x,y):
        for k in range(len(self._board_state)):
            for piece in self._board_state[k]:
                if piece != 0:
                    piece._X += x
                    piece._Y += y
        


    

