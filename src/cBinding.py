import ctypes
import pathlib
from board import *
from piece import *

def cConnector(board :Board, piece :Piece ) -> int:
    piece_list = [i for i in range(76)]
    counter = 0
    current_pieces = board.getBoardState()
    #find all Pieces on board, count them
    for index1,i in enumerate(range(len(current_pieces))):
        for index2,k in enumerate(current_pieces[i]):
            #read if k is a piece
            if k!= 0:
                tempPiece = cPiece(mapColorToInteger(k._color1),mapColorToInteger(k._color2),mapColorToInteger(k._color3),index1,index2,k._upside)
                piece_list[counter] = tempPiece
                counter += 1
    #create c struct array for passing it to c function
    piece_array = (cPiece * counter)()
    for i in range(counter):
        piece_array[i] = piece_list[i]
    struct_piece = cPiece(mapColorToInteger(piece._color1),mapColorToInteger(piece._color2),mapColorToInteger(piece._color3),0,0,piece._upside)
    cEckolo = CDLL("eckolo.sh")
    cEckolo.connect()
    cEckolo.connectPython.restype = c_int        
    amount_of_moves = cEckolo.connectPython(byref(piece_array),struct_piece, c_int(counter))
    return amount_of_moves

def mapColorToInteger(color: str) -> int:
    for index,element in enumerate(colors):
        if color == element:
            return index
    return -1