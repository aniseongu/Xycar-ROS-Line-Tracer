#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author: 안성우 (성균관대학교 소프트웨어학과)
# Project: Xycar ROS Line Tracer - Vision Perception Node

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import Float32
from cv_bridge import CvBridge, CvBridgeError

class XycarVisionNode:
    def __init__(self):
        # ROS 노드 초기화
        rospy.init_node('xycar_vision_node', anonymous=True)
        self.bridge = CvBridge()
        
        # Publisher: 계산된 라인 오차값을 '/line_error' 토픽으로 퍼블리시
        self.error_pub = rospy.Publisher('/line_error', Float32, queue_size=10)
        
        # Subscriber: Xycar 전방 카메라 이미지 구독
        self.image_sub = rospy.Subscriber('/usb_cam/image_raw', Image, self.image_callback)
        
        # 검은색 라인 검출을 위한 이진화 임계값 (실차 주행 환경에 맞춰 튜닝)
        self.threshold_val = 60

    def image_callback(self, data):
        try:
            # ROS Image 메시지를 OpenCV 이미지 포맷으로 변환
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            rospy.logerr("CV Bridge Error: {0}".format(e))
            return

        # 1. 전처리: 그레이스케일 변환 및 가우시안 블러로 노이즈 제거
        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray, (5, 5), 0)
        
        # 2. 이진화: 검은색 라인을 흰색(255)으로 분리
        _, thresh = cv2.threshold(blur, self.threshold_val, 255, cv2.THRESH_BINARY_INV)

        # 3. ROI(관심영역) 설정: 카메라 하단 30% 영역만 사용하여 연산 속도 최적화
        h, w = thresh.shape
        roi_top = int(h * 0.7)
        roi = thresh[roi_top:h, 0:w]

        # 4. 무게중심(Moments)을 이용한 라인 중앙 픽셀 위치 계산
        M = cv2.moments(roi)
        if M['m00'] > 0:
            cx = int(M['m10'] / M['m00'])
            # 화면 중앙(w/2)과 라인 중앙(cx)의 오차 계산
            error = cx - (w / 2)
            
            # 오차값 퍼블리시
            self.error_pub.publish(error)
            rospy.loginfo("Detected Line Error: {:.2f}".format(error))
            
if __name__ == '__main__':
    try:
        node = XycarVisionNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
