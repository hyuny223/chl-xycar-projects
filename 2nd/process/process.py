

import cv2
import numpy as np

def make_mask(lu, ru, rd, ld):
    ## 마스크 제작
    mask = np.zeros((480,640), dtype=np.uint8)
    pts = np.array([lu, ru, rd, ld])
    cv2.fillPoly(mask, [pts], [255,255,255])

    return mask


def image_process(frame, mask):

    ## hls에서 중요한 파라미터인 명도와 threshold value 값을 조정하기 위한 트랙바 설정
    fixed = cv2.getTrackbarPos('fixed', 'trackbar')
    value = cv2.getTrackbarPos('value', 'trackbar')

    ## 밝기 평균 고정
    m = cv2.mean(frame, mask)
    scalar = (-int(m[0])+fixed, -int(m[1])+fixed, -int(m[2])+fixed, 0)
    dst = cv2.add(frame, scalar, mask)

    ## 블러
    blur = cv2.GaussianBlur(dst, (0,0), 3.5)
    blur = cv2.GaussianBlur(blur, (0,0), 3.5)
    blur = cv2.GaussianBlur(blur, (0,0), 3.5)
    '''
    가우시안 블러를 한번만 사용하니 잡음이 어느 정도 검출되어서 3번을 주기로 함
    '''

    ## hls 변환
    _, hls, _ = cv2.split(cv2.cvtColor(blur, cv2.COLOR_BGR2HLS))
    tmp = cv2.bitwise_and(hls, mask)

    ## 이진화
    #_, lane = cv2.threshold(tmp, 0, 255, cv2.THRESH_OTSU |cv2.THRESH_BINARY)

    _, lane = cv2.threshold(tmp, value, 255, cv2.THRESH_BINARY)

    return lane
    '''
    오츠를 사용하려 했으나, value값을 변경해도 큰 변화가 없어서 바이너리만 사용하기로 판단
    '''
