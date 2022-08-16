#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2, rospy
import numpy as np

from xycar_msgs.msg import xycar_motor
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

from src import subscriber

################## 상수 ########################################
width, height = 640, 480            # 영상의 크기
init_l, init_r = 320, 320           # 좌우 차선 차선 탐색을 위한 픽셀 시작 점
roi_w, roi_h = 200, 20              # 좌우 차선 ROI 영역의 크기 값
lane_width = 440                    # 차선 너비 픽셀
no_l, bo_r = 0, 0
speed = 50
################## 변수 ########################################
box_w, box_h = 15, 20               # 차선 체크박스 크기
l_pos, r_pos = box_w / 2, width - box_w /2      # 위치 초기화
offset = 340                        # ROI 창의 높이
pixel_threshold = int(0.4 * box_w * box_h)   # 차선 체크박스 임계값
k_p, k_i, k_d = 3, 0, 0             # PID 변수



if __name__ == "__main__":

    rospy.init_node('line_follow')
    p, i, d = 0.6, 0.0006, 0.025
    xycar = subscriber.Subscriber("brightness")
    rospy.spin()

