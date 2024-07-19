import cv2 as cv
import numpy as np



img = cv.imread('skull.jpg')

height, width, _  = img.shape

#print(height, ' ', width) 

black = np.zeros((4*height, width, 3), dtype = np.uint8)

# Aflu preimaginea imaginii initiale prin aplicatia liniara f aplicata fiecarui vector de poz al fiecarui pixel cu M(f) = ((1, 1/2), (1, 0)) (unde in paranteze se afla coloanele matricei)
# In loc de a rezola ecuatia v = f^(-1)(w), unde vrem sa il aflam pe v si il cunoastem pe w, procedez astfel:
# Atribui fiecarui pixel determinat de M(f) * v = w, unde w are coordonatele din imaginea 'skull.jpg'
# 'height - i - 1' reporezinta coodonata pe axa x cu care lucram

for i in range(int(height)):
    for j in range(int(width)):
            black[(height-i)+int(0.5*j)-1][j] = img[height-i-1][j]

# Imaginii obtinute ii aplicam inca o transformare prin care ingustam imaginea cu un factor de 1/4

for i in range(4*height):
    for j in range(int(width/4)-1):
            black[i][j] = (max(black[i][4*j][0],black[i][4*j+1][0]), max(black[i][4*j][1],black[i][4*j+1][1]), max(black[i][4*j][2],black[i][4*j+1][2])) 

# Din imaginea obtinuta 'black' selectam ulterior doar regiunea care ne intereseaza, anume craniul

FINAL_IMG = black[int(0.8*height):int(1.15*height), 0:int(width/4-1)] 

#cv.imshow('IMAGINE', img)

cv.imwrite('Preliminar.jpg', black)             # Imaginea preliminara
cv.imwrite('Imagine Finala.jpg', FINAL_IMG)     # Imaginea finala

# cv.waitKey(0)

# cv.destroyAllWindows()

