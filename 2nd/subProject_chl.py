#!/usr/bin/env python3

import argparse
import cv2
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from kalman import kalman
from utils import trackbar, draw, save
from process import process, lane_detection

'''
칼만필터 이해해보기

  img = cv2.imread('/home/chl/Pictures/1.png')

  ball_point = [(100,100),(150,100),(200,100),
                (250,100),(300,100),(350,100),
                (400,100),(450,100),(500,100)]

  for pt in ball_point:
    cv2.circle(img,pt,15,(0,0,255),-1)

    predicted = kf.predict(pt[0],pt[1])

    cv2.circle(img,(predicted[0],predicted[1]), 15, (255,0,0), 4)
'''

def parse():
  args = argparse.ArgumentParser(description="Lane Detection with Brightness Image Process")

  args.add_argument("--video_path",
                    type=str,
                    help="put the path of your video on which you want to detect lane")

  parser = args.parse_args()

  path = parser.video_path

  return path


def main(path):
	# "/home/chl/Videos/subProject.avi"
	cap = cv2.VideoCapture(path)

	if (not cap.isOpened()):
		print("Open Falied")
		return -1

	col = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	row = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
	center = col // 2

	mask = process.make_mask([0,300],[640,300],[640,440],[0,440])

	## 칼만필터 선언
	kf = kalman.Kalman()

	## lpos, rpos 저장 공간
	line_tmp = [['index'],['lpos'],['rpos']]
	cnt = 0
	index = 0

	while True:
		ret, frame = cap.read()

		if not ret: 
			break

		# 횟수 세기
		cnt += 1

		# 영상 처리
		lane = process.image_process(frame, mask)

		# 차선 검출 
		lpos, rpos, left, right = lane_detection.lane_detection(lane, row, col)
		frame = draw.draw_lane(lpos, rpos, left, right, frame)

		# # 비교값 저장
		# if cnt == 30:
		# 	cnt = 0
		# 	line_tmp[0].append(index)
		# 	line_tmp[1].append(lpos)
		# 	line_tmp[2].append(rpos)
		# 	print(f'index: {index}, lpos:{lpos}, rpos:{rpos}')
		# 	print("-------------------------")
		# 	index+=1

		if left == -1:
			lpos = predicted[0]

		if right == -1:
			rpos = predicted[1]

		predicted = kf.predict(lpos,rpos)

		frame = draw.draw_predicted(lpos, rpos, frame)

		cv2.imshow('lane',lane)
		cv2.imshow("img",frame)
		if (cv2.waitKey(1) == 27):
			break

	# save.save_pos(line_tmp)


if __name__ == '__main__':
  path = parse()
  bar = trackbar.Trackbar()
  main(path)
