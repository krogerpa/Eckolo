#---IMPORTS---
from ctypes import util
from operator import truediv
import pathlib
from cgi import test
from re import T
from tokenize import String
import pygame
from pygame.locals import *
from time import sleep
from cBinding import mapColorToInteger
from constants import *
from createPieces import *
from piece import *
from board import *
from random import *
from ctypes import *
from os import *

def drawButtons(screen):
    dir_path = path.dirname(path.realpath(__file__))
    draw_button = pygame.image.load( dir_path + '/img/triangle_button.png')
    draw_button = pygame.transform.scale(draw_button,button_size)
    draw_button = draw_button.convert_alpha()
    arrow_button = pygame.image.load(dir_path + '/img/arrow_button2.png')
    arrow_button = pygame.transform.scale(arrow_button, arrow_button_size)
    replay_button = pygame.image.load(dir_path + '/img/replay_button.png')
    replay_button = pygame.transform.scale(replay_button, replay_button_size)

    screen.blit(replay_button, (window_resolution[0] - replay_button_size[0] - 20, 0) )
    screen.blit(draw_button, (window_resolution[0] - button_size[0], window_resolution[1] - button_size[1] + 20))
    screen.blit(arrow_button, (window_resolution[0] - button_size[0] - arrow_button_size[0], window_resolution[1] - arrow_button_size[1]*1.3))
    screen.blit(arrow_button, (window_resolution[0] - button_size[0] - arrow_button_size[0],10))

    arrow_button = pygame.transform.rotate(arrow_button,180)

    screen.blit(arrow_button, (window_resolution[0] - button_size[0] - arrow_button_size[0] * 1.5, window_resolution[1] - arrow_button_size[1]*1.3))
    screen.blit(arrow_button, (window_resolution[0] - button_size[0] - arrow_button_size[0] * 1.5, 10))
 
def drawBackground(screen):
    dir_path = path.dirname(path.realpath(__file__))
    background = pygame.image.load(dir_path + '/img/table.jpg')
    background = pygame.transform.scale(background, window_resolution)
    pygame.display.set_caption("PyEcko")
    screen.blit(background,(0,0))
    drawButtons(screen)

def blitPieces(player_pieces, display):
    xpos = 0
    ypos = 0
    for i in range(len(player_pieces)):
        if(len(player_pieces[i]) - (display_player_pieces_index[i] * display_player_pieces_length) <= 0):
            display_player_pieces_index[i] = 0
        if(len(player_pieces[i]) > (display_player_pieces_index[i] + 1) * display_player_pieces_length):
            range_number = (display_player_pieces_index[i] + 1) * display_player_pieces_length 
        else:
            range_number = len(player_pieces[i])
        for k in range(0,len(player_pieces[i])):
            if (k in range(display_player_pieces_index[i] * display_player_pieces_length,range_number)): 
                player_pieces[i][k].setX(xpos)
                player_pieces[i][k].setY(ypos)
                player_pieces[i][k].printPiece()
                xpos = xpos + triangle_size[0]
            else:
                player_pieces[i][k].setX(-1000)
                player_pieces[i][k].setY(-1000)
        ypos = ypos + 800
        xpos = 0
#def positionPiece(x,y,desired_position,piece):
#TODO Where to make move
def makeMove(move, piece:Piece,board):
    position = move[0]
    x = move[2]._X
    y = move[2]._Y
    piece._upside = move[3]
    piece.rotatePiece(move[1])

    match position:
        case "bottom":
            piece.setX(x)
            piece.setY(y + triangle_size[1])
            piece.setBoardPosition((move[2]._board_position[0] + 1, move[2]._board_position[1]))
            board.insertPiece(piece)
        case "left":
            piece.setX(x - triangle_size[0]/2)
            piece.setY(y)
            piece.setBoardPosition((move[2]._board_position[0] , move[2]._board_position[1] - 1))
            board.insertPiece(piece)
        case "right":
            piece.setX(x + triangle_size[0]/2)
            piece.setY(y)
            piece.setBoardPosition((move[2]._board_position[0], move[2]._board_position[1] + 1))
            board.insertPiece(piece)
        case "top":
            piece.setX(x)
            piece.setY(y - triangle_size[1])
            piece.setBoardPosition((move[2]._board_position[0] - 1, move[2]._board_position[1]))
            board.insertPiece(piece)
        case _:
            raise ValueError("Unexpected Value")


    piece.printPiece()

#checks for potential colormismatches on the board if new piece is to be played
#INPUT: piece to be played, board, move (see available_moves())
def checkCollisions(piece,board,move) -> bool:
    result: bool = False
    match move[0]:
        case "top":
            new_board_pos = (move[2]._board_position[0] - 1, move[2]._board_position[1])
        case "bottom":
            new_board_pos = (move[2]._board_position[0] + 1, move[2]._board_position[1])
        case "left":
            new_board_pos = (move[2]._board_position[0], move[2]._board_position[1] - 1)
        case "right":
            new_board_pos = (move[2]._board_position[0], move[2]._board_position[1] + 1)
    #Havent found a better way other than checking manually...
    #read if piece is to be placed upside down
    if move[3] == 1:
#CHECK FOR COLOR1    
        color_list = []
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color2)        
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        for colors in color_list:
            if piece._color1 != colors:
                return True

#CHECK FOR COLOR2
        color_list = []
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-2]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]-2]
        if current_piece != 0:
            color_list.append(current_piece._color3)        
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        for colors in color_list:
            if piece._color2 != colors:
                return True
#CHECK FOR COLOR3
        color_list = []
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+2]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]+2]
        if current_piece != 0:
            color_list.append(current_piece._color1)        
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        for colors in color_list:
            if piece._color3 != colors:

                return True
#Read if piece is not to be placed upside down
    else:
#CHECK FOR COLOR1
        color_list = []
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-2]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]-2]
        if current_piece != 0:
            color_list.append(current_piece._color3)        
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        for colors in color_list:
            if piece._color1 != colors:
                return True
#CHECK FOR COLOR2
        color_list = []
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]-1]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color1)        
        current_piece = board._board_state[new_board_pos[0]-1][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        for colors in color_list:
            if piece._color2 != colors:
                return True
#CHECK FOR COLOR3
        color_list = []
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]]
        if current_piece != 0:
            color_list.append(current_piece._color3)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color2)
        current_piece = board._board_state[new_board_pos[0]+1][new_board_pos[1]+2]
        if current_piece != 0:
            color_list.append(current_piece._color2)        
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+2]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        current_piece = board._board_state[new_board_pos[0]][new_board_pos[1]+1]
        if current_piece != 0:
            color_list.append(current_piece._color1)
        for colors in color_list:
            if piece._color3 != colors:

                return True
    return False

def checkIfClickedOnButton(x,y) -> str:

    if x in range(window_resolution[0] - button_size[0],window_resolution[0]):
        if y in range(window_resolution[1] - button_size[1], window_resolution[1]):
            return "drawButton"

    if x in range(window_resolution[0] - button_size[0] - arrow_button_size[0],window_resolution[0] - button_size[0]):
        if y in range(0,arrow_button_size[1]):
            return "button_right_computer"
    

    if x in range(int(window_resolution[0] - button_size[0] - arrow_button_size[0] * 1.5) , window_resolution[0] - button_size[0] - arrow_button_size[0]):
        if y in range(0, arrow_button_size[1]):
            return "button_left_computer"

    if x in range(window_resolution[0] - button_size[0] - arrow_button_size[0],window_resolution[0] - button_size[0]):
        if y in range(int(window_resolution[1] - arrow_button_size[1]*1.3), window_resolution[1]):
            return "button_right_player"
    

    if x in range(int(window_resolution[0] - button_size[0] - arrow_button_size[0] * 1.5) , window_resolution[0] - button_size[0] - arrow_button_size[0]):
        if y in range(int(window_resolution[1] - arrow_button_size[1]*1.3), window_resolution[1]):
            return "button_left_player"



    if x in range(window_resolution[0] - replay_button_size[0],window_resolution[0]):
        if y in range(0, replay_button_size[1]):
            return "replay_button"
    return "None"

def checkIfClickedOnPiece(x,y,player_pieces):
    for index,piece in enumerate(player_pieces):
        if x >= (piece._X) and x <= (piece._X + triangle_size[0]) \
            and y >= (piece._Y) and y <= (piece._Y + triangle_size[1]):

            return index,piece
    
    else:
        return 0,0

def computerMonteMove(player_pieces, board):
    made_a_move = False
    result = 1
    while not made_a_move:
        for index,piece in enumerate(player_pieces[0]):
            moves = availableMoves(board,piece)
            # cmoves = cConnector(board,piece)
            # if (len(moves) != cmoves):
            #     print(len(moves),cmoves)
            #     return 10
            if len(moves) != 0:
                cConnector(board,player_pieces)
                made_a_move = True
                break
        if not made_a_move:
            result = board.drawPiece(player_pieces[0])
            if result == 0:
                return result
    return result
                


def computerRandomMove(computer_pieces, board):
    made_a_move = False
    result = 1
    while not made_a_move:
        for index,piece in enumerate(computer_pieces):
            moves = availableMoves(board,piece)
            # cmoves = cConnector(board,piece)
            # if (len(moves) != cmoves):
            #     print(len(moves),cmoves)
            #     return 10
            if len(moves) != 0:
                random_number = randrange(0,len(moves))
                makeMove(moves[0],piece,board)
                computer_pieces.pop(index)
                made_a_move = True
                break
        if not made_a_move:
            result = board.drawPiece(computer_pieces)
            if result == 0:
                return result
    return result
                


def showPossibleMoves(screen,moves):
    dir_path = path.dirname(path.realpath(__file__))
    image = pygame.image.load(dir_path + '/img/PossiblePlaceTriangle.png')
    image = pygame.transform.scale(image,triangle_size)
    image = image.convert_alpha()
    for move in moves:
        xpos = move[2]._X
        ypos = move[2]._Y
        position = move[0]
        match position:
            case "bottom":
                ypos += triangle_size[1]

            case "left":
                xpos -= triangle_size[0]/2
                

            case "right":
                xpos += triangle_size[0]/2
                

            case "top":
                ypos -= triangle_size[1]

            case _:
                raise ValueError("Unexpected Value")

        if move[3] == 1:
            image = pygame.transform.rotate(image,180)
            screen.blit(image,(xpos,ypos))
            image = pygame.transform.rotate(image,180)
        else:
            screen.blit(image,(xpos,ypos))
    pygame.display.update()

def checkIfClickedOnMove(x,y,possible_moves):
    for index,move in enumerate(possible_moves):
        xpos = move[2]._X
        ypos = move[2]._Y
        position = move[0]
        match position:
            case "bottom":
                ypos += triangle_size[1]

            case "left":
                xpos -= triangle_size[0]/2
                

            case "right":
                xpos += triangle_size[0]/2
                

            case "top":
                ypos -= triangle_size[1]

            case _:
                raise ValueError("Unexpected Value")
        if x >= xpos and x <= xpos+triangle_size[0]:
            if y >= ypos and y <= ypos+triangle_size[1]:
                return index
    
    return None
def analzye(board,player_pieces,screen):
    playing = True
    while playing:
            
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    board.moveCamera(50,0)
                    screen.fill((124,252,0))
                    board.drawBoard()
                    blitPieces(player_pieces,screen)
                if event.key == pygame.K_LEFT:
                    board.moveCamera(-50,0)
                    screen.fill((124,252,0))
                    board.drawBoard()
                    blitPieces(player_pieces,screen)
                if event.key == pygame.K_UP:
                    board.moveCamera(0,-50)
                    screen.fill((124,252,0))
                    board.drawBoard()
                    blitPieces(player_pieces,screen)
                if event.key == pygame.K_DOWN:
                    board.moveCamera(0,50)
                    screen.fill((124,252,0))
                    board.drawBoard()
                    blitPieces(player_pieces,screen)
                if event.key == pygame.K_SPACE:
                    playing = False

                
def playGame(screen):
    counter = 0
    wins = [0,0]
    for i in range(1000):
        screen.fill((124,252,0))
        pieces = createPieces()
        player_pieces = distribute_pieces(pieces,players,screen)
        board = Board(screen,pieces)
        playing = True
        
        while playing:
            for index,player in enumerate(player_pieces):
                
                result = computerRandomMove(player,board)
                # if result == 10:
                #     screen.fill((124,252,0))
                #     board.drawBoard()
                #     blitPieces(player_pieces,screen)
                #     analzye(board,player_pieces,screen)
                if  result == 0:
                    screen.fill((124,252,0))
                    board.drawBoard()
                    blitPieces(player_pieces,screen)
                    print("it was player ", index)
                    counter += 1
                    # analzye(board,player_pieces,screen)
                    playing = False
                    break
                if len(player) == 0:
                    print("Player", index ,"won", i)
                    wins[index] += 1
                    playing = False
                    break
    print(counter)
    print(counter/1000)
    print(wins[0]/1000)
    print(wins[1]/1000)
#returns list of tuples with piece and corresponding positions
#INPUT: board object, piece to check moves for
#OUTPUT: List of available piece placements + whether the piece is upside down
def availableMoves(board, piece):
    #check right or left
    current_pieces = board.getBoardState()
    results = []
    for i in range(len(current_pieces)):
        for k in current_pieces[i]:
            #TODO check if free space
            #read: if k is a piece
            if k != 0:
                top_flag = 0
                bottom_flag = 0
                left_flag = 0
                right_flag = 0  
                upside_flag = 1              
                #if i==0 or k == 0:
                #get current angle and colors in following order given piece is upside down:(bottom color, top left color, top right color)
                # if k._angle > 180:
                #     current_colors = (k._color3,k._color1,k._color2)
                # elif k._angle > 60:
                #     current_colors = (k._color2,k._color3,k._color1)
                # else:
                #     current_colors = (k._color1,k._color2,k._color3)

                current_colors = (k._color1,k._color2,k._color3)
                #splits in two here depending on whether k is upside down or not
    
                if k._upside == 1:
                    #check if there is free space
                    upside_flag = 0
                    if current_pieces[k._board_position[0]-1][k._board_position[1]] == 0 :
                        top_flag = 1
                    if current_pieces[k._board_position[0]][k._board_position[1] - 1] == 0:
                        left_flag = 1
                    if current_pieces[k._board_position[0]][k._board_position[1] + 1] == 0:
                        right_flag = 1
                    
                else:
                    #check if there is free space
                    if current_pieces[k._board_position[0]+1][k._board_position[1]] == 0 :
                        bottom_flag = 1
                    if current_pieces[k._board_position[0]][k._board_position[1] - 1] == 0:
                        left_flag = 1
                    if current_pieces[k._board_position[0]][k._board_position[1] + 1] == 0:
                        right_flag = 1
                
                #check if colors match
                #adds tupel of where it matched and angle at which the piece needs to be rotated in order to fit
                if top_flag == 1:
                    if current_colors[1] == piece._color1 \
                    and current_colors[2] == piece._color3:
                        results.append(["top",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[1] == piece._color2 \
                    and current_colors[2] == piece._color1:
                        results.append(["top",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[1] == piece._color3 \
                    and current_colors[2] == piece._color2:
                        results.append(["top",(piece._color3,piece._color1,piece._color2),k,upside_flag])
                
                if left_flag == 1 and k._upside != 1:
                    if current_colors[0] == piece._color1 \
                    and current_colors[1] == piece._color3:
                        results.append(["left",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[0] == piece._color2 \
                    and current_colors[1] == piece._color1:
                        results.append(["left",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[0] == piece._color3 \
                    and current_colors[1] == piece._color2:
                        results.append(["left",(piece._color3,piece._color1,piece._color2),k,upside_flag])
                
                if left_flag == 1 and k._upside == 1:
                    if current_colors[0] == piece._color3 \
                    and current_colors[1] == piece._color2:
                        results.append(["left",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[0] == piece._color1 \
                    and current_colors[1] == piece._color3:
                        results.append(["left",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[0] == piece._color2 \
                    and current_colors[1] == piece._color1:
                        results.append(["left",(piece._color3,piece._color1,piece._color2),k,upside_flag])
                        
                
                if right_flag == 1 and k._upside == 1:
                    if current_colors[0] == piece._color1 \
                    and current_colors[2] == piece._color2:
                        results.append(["right",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[0] == piece._color2 \
                    and current_colors[2] == piece._color3:
                        results.append(["right",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[0] == piece._color3 \
                    and current_colors[2] == piece._color1:
                        results.append(["right",(piece._color3,piece._color1,piece._color2),k,upside_flag])
                
                if right_flag == 1 and k._upside != 1:
                    if current_colors[1] == piece._color2 \
                    and current_colors[2] == piece._color1:
                        results.append(["right",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[1] == piece._color3 \
                    and current_colors[2] == piece._color2:
                        results.append(["right",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[1] == piece._color1 \
                    and current_colors[2] == piece._color3:
                        results.append(["right",(piece._color3,piece._color1,piece._color2),k,upside_flag])
                
                if bottom_flag == 1:
                    if current_colors[0] == piece._color2 \
                    and current_colors[2] == piece._color3:
                        results.append(["bottom",(piece._color1,piece._color2,piece._color3),k,upside_flag])
                    if current_colors[0] == piece._color3 \
                    and current_colors[2] == piece._color1:
                        results.append(["bottom",(piece._color2,piece._color3,piece._color1),k,upside_flag])
                    if current_colors[0] == piece._color1 \
                    and current_colors[2] == piece._color2:
                        results.append(["bottom",(piece._color3,piece._color1,piece._color2),k,upside_flag])
             #TODO Check for collisions
    real_result = []
    for move in results:
        piece.rotatePiece(move[1])
        if checkCollisions(piece,board,move) == False:
            real_result.append(move)
    return real_result

def cConnector(board :Board, player_pieces :list[list[Piece]] ):
    piece_list = []
    counter = 0
    current_pieces = board.getBoardState()
    #find all Pieces on board, count them and pass them into c array
    for index1,i in enumerate(range(len(current_pieces))):
        for index2,k in enumerate(current_pieces[i]):
            #read if k is a piece
            if k!= 0:
                tempPiece = cPiece(mapColorToInteger(k._color1),mapColorToInteger(k._color2),mapColorToInteger(k._color3),index1,index2,k._upside)
                piece_list.append(tempPiece)
                counter += 1
    
    piece_array = (cPiece * counter)()
    for index,i in enumerate(piece_list):
        piece_array[index] = i



    #map draw_pile

    draw_counter = 0
    draw_pile = board.getPieceList()
    draw_pile_array = (cPiece * len(draw_pile))()
    for i in draw_pile:
        tempPiece = cPiece(i[0],i[1],i[2],0,0,False)
        draw_pile_array[draw_counter] = tempPiece
        draw_counter += 1
    #map player_pieces to piece_array
    PPiece = POINTER(cPiece)

    total_pieces = 0
    for i in player_pieces:
        total_pieces += len(i)

    player_pieces_array = (cPiece * total_pieces)()

    piece_counter = 0
    for i in player_pieces:
        for j in i:
            tempPiece = cPiece(mapColorToInteger(j._color1),mapColorToInteger(j._color2),mapColorToInteger(j._color3),index1,index2,False)
            player_pieces_array[piece_counter] = tempPiece
            piece_counter += 1
    #create c struct array for passing it to c function
    #populate c struct array

    #int array for number of pieces per player
    amount_of_pieces = (c_int * len(player_pieces))()
    for index,i in enumerate(player_pieces):
        amount_of_pieces[index] = len(i)

    libc = CDLL(util.find_library("c"))
    libname = pathlib.Path().absolute() / "searchMethods.so"
    cEckolo = CDLL(libname)
    cEckolo.connect()
    cEckolo.connectPython.restype = c_void_p        
    return_string = cEckolo.connectPython(piece_array,draw_pile_array,player_pieces_array, c_int(counter),c_int(draw_counter),amount_of_pieces, c_int(len(player_pieces)))
    new_string = cast(return_string,c_char_p).value
    #TODO FREE RETURN_STRING IN C
    libc.free(return_string)
    makeMoveFromReturnString(board,player_pieces,new_string)
    return

def mapColorToInteger(color: str) -> int:
    for index,element in enumerate(colors):
        if color == element:
            return index
    return -1

def makeMoveFromReturnString(board: Board,player_pieces: list[list[Piece]],string : str):
    string = string.decode().split('|')
    moves = availableMoves(board,player_pieces[0][int(string[0])])
    makeMove(moves[int(string[1])],player_pieces[0][int(string[0])],board)
    player_pieces[0].pop(int(string[0]))
    print(string)

def drawWin(index,screen):
    if index == 0:
        dir_path = path.dirname(path.realpath(__file__))
        image_file = pygame.image.load(dir_path + '/img/computer_wins.png')
        image_file = pygame.transform.scale(image_file, win_resolution)
        
    
    else:
        dir_path = path.dirname(path.realpath(__file__))
        image_file = pygame.image.load(dir_path + '/img/you_win.png')
        image_file = pygame.transform.scale(image_file, win_resolution)  

    screen.blit(image_file,(0,0))
    sleep(3)
def main():
   

    runGame()





    #screen = pygame.display.set_mode(window_resolution)
    #playGame(screen)
    # pieces = createPieces()
    # player_pieces = distribute_pieces(pieces, players, screen)
    # board = Board(screen,pieces)
    # temp = cConnector(board,player_pieces[0][0])
    # path = pathlib.Path().absolute() / "src/eckolo.sh"
    # cEckolo = CDLL("src/eckolo.sh")
    # cEckolo.connect()
    # testPiece = cPiece(0,3,6,77,78,True)
    # testPiece2 = cPiece(0,3,2,33,78,True)
    # testPiece3 = cPiece(0,24,6,77,78,True)
    # pieces = [testPiece,testPiece2,testPiece3]
    # pieceArray = (cPiece * 3)()
    # pieceArray[0] = testPiece
    # pieceArray[1] = testPiece2
    # pieceArray[2] = testPiece3

        
    # cEckolo.connectPython(byref(pieceArray),c_int(3))

          
            	
def runGame():
    pygame.init() 
    screen = pygame.display.set_mode(window_resolution)
    # playGame(screen)
    drawBackground(screen)
    FramePerSec = pygame.time.Clock()
    #create,distribute and draw the pieces
    pieces = createPieces()
    player_pieces = distribute_pieces(pieces, players, screen)
    blitPieces(player_pieces, screen)

    #create Board
    board = Board(screen,pieces)
    board.drawBoard()
    running = True
    breakFlag = 0

    pygame.display.update()
    drawBackground(screen)
    blitPieces(player_pieces, screen)
    board.drawBoard()
    pygame.display.update()
    playing_piece_state = False
    piece_to_be_played = None
    possible_moves = None
    piece_index = None
    while running:

        for event in pygame.event.get():
            
            #HANDLE QUIT EVENT
            if event.type == pygame.QUIT:
                running = False
            
            if event.type == MOUSEBUTTONDOWN and not playing_piece_state :
                x,y = event.pos

                index,clicked_piece = checkIfClickedOnPiece(x,y,player_pieces[-1])
                if clicked_piece != 0:
                    moves = availableMoves(board, clicked_piece)
                    if len(moves) != 0:
                        showPossibleMoves(screen,moves)
                        #Enter playing piece gamestate
                        playing_piece_state = True
                        #Remember values for playing piece gamestate
                        piece_to_be_played, possible_moves, piece_index = clicked_piece, moves, index
               
                else:
                    match checkIfClickedOnButton(x,y):
                        case "drawButton":
                            moveFlag = False
                            for piece in player_pieces[-1]:
                                if len(availableMoves(board,piece)) != 0:
                                    moveFlag = True
                                    break
                            if not moveFlag:
                                board.drawPiece(player_pieces[-1])
                                drawBackground(screen)
                                blitPieces(player_pieces, screen)
                                board.drawBoard()
                                #TODO error message maybe?
                        case "button_right_player":
                            display_player_pieces_index[1] += 1
                            drawBackground(screen)
                            blitPieces(player_pieces, screen)
                            board.drawBoard()
                        case "button_left_player":
                            if (display_player_pieces_index[1] > 0):
                                display_player_pieces_index[1] -= 1
                                drawBackground(screen)
                                blitPieces(player_pieces, screen)
                                board.drawBoard()       

                        case "button_right_computer":
                            display_player_pieces_index[0] += 1
                            drawBackground(screen)
                            blitPieces(player_pieces, screen)
                            board.drawBoard()
                        case "button_left_computer":
                            if (display_player_pieces_index[0] > 0):
                                display_player_pieces_index[0] -= 1
                                drawBackground(screen)
                                blitPieces(player_pieces, screen)
                                board.drawBoard()
                        case "replay_button":
                            pieces = createPieces()
                            player_pieces = distribute_pieces(pieces,players,screen)
                            board = Board(screen,pieces)
                            drawBackground(screen)
                            blitPieces(player_pieces, screen)
                            board.drawBoard()

                                
                


#TODO ABHAENGIG VON WINDOW RESOLUTION
            elif event.type == MOUSEBUTTONDOWN and playing_piece_state:
                x,y = event.pos
                index = checkIfClickedOnMove(x,y,possible_moves)
                if index != None:
                    makeMove(moves[index],piece_to_be_played,board)
                    player_pieces[-1].pop(piece_index)
                    computerRandomMove(player_pieces[-1],board)
                    #cConnector(board,player_pieces)
                    drawBackground(screen)
                    blitPieces(player_pieces, screen)
                    board.drawBoard()
                    playing_piece_state = False
                else:
                    playing_piece_state = False
                    drawBackground(screen)
                    blitPieces(player_pieces,screen)
                    board.drawBoard()
                for index,k in enumerate(player_pieces):
                    if len(k) == 0:
                        drawBackground(screen)
                        drawWin(index,screen)
                        #gameoverfunctionhere
                        pieces = createPieces()
                        player_pieces = distribute_pieces(pieces,players,screen)
                        board = Board(screen,pieces)
                        drawBackground(screen)
                        blitPieces(player_pieces, screen)
                        board.drawBoard()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    board.moveCamera(50,0)
                if event.key == pygame.K_LEFT:
                    board.moveCamera(-50,0)
                if event.key == pygame.K_UP:
                    board.moveCamera(0,-50)
                if event.key == pygame.K_DOWN:
                    board.moveCamera(0,50)
                
                if event.key == pygame.K_SPACE:
                    cConnector(board,player_pieces)
                
                drawBackground(screen)
                blitPieces(player_pieces,screen)
                board.drawBoard()
                if playing_piece_state:
                    showPossibleMoves(screen, possible_moves)
            pygame.display.update()


        #SLOW DOWN GAME TO X FPS
        FramePerSec.tick(FPS)
    pygame.quit()
    exit()            
if __name__ == "__main__":
    main()
