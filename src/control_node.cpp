#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include "xycar_lib.h"

class ControlNode {
    ros::NodeHandle nh;
    ros::Subscriber lane_sub, ultra_sub;
    ros::Publisher drive_pub;

    PID pid_controller;
    MedianFilter filter;
    int current_midpoint = 320;

public:
    ControlNode() : pid_controller(0.4, 0.001, 0.15) {
        lane_sub = nh.subscribe("/lane_midpoint", 1, &ControlNode::laneCallback, this);
        ultra_sub = nh.subscribe("/xycar_ultrasonic", 1, &ControlNode::ultraCallback, this);
        drive_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("/xycar_motor", 1);
    }

    void laneCallback(const std_msgs::Int32::ConstPtr& msg) {
        current_midpoint = msg->data;
    }

    void ultraCallback(const std_msgs::Int32MultiArray::ConstPtr& msg) {
        // 전방 초음파 노이즈 필터링
        double dist = filter.update(msg->data[2]);
        
        ackermann_msgs::AckermannDriveStamped drive_msg;
        
        // 정지 조건 (ultra_gostop 로직)
        if (dist > 0 && dist < 25) {
            drive_msg.drive.speed = 0;
        } else {
            drive_msg.drive.speed = 0.5;
            // 차선 오차 계산 및 PID 제어
            double error = current_midpoint - 320;
            drive_msg.drive.steering_angle = -pid_controller.control(error) * (M_PI/180.0);
        }
        drive_pub.publish(drive_msg);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "control_node");
    ControlNode cn;
    ros::spin();
    return 0;
}
