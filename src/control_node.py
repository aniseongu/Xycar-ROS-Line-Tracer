#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Author: 안성우 (성균관대학교 소프트웨어학과)
# Project: Xycar ROS Line Tracer - PID Control Node

import rospy
from std_msgs.msg import Float32
from ackermann_msgs.msg import AckermannDriveStamped

class XycarControlNode:
    def __init__(self):
        rospy.init_node('xycar_control_node', anonymous=True)
        
        # 파라미터 서버에서 PID 게인 값 가져오기 (launch 파일에서 수정 가능)
        self.kp = rospy.get_param('~kp', 0.4)
        self.ki = rospy.get_param('~ki', 0.001)
        self.kd = rospy.get_param('~kd', 0.15)
        
        self.prev_error = 0
        self.integral = 0
        self.base_speed = 0.5  # Xycar 기본 전진 속도
        
        # Publisher: Xycar 조향 및 모터 제어 명령 퍼블리시 (Ackermann 타입)
        self.drive_pub = rospy.Publisher('/xycar_motor', AckermannDriveStamped, queue_size=1)
        
        # Subscriber: 비전 노드에서 발행한 오차값 구독
        self.error_sub = rospy.Subscriber('/line_error', Float32, self.error_callback)

    def error_callback(self, msg):
        error = msg.data
        
        # 1. PID 제어 로직
        self.integral += error
        derivative = error - self.prev_error
        
        steering_output = (self.kp * error) + (self.ki * self.integral) + (self.kd * derivative)
        self.prev_error = error
        
        # 2. Xycar 구동 메시지 생성
        drive_msg = AckermannDriveStamped()
        drive_msg.drive.speed = self.base_speed
        
        # 오차 방향에 맞춰 조향각 적용 (하드웨어 세팅에 따라 부호 반전 가능)
        drive_msg.drive.steering_angle = -steering_output 
        
        # 명령 퍼블리시
        self.drive_pub.publish(drive_msg)
        rospy.loginfo("PID Steering Output: {:.2f}".format(steering_output))

if __name__ == '__main__':
    try:
        node = XycarControlNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
