#!/usr/bin/env python3
import rospy
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist

class ControlNode:
    def __init__(self):
        rospy.init_node('control_node', anonymous=True)
        
        # PID 파라미터 (rospy.get_param을 쓰면 launch 파일에서 수정 가능해 더 전문적임)
        self.kp = rospy.get_param('~kp', 0.01)
        self.ki = rospy.get_param('~ki', 0.0001)
        self.kd = rospy.get_param('~kd', 0.005)
        
        self.prev_error = 0
        self.integral = 0
        self.base_speed = 0.2  # 기본 전진 속도
        
        # Publisher: 로봇의 바퀴/조향을 제어하는 표준 토픽(cmd_vel)
        self.cmd_pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        
        # Subscriber: 비전 노드에서 계산한 오차값 구독
        self.error_sub = rospy.Subscriber('/line_error', Float32, self.error_callback)

    def error_callback(self, msg):
        error = msg.data
        
        # PID 계산
        self.integral += error
        derivative = error - self.prev_error
        steering_output = (self.kp * error) + (self.ki * self.integral) + (self.kd * derivative)
        self.prev_error = error
        
        # Twist 메시지 생성 및 퍼블리시
        twist = Twist()
        twist.linear.x = self.base_speed       # 직진 속도
        twist.angular.z = -steering_output     # 조향각 (방향에 따라 부호 반전 필요)
        
        self.cmd_pub.publish(twist)
        rospy.loginfo(f"Published cmd_vel -> Linear: {twist.linear.x}, Angular: {twist.angular.z}")

if __name__ == '__main__':
    try:
        ControlNode()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
