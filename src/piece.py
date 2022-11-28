import pygame,sys
from constants import *
from ctypes import *
from os import *
class Piece(object):
    def __init__(self, color1, color2, color3, display, X, Y):

        self._color1 = colors[color1]
        self._color2 = colors[color2]
        self._color3 = colors[color3]
        
        self._X = X
        self._Y = Y
        
        #Load and convert image to appropriate size
        dir_path = path.dirname(path.realpath(__file__))
        self._image_string = dir_path + "/img/" + self._color1 + self._color2 + self._color3 + ".png"
        self._image = pygame.image.load(self._image_string)
        self._image = pygame.transform.scale(self._image, triangle_size)
        self._image = self._image.convert_alpha()
        
#        self._rect = self._image.get_rect()

        # self._angle = 0
        self._display = display
        #Is the piece upside down or not
        #I suppose a triangle being "upside down" doesnt really make sense but bear with me
        self._upside = 0

        self._image_width = self._image.get_size()[0]
        self._image_height = self._image.get_size()[1]
        #where on the board is the piece located when played
        self._board_position = (0,0)

    def setBoardPosition(self, board_pos):
        self._board_position = board_pos
        
    #gets new color order as tuple
    def rotatePiece(self, colors):
        self._color1 = colors[0]
        self._color2 = colors[1]
        self._color3 = colors[2]
        dir_path = path.dirname(path.realpath(__file__))
        if self._upside == 0:
            self._image_string = dir_path + "/img/" + self._color1 + self._color2 + self._color3 + ".png"
            self._image = pygame.image.load(self._image_string)
            self._image = pygame.transform.scale(self._image, triangle_size)
            self._image = self._image.convert_alpha()
        else:
            self._image_string = dir_path + "/img/" + self._color3 + self._color1 + self._color2 + ".png"
            self._image = pygame.image.load(self._image_string)
            self._image = pygame.transform.scale(self._image, triangle_size)
            self._image = self._image.convert_alpha()
            self._image = pygame.transform.rotate(self._image,180)
    # def setAngle(self,angle):
    #     self._angle = angle
    #     self._image = pygame.transform.rotate(self._image,self._angle)

    def setX(self,X):
        self._X = X
    
    def setY(self,Y):
        self._Y = Y
   
   
    def printPiece(self):
#        self._rect = self._image.get_rect()
#        self._rect.center = (self._X, self._Y)
#        if(self._angle > 0):
#            self._rect.topleft = (self._image_width - self._X, self._image_height -  self._Y )
#            offset = pygame.math.Vector2((self._image_width,self._image_height)) - self._rect.center
#           rotation_offset = offset.rotate(-self._angle)
#            rotated_image_center = (self._image_width - rotation_offset.x, self._image_height - rotation_offset.y)
#            self._image = pygame.transform.rotate(self._image, self._angle)
#            self._rect = self._image.get_rect(center = (self._X, self._Y))

        self._display.blit(self._image, (self._X,self._Y))
        pygame.display.update()
class cPiece(Structure):
    _fields_ = [('color1', c_int),
                ('color2',c_int),
                ('color3',c_int),
                ('xpos',c_int),
                ('ypos',c_int),
                ('upside_down',c_bool)]