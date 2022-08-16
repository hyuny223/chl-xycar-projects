#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import rospy, rospkg
import numpy as np
import cv2, random, math
from cv_bridge import CvBridge
from xycar_motor.msg import xycar_motor
from sensor_msgs.msg import Image

import sys, os, signal
from src import subscriber


def signal_handler(sig, frame):
    os.system('killall -9 python rosout')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)


if __name__ == '__main__':
    rospy.init_node('auto_drive')

    p, i, d = 0.6, 0.0006, 0.025
    xycar = subscriber.Subscriber("huff",p, i, d)

    rospy.spin()



