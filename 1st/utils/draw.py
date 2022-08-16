import cv2
import random

####################### Brightness #########################
def draw_box(show, bin, l_pos, r_pos, c_pos):
    global box_w, box_h, width, offset
    show_bin = cv2.cvtColor(bin, cv2.COLOR_GRAY2BGR)
    cv2.rectangle(show, (l_pos - box_w, offset), (l_pos, offset + box_h), (255, 0, 0), 1)
    cv2.rectangle(show, (r_pos, offset), (r_pos + box_w, offset + box_h), (0, 0, 255), 1)
    cv2.rectangle(show, (c_pos - box_w/2, offset), (c_pos + box_w/2, offset + box_h), (0, 255, 0), 1)
    cv2.rectangle(show, (width/2 - box_w/2, offset), (width/2 + box_w/2, offset + box_h), (255, 255, 255), 2)

    cv2.rectangle(show_bin, (l_pos - box_w, offset), (l_pos, offset + box_h), (255, 0, 0), 1)
    cv2.rectangle(show_bin, (r_pos, offset), (r_pos + box_w, offset + box_h), (0, 0, 255), 1)
    cv2.rectangle(show_bin, (c_pos - box_w/2, offset), (c_pos + box_w/2, offset + box_h), (0, 255, 0), 1)
    cv2.rectangle(show_bin, (width/2 - box_w/2, offset), (width/2 + box_w/2, offset + box_h), (255, 255, 255), 2)

    dst = cv2.hconcat([show, show_bin])
    return dst



######################## Huff ###########################
def draw_lines(img, lines):
    global Offset
    for line in lines:
        x1, y1, x2, y2 = line[0]
        color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
        img = cv2.line(img, (x1, y1+Offset), (x2, y2+Offset), color, 2)
    return img

# draw rectangle
def draw_rectangle(img, lpos, rpos, offset=0):
    center = (lpos + rpos) / 2

    cv2.rectangle(img, (lpos - 5, 15 + offset),
                       (lpos + 5, 25 + offset),
                       (0, 255, 0), 2)
    cv2.rectangle(img, (rpos - 5, 15 + offset),
                       (rpos + 5, 25 + offset),
                       (0, 255, 0), 2)
    cv2.rectangle(img, (center-5, 15 + offset),
                       (center+5, 25 + offset),
                       (0, 255, 0), 2)
    cv2.rectangle(img, (315, 15 + offset),
                       (325, 25 + offset),
                       (0, 0, 255), 2)
    return img
