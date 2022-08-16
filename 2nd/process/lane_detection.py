import numpy as np
import cv2

from utils import draw

def lane_detection(lane, row, col):
    ## lpos, rpos 찾기. 안쪽에서 바깥쪽
    
    center = col // 2
    left, right = -1, -1
    lpos, rpos = 0, col

    for l in range(center-65, -1, -1):
        if lane[400, l] == 0:
            lpos = l
            left = 1
            break

    for r in range(center+65, col):
        if lane[400, r] == 0:
            rpos = r
            right = 1
            break
    '''
    블러로 인하여 바이너리 경계가 넓어져도 라이다와 겹치지 않으면서도,
    라이다와 차선이 겹치는 경우 최대한으로 차선을 검출할 수 있도록 하는 경계를 center±65로 잡음
    '''

    if lpos > 5:
        lpos -= 5
    if rpos < 635:
        rpos += 5

    return lpos, rpos, left, right




    ############################# 차선저장 #############################
    ##################################################################

    if cnt == 30:
        cnt = 0
        line_tmp[0].append(index)
        line_tmp[1].append(lpos)
        line_tmp[2].append(rpos)
        print(f'index: {index}, lpos:{lpos}, rpos:{rpos}')
        print("-------------------------")
        index+=1

    ############################# 자매품 #############################
    ##################################################################

    # 칼만필터: 차선이 없는 경우, 이전 프레임에서 예측한 차선을 현재의 차선으로 선택
    # 라이다가 차선을 가리는 구간은 잘 추정하는데, 곡선으로 인하여 차선이 없어지는 경우는 값 변동이 심하다

