import serial
import time
import cv2

from PIL import ImageGrab
import numpy as np

arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)
time.sleep(1) # give the connection a second to settle

# ------ camera drivers not working ------
# cap = cv2.VideoCapture(0)

while(True):
    # ------ 1. Read each frame ------
    # A specific part of the computer frame is captured
    imgread = ImageGrab.grab(bbox=(62,104,650,533)) #bbox specifies specific region (bbox= x,y,width,height)
    img = np.array(imgread)


    # ------ 2. Flip each frame for it to mirror ------
    img = cv2.flip(img, 1)


    # ------ visual -------
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    full_height, full_width = img.shape[:2] # get full height and width for resizing and use for visual representation


    # ------ 3. Resize frame to desired size ------
    desired_height, desired_width = (2, 4) # set desired height and width (number of pixels)
    img = cv2.resize(img, (desired_width, desired_height), interpolation=cv2.INTER_LINEAR) # create resized image in the desired size


    # ------ visual -------
    pixel_big = cv2.resize(img, (full_width, full_height), interpolation=cv2.INTER_NEAREST) # show full window
    pixel_gray = cv2.cvtColor(pixel_big, cv2.COLOR_BGR2GRAY) # grayscale, for better representation of the mirror
    cv2.imshow('img', img_gray)
    cv2.imshow('img2', pixel_gray)


    # ------ 4. Convert BGR (Blue, Green, Red) to HSV-format (Hue, Saturation, Value) ------
    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)


    # ------ 5. Merge each pixels brightness-value to a String, frameValues ------
    frameValues = ""
    for j in range(0, desired_height): # for every "row"
        for i in range(0, desired_width): # for every "column"
            frameValues += str(round(img[j,i,2])) # add to String (array)
            if not (j == (desired_height-1) and i == (desired_width-1)): # separate each servo-value with ":"
                frameValues += ":"


    # ------ 6. Send all values to arduino, string with format: (XX:XX:XX:XX:XX:XX:XX:XX) ------
    arduino.write(bytes(frameValues, 'utf-8'))


    k = cv2.waitKey(10) & 0xFF
    if k == 27: # ESC to break program
        break

    

cv2.destroyAllWindows()