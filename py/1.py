import cv2 as cv
import numpy as np



img = cv.imread('opie.jpg')

height, width, _  = img.shape

print(height, ' ', width) 

black = np.zeros((height, 3 * width, 3), dtype = np.uint8)

for i in range(int(height)):
    for j in range(int(width)):
    #    if(0 <= height-i-1 and height-i-1<height and 0 <= j+2*height-2*i-width and j+2*height-2*i-width < width):
            black[height-i-1][j+2*height-2*i] = img[height-i-1][j-1]

black1 = np.zeros((int(height/2), 3*width, 3), dtype=np.uint8)

for i in range(int(height/2)):
    for j in range(3*width):
        black1[i][j] = black[i*2][j]



cv.imshow('IMAGINE', img)
cv.imshow('NEGRU', black)
cv.imshow('compact', black1)
cv.waitKey(0)

cv.destroyAllWindows()

