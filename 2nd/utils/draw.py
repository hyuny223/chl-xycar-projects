import cv2

def draw_lane(lpos, rpos, left, right, frame):
    if left:
        cv2.rectangle(frame, (lpos - 10, 390),(lpos + 10, 410), (0, 255, 0), 3)

    if right:
        cv2.rectangle(frame, (rpos - 10, 390),(rpos + 10, 410), (0, 255, 0), 3)

    return frame


def draw_predicted(lpos, rpos, frame):
    cv2.rectangle(frame, (lpos - 5, 395),(lpos + 5, 405), (0, 0, 255), 3)
    cv2.rectangle(frame, (rpos - 5, 395),(rpos + 5, 405), (0, 0, 255), 3)

    return frame
