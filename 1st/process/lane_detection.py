import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from xycar_msgs.msg import xycar_motor


import math, random
import numpy as np
from utils import draw

# ================================== Brightness =======================================
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))


def process_image_brightness(cv_img):
    global roi_h, offset, low, high
    show = cv_img.copy()
    gray = cv2.cvtColor(cv_img, cv2.COLOR_BGR2GRAY)
    conv = 255 - gray
    _, output = cv2.threshold(conv, 115, 255, cv2.THRESH_BINARY)
    erode = cv2.erode(output, kernel)
    bin = cv2.dilate(erode, kernel)
    bin_roi = bin[offset : (offset + roi_h) + 1, :].copy()
    return show, bin, bin_roi


# 차선 좌표 검출 함수
def get_line_pos_brightness(bin_roi):
    global box_w, box_h, pixel_threshold, width, lane_width, l_pos, r_pos, init_l, init_r, no_l, no_r

    # 왼쪽 차선 검출
    for l in range(init_l, 0 - 1, -1):                  # init_l부터 왼쪽으로 탐색
        box = bin_roi[0:box_h + 1, l - box_w:l + 1]     # 박스 크기 설정
        if cv2.countNonZero(box) > pixel_threshold:     # 차선 탐색 조건문
            l_pos = l                                   # 왼쪽 차선 검출
            no_l = 0
            # init_l = l_pos + 50                         # init_l 재설정
            break
    # 왼쪽 차선이 검출되지 않았다면 
    else:
        l_pos = r_pos - lane_width                      # 왼쪽 차선을 오른쪽 차선 기준으로 보정
        if l_pos < - 100:
            no_l = 1
        # init_l = 50                                     # init_l을 50으로 설정

    # 오른쪽 차선 검출
    for r in range(init_r, width - 1):                  # init_r부터 오른쪽으로 탐색
        box = bin_roi[0:box_h + 1, r:r + box_w + 1]     # 박스 크기 설정
        if cv2.countNonZero(box) > pixel_threshold:     # 차선 탐색 조건문
            r_pos = r                                   # 오른쪽 차선 검출
            no_r = 0
            # init_r = r_pos - 50                         # init_r 재설정
            break
    # 오른쪽 차선이 검출되지 않았다면 
    else:
        r_pos = l_pos + lane_width                      # 오른쪽 차선을 왼쪽 차선 기준으로 보정
        if r_pos > width +  100:
            no_r = 1
        # init_r = width - 50                             # init_r을 50으로 설정
    # 중간 픽셀 계산
    # c_pos = (l_pos + r_pos) /2
    return l_pos, r_pos#, c_pos

# ============================== Huff Transform ===========================

# left lines, right lines
def divide_left_right(lines):
    global Width

    low_slope_threshold = 0
    high_slope_threshold = 10

    # calculate slope & filtering with threshold
    slopes = []
    new_lines = []

    for line in lines:
        x1, y1, x2, y2 = line[0]

        if x2 - x1 == 0:
            slope = 0
        else:
            slope = float(y2-y1) / float(x2-x1)
        
        if abs(slope) > low_slope_threshold and abs(slope) < high_slope_threshold:
            slopes.append(slope)
            new_lines.append(line[0])

    # divide lines left to right
    left_lines = []
    right_lines = []

    for j in range(len(slopes)):
        Line = new_lines[j]
        slope = slopes[j]

        x1, y1, x2, y2 = Line

        if (slope < 0) and (x2 < Width/2 - 90):
            left_lines.append([Line.tolist()])
        elif (slope > 0) and (x1 > Width/2 + 90):
            right_lines.append([Line.tolist()])

    return left_lines, right_lines

# get average m, b of lines
def get_line_params(lines):
    # sum of x, y, m
    x_sum = 0.0
    y_sum = 0.0
    m_sum = 0.0

    size = len(lines)
    if size == 0:
        return 0, 0

    for line in lines:
        x1, y1, x2, y2 = line[0]

        x_sum += x1 + x2
        y_sum += y1 + y2
        m_sum += float(y2 - y1) / float(x2 - x1)

    x_avg = x_sum / (size * 2)
    y_avg = y_sum / (size * 2)
    m = m_sum / size
    b = y_avg - m * x_avg

    return m, b

# get lpos, rpos
def get_line_pos_huff(img, lines, left=False, right=False):
    global Width, Height
    global Offset, Gap

    m, b = get_line_params(lines)

    if m == 0 and b == 0:
        if left:
            pos = 0
        if right:
            pos = Width
    else:
        y = Gap / 2
        pos = (y - b) / m

        b += Offset
        x1 = (Height - b) / float(m)
        x2 = ((Height/2) - b) / float(m)

        cv2.line(img, (int(x1), Height), (int(x2), (Height/2)), (255, 0,0), 3)

    return img, int(pos)

# show image and return lpos, rpos
def process_image_huff(frame):
    global Width
    global Offset, Gap
    global pts1, pts2 

    # gray
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

    # perspective
    M = cv2.getPerspectiveTransform(pts1,pts2)
    Mmin = cv2.getPerspectiveTransform(pts2,pts1)

    gray = cv2.warpPerspective(gray,M, (640,480), flags=cv2.INTER_LINEAR)

    # blur
    kernel_size = 5
    blur_gray = cv2.GaussianBlur(gray,(kernel_size, kernel_size), 0)

    # canny edge
    low_threshold = 60
    high_threshold = 70
    edge_img = cv2.Canny(np.uint8(blur_gray), low_threshold, high_threshold)

    # HoughLinesP
    roi = edge_img[Offset : Offset+Gap, 0 : Width]
    all_lines = cv2.HoughLinesP(roi,1,math.pi/180,30,30,10)

    # divide left, right lines
    if all_lines is None:
        return 0, 640
    left_lines, right_lines = divide_left_right(all_lines)

    # get center of lines
    frame, lpos = get_line_pos_huff(frame, left_lines, left=True)
    frame, rpos = get_line_pos_huff(frame, right_lines, right=True)	

    # draw lines
    frame = draw.draw_lines(frame, left_lines)
    frame = draw.draw_lines(frame, right_lines)
    frame = cv2.line(frame, (230, 235), (410, 235), (255,255,255), 2)

    # draw rectangle
    frame = draw.draw_rectangle(frame, lpos, rpos, offset=Offset)
    #roi2 = cv2.cvtColor(roi, cv2.COLOR_GRAY2BGR)
    #roi2 = draw.draw_rectangle(roi2, lpos, rpos)

    # show image
    cv2.imshow('calibration', frame)

    return lpos, rpos
