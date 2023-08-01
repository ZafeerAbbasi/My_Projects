# CMPT 120 Yet Another Image Processer
# Starter code for cmpt120imageManip.py
# Author(s): Zafeer Abbasi
# SFU ID: 301394323
# Platform Used: replit.com
# Date: 2021-12-05
# Description:
 
import cmpt120imageProjHelper
import numpy

import copy
# this module is only used for the finding fish function ^



# defining function to apply red filter
def applyRedFilter(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      newpixel = [r, 0, 0]
      newimage[row][col] = newpixel

  return newimage





# defining function to apply green filter
def applyGreenFilter(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      newpixel = [0, g, 0]
      newimage[row][col] = newpixel

  return newimage






# defining function to apply blue filter
def applyBlueFilter(pixels):  
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      newpixel = [0, 0, b]
      newimage[row][col] = newpixel

  return newimage






# defining function to apply sepia filter
def applySepiaFilter(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]


      sepia_red = int((r * 0.393) + (g * 0.769) + (b * 0.189))
      sepia_green = int((r * 0.349) + (g * 0.686) + (b * 0.168))
      sepia_blue = int((r * 0.272) + (g * 0.534) + (b * .131))


      newpixel = [min(sepia_red, 255), min(sepia_green, 255), 
      min(sepia_blue, 255)]
      
      newimage[row][col] = newpixel

  return newimage
  


# defining a function that does the scale up calculation
###### WILL BE USED IN THE WARM AND COLD FILTER FUNCTION #####
###### WILL NOT RETURN ANY ARRAY, THIS FUNCTION IS SIMPLY FOR CALCULATION PURPOSES####
def scaleup(value):

  if value < 64:
    value = (value/64) * 80
  elif value >= 64 and value < 128:
    value = (value - 64)/(128 - 64) * (160 - 80) + 80
  else:
    value = (value - 128)/(255-128) * (255 - 160) + 160

  return int(value)


# defining a function that does the scale down function
###### WILL BE USED IN THE WARM AND COLD FILTER FUNCTION #####
###### WILL NOT RETURN ANY ARRAY, THIS FUNCTION IS SIMPLY FOR CALCULATION PURPOSES####
def scaledown(value):

  if value < 64:
    value = (value/64) * 50
  elif value >= 64 and value < 128:
    value = (value - 64)/(128 - 64) * (100 - 50) + 50
  else:
    value = (value - 128)/(255-128) * (255 - 100) + 100 

  return int(value)




# defining a function to apply Warm filter
def applyWarmFilter(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      newpixel = [scaleup(r), g , scaledown(b)]
      newimage[row][col] = newpixel

  return newimage



# defining a function to apply Cold filter
def applyColdFilter(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width, height)

  # looping through every pixel
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      newpixel = [scaledown(r), g , scaleup(b)]
      newimage[row][col] = newpixel

  return newimage




# defining function to rotate left by 90 degrees
def rotate_left(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(height, width)

  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      newimage[width - 1 - col][row] = pixel
    

  return newimage






# defining function to rotate right by 90 degrees
def rotate_right(pixels):
  # dimensions of image
  height = len(pixels)
  width = len(pixels[0])

  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(height, width)

  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      newimage[col][height - 1 - row] = pixel
      
    

  return newimage







# defining function to double size
def doublesize(pixels):

  height = len(pixels)
  width = len(pixels[0])


  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(width * 2, height * 2)

  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]


      # all the pixels in the new image that surround the pixel in the original..
      #.. are given the same value as the pixel in the original
      newimage[2*row][2*col] = pixel
      newimage[2*row][2*col + 1] = pixel
      newimage[2*row + 1][2*col] = pixel
      newimage[2*row + 1][2*col + 1] = pixel

  return newimage







# defining function to half size
def halfsize(pixels):
  height = len(pixels)
  width = len(pixels[0])


  # creating a blank black canvas
  newimage = cmpt120imageProjHelper.getBlackImage(int(width / 2), int(height / 2))

  for row in range( int(height / 2)):
    for col in range(int(width / 2)):
      pixel = newimage[row][col]
      r = pixel[0]
      g = pixel[1]
      b = pixel[2]

      # These are the pixels that will be used to take the average value for the..
      # ..new image
      pixel1 = pixels[2*row][2*col] 
      pixel2 = pixels[2*row][2*col + 1] 
      pixel3 = pixels[2*row + 1][2*col] 
      pixel4 = pixels[2*row + 1][2*col + 1] 


      # Taking the average r,g,b values for the previous pixels
      average_r = int((pixel1[0] + pixel2[0] + pixel3[0] + pixel4[0])/4)
      average_g = int((pixel1[1] + pixel2[1] + pixel3[1] + pixel4[1])/4)
      average_b = int((pixel1[2] + pixel2[2] + pixel3[2] + pixel4[2])/4)

      # assigning the pixel in the new image the average values that was calculated..
      #.. previously
      newimage[row][col] = [average_r, average_g, average_b]
  
  return newimage

 




# defining function to check if pixel is yellow
###### WILL BE USED IN THE LOCATE FISH FUNCTION #####
###### WILL NOT RETURN ANY ARRAY, THIS FUNCTION IS SIMPLY FOR SIMPLIFYING PURPOSES####
def isyellow(pixel):
  r = pixel[0]
  g = pixel[1]
  b = pixel[2]



  hue = cmpt120imageProjHelper.rgb_to_hsv(r,g,b)[0]
  saturation = cmpt120imageProjHelper.rgb_to_hsv(r,g,b)[1]
  value = cmpt120imageProjHelper.rgb_to_hsv(r,g,b)[2]


  # if the pixel meets the criteria, then return True
  return 30 < hue < 120 and  47 < saturation and 47 < value 





# defining function to draw box
###### WILL BE USED IN THE LOCATE FISH FUNCTION #####
###### WILL NOT RETURN ANY ARRAY, THIS FUNCTION IS SIMPLY FOR SIMPLIFYING PURPOSES####
def drawbox(max_Y, min_Y, max_X, min_X, pixels):


  newimage = copy.deepcopy(pixels)

  
  # creating horizontal line from top left corner to top right
  for i in range(min_X, max_X, 1):
    newimage[max_Y][i] = [0, 255, 0]
  
  # creating vertical line from top right corner to bottom right corner
  for i in range(max_Y, min_Y, 1):
    newimage[i][max_X] = [0, 255, 0]

  # creating a horizontal line from bottom right corner to bottom left
  for i in range(max_X, min_X, -1):
    newimage[min_Y][i] = [0, 255, 0]

  # creating a vertical line from bottom left to top left corner
  for i in range(min_Y, max_Y, -1):
    newimage[i][min_X] = [0, 255, 0]


  return newimage


# defining function to Locate fish
def locatefish(pixels):
  height = len(pixels)
  width = len(pixels[0])

  row_values = []
  col_values = []

  # looping through image and adding any yellow pixel locations to certain lists
  for row in range(height):
    for col in range(width):
      pixel = pixels[row][col]
      if isyellow(pixel): 
        row_values.append(row)
        col_values.append(col)  

  # using max() and min() to find the dimensions of the box for the fish
  max_Y = min(row_values)
  min_Y = max(row_values)
  min_X = min(col_values)
  max_X = max(col_values)

  # calling function to draw the box using the dimensions determined previously
  newpicture = drawbox(max_Y, min_Y, max_X, min_X, pixels)

  return newpicture



