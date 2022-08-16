import cv2
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from xycar_msgs.msg import xycar_motor

from process import control, lane_detection

import numpy as np

width, height = 640, 480
no_l, no_r = 0, 0

# cv 브릿지
bridge = CvBridge()
cv_img = np.empty(shape=[0])


################# 이동평균 필터 상수 ######################
k_l, k_r = 0, 0                     # k번 째 수 의미
preAvg_l, preAvg_r = 0, 0           # 이전의 평균 값


class Subscriber:
    def __init__(self, mode, p = 0.6, i = 0.0006, d = 0.025):
        rospy.Subscriber("/usb_cam/image_raw/", Image, self.img_callback)
        self.pub = rospy.Publisher("xycar_motor", xycar_motor, queue_size=1)
        self.msg = xycar_motor()
        self.mode = mode
        self.p = p
        self.i = i
        self.d = d
        # 0.6, 0.0006, 0.025

    # 콜백 함수 선언
    def img_callback(self, data):
        self.cv_img = bridge.imgmsg_to_cv2(data, "bgr8")
        if cv_img.size != (width*height*3):
            return
        if(self.mode == "brightness"):
            self.process_brightness()
        elif(self.mode == "huff"):
            self.process_huff()

    # publish xycar_motor msg
    def drive(self, Angle, Speed): 

        self.msg.angle = Angle
        self.msg.speed = Speed

        self.pub.publish(self.msg)

    def process_brightness(self):

        tm= cv2.TickMeter()
        tm.start()
        show, bin, bin_roi = lane_detection.process_image_brightness(cv_img)
        l_pos, r_pos = lane_detection.get_line_pos_brightness(bin_roi)
        k_l, preAvg_l, l_buf, l_pos = control.movAvgFilter(l_pos, k_l, preAvg_l, l_buf)
        k_r, preAvg_r, r_buf, r_pos = control.movAvgFilter(r_pos, k_r, preAvg_r, r_buf)
        init_l = l_pos + 50                         # init_l, init_r 재설정
        init_r = r_pos - 50
        if init_l < 0:
            init_l = 50
        if init_r > width:
            init_r = width - 50
        c_pos = (l_pos + r_pos) / 2
        error = c_pos - width/2
        pid = control.PID(self.p, self.i, self.d) # p i d
        if no_l == 1:
            angle, speed = 50, -50
        elif no_r == 1:
            angle, speed = -50, -50
        else:
            angle = pid.pid_process(error)
            speed = 50

        self.drive(angle, speed)

    def process_huff(self):
        ll = control.MovingAverage(5)
        rr = control.MovingAverage(5)

        lpos, rpos = lane_detection.process_image_huff(self.cv_img)


        ll.add_samples(lpos)
        rr.add_samples(rpos)

        lpos = ll.get_wmm()
        rpos = rr.get_wmm()

        pid = control.PID(self.p, self.i, self.d)

	# 좌회전일 때
        if (lpos == 0 and rpos < 640):
            e = rpos - width/2
            lpos -= e
            center = (lpos + rpos) / 2
            error = center - width/2
            angle = pid.pid_process(error)

            # 일정 속도 이상이면 서서히 감속 
            if (speed > 25):
                speed -= 0.5
	# 우회전일 때
        elif (0 < lpos and rpos == 640):
            e = width/2 - lpos
            rpos += e
            center = (lpos + rpos)/2
            error = center - width/2
            angle = pid.pid_control(error)

            # 일정 속도 이상이면 서서히 감속
            if (speed > 25):
                speed -= 0.5

	# 두 차선을 모두 잃은 경우, 각도를 유지하고, 속도는 감속
        elif (0 == lpos and rpos == 640):
            # angle = -angle
            speed -= 0.5

        else:
            center = (lpos + rpos)/2
            error = (center - width/2)
            angle = pid.pid_control(error)
            speed = 40

        self.drive(angle,speed)
