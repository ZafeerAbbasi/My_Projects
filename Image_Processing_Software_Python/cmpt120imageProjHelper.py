# Some helper functions to wrap the Pygame image functions
# Plus the showInterface function that displays the user interface
# DO NOT modify this file
# Supports load+save for common image formats like PNG, JPG, and BMP
# Pre-requisite: Pygame and Numpy modules imported
# Authors: Angelica Lim, Victor Cheung
# Last update: 16 Nov, 2021

import pygame
import numpy

def getImage(filename):
    """
    Input: filename - string containing image filename to open
    Returns: 3d list of lists (a height-by-width-by-3 list)
    """
    image = pygame.image.load(filename)
    # do a transpose so its rows correspond to height of the image
    return pygame.surfarray.array3d(image).transpose(1, 0, 2).tolist()

def saveImage(pixels, filename):
    """
    Input:  pixels - 3d list of lists of RGB values (a height-by-width-by-3 list)
            filename - string containing filename to save image
    Output: Saves a file containing pixels
    """
    # do a transpose so its rows correspond to width of the image (used by Pygame)
    nparray = numpy.asarray(pixels).transpose(1, 0, 2)
    surf = pygame.surfarray.make_surface(nparray)
    (width, height, colours) = nparray.shape
    surf = pygame.display.set_mode((width, height))
    pygame.surfarray.blit_array(surf, nparray)
    pygame.image.save(surf, filename)

def showImage(pixels):
    """
    Input:  pixels - 3d list of list of RGB values (a height-by-width-by-3 list)
    Output: show the image in a window
    *this function uses the Pygame to display a window in a not-so-conventional way
    (without an event loop) so it might appear frozen.
    Suggested use: use it at the end of the program to show how the image looks like
    and make it stay by a this line:
    print("Press enter to quit")
    """
    # do a transpose so its rows correspond to width of the image (used by Pygame)
    nparray = numpy.asarray(pixels).transpose(1, 0, 2)
    surf = pygame.surfarray.make_surface(nparray)
    (width, height, colours) = nparray.shape
    #surf = pygame.display.set_mode((width, height))
    #pygame.surfarray.blit_array(surf, nparray)
    # for pixels2
    pygame.display.init()
    pygame.display.set_caption("CMPT120 - Image")
    screen = pygame.display.set_mode((width, height))
    screen.fill((225, 225, 225))
    screen.blit(surf, (0, 0))
    pygame.display.update()

def getBlackImage(width, height):
    """
    Input:  width - width of the image
            height - height of the image
    Output: 3d list of lists of a black image (a height-by-width-by-3 list)
    """
    return [[[0, 0, 0] for i in range(width)] for j in range(height)]

def showInterface(pixels, title, textList):
    """
    Input:  pixels - 2d array of RGB values
            title - title of the window
            text - list of strings to be displayed at the bottom of the window
    Output: show the image in a window
    """
    # do a transpose so its rows correspond to width of the image (used by Pygame)
    nparray = numpy.asarray(pixels).transpose(1, 0, 2)
    surf = pygame.surfarray.make_surface(nparray)
    (width, height, colours) = nparray.shape
    # set up the text to be displayed
    fontObj = pygame.font.Font("freesansbold.ttf", 16)
    textObjs = []
    for line in textList:
        textObjs += [fontObj.render(line, False, (0, 0, 0), (225, 225, 225))]
    # find out the largest width within the lines
    maxLineWidth = textObjs[0].get_width()
    for lo in textObjs:
        if maxLineWidth < lo.get_width():
            maxLineWidth = lo.get_width()
    # find out the width of the screen, then pad 20 pixels
    screenWidth = max(width, maxLineWidth) + 20
    # set up the display
    pygame.display.set_caption(title + " (" + str(width) + "x" + str(height) + ")")
    screen = pygame.display.set_mode((screenWidth, height + textObjs[0].get_height()*len(textObjs) + 10))
    screen.fill((225, 225, 225))
    # add the image to the top middle of the display
    screen.blit(surf, ((screenWidth-width)/2, 0))
    # add the texts to the display, add 10 pixels below the image
    textHeight = height + 10
    for textObj in textObjs:
        screen.blit(textObj, (10, textHeight))
        textHeight += textObj.get_height()
    # display everything
    pygame.display.update()

def rgb_to_hsv(r, g, b):
    """
    Input:  r, g, b - the R/G/B values of a pixel in the RGB colour space
    Output: a tuple of the Hue, Saturation, and Value representation of a pixel
    From https://www.w3resource.com/python-exercises/math/python-math-exercise-77.php
    """
    r, g, b = r/255.0, g/255.0, b/255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    df = mx-mn
    if mx == mn:
        h = 0
    elif mx == r:
        h = (60 * ((g-b)/df) + 360) % 360
    elif mx == g:
        h = (60 * ((b-r)/df) + 120) % 360
    elif mx == b:
        h = (60 * ((r-g)/df) + 240) % 360
    if mx == 0:
        s = 0
    else:
        s = (df/mx)*100
    v = mx*100
    return h, s, v
