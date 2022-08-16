
import cv2

def onChange(arg):
    pass

class Trackbar:
    def __init__(self):
        cv2.namedWindow("trackbar")
        cv2.createTrackbar('value','trackbar', 56, 255, onChange) # best : 56
        cv2.createTrackbar('fixed','trackbar', 70, 255, onChange) # best : 70
