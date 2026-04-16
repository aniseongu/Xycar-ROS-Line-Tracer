#!/usr/bin/env python3
import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import Float32
from cv_bridge import CvBridge, CvBridgeError

class VisionNode:
    def __init__(self):
        rospy.init_node('vision_node', anonymous=True)
        self.bridge = CvBridge()
        
        # Publisher: 계산된 라인 오차값을 퍼블리시
        self.error_pub = rospy.Publisher('/line_error', Float32, queue_size=10)
        
        # Subscriber: 카메라 노드에서 나오는 이미지 구독 (토픽명은 환경에 따라 다를 수 있음)
        self.image_sub = rospy.Subscriber('/usb_cam/image_raw', Image, self.image_callback)
        
        self.threshold_val = 60

    def image_callback(self, data):
        try:
            # ROS 이미지를 OpenCV 이미지로 변환
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            rospy.logerr(e)
            return

        # 1. 전처리 및 이진화
        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray, (5, 5), 0)
        _, thresh = cv2.threshold(blur, self.threshold_val, 255, cv2.THRESH_BINARY_INV)

        # 2. 하단 ROI(관심영역) 설정
        h, w = thresh.shape
        roi = thresh[int(h*0.7):h, 0:w]

        # 3. 라인 중심점(Centroid) 계산
        M = cv2.moments(roi)
        if M['m00'] > 0:
            cx = int(M['m10']/M['m00'])
            error = cx - (w / 2)
            
            # 오차값을 /line_error 토픽으로 퍼블리시
            self.error_pub.publish(error)
            # 디버깅용 로그
            rospy.loginfo(f"Line Error: {error}")
            
if __name__ == '__main__':
    try:
        VisionNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
