// Author: 안성우 (성균관대학교 소프트웨어학과)
// Project: Xycar ROS Line Tracer - PID Control Node (C++)

#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <ackermann_msgs/AckermannDriveStamped.h>

class XycarController {
private:
    ros::NodeHandle nh_;
    ros::Publisher drive_pub_;
    ros::Subscriber error_sub_;

    // PID 파라미터
    double kp_, ki_, kd_;
    double prev_error_;
    double integral_;
    double base_speed_;

public:
    XycarController() {
        // ROS 파라미터 서버에서 게인 값 가져오기 (기본값 설정)
        nh_.param("kp", kp_, 0.4);
        nh_.param("ki", ki_, 0.001);
        nh_.param("kd", kd_, 0.15); 
        // 💡 kd(미분) 값이 네가 기억하는 "벗어나려고 할 때 확 꺾어주는" 핵심 역할!

        prev_error_ = 0.0;
        integral_ = 0.0;
        base_speed_ = 0.5; // 자이카 기본 주행 속도

        // Xycar 모터 구동 토픽 퍼블리셔 (Ackermann 스티어링)
        drive_pub_ = nh_.advertise<ackermann_msgs::AckermannDriveStamped>("/xycar_motor", 1);
        
        // 비전 노드(Python)에서 계산한 오차값 구독
        error_sub_ = nh_.subscribe("/line_error", 1, &XycarController::errorCallback, this);
    }

    void errorCallback(const std_msgs::Float32::ConstPtr& msg) {
        double error = msg->data;

        // 1. PID 제어 연산
        integral_ += error;
        double derivative = error - prev_error_; 
        // 차가 라인을 급격히 벗어날 때 derivative 값이 커져서 강하게 조향을 보정함

        double steering_output = (kp_ * error) + (ki_ * integral_) + (kd_ * derivative);
        prev_error_ = error;

        // 2. Xycar 제어 메시지 생성
        ackermann_msgs::AckermannDriveStamped drive_msg;
        drive_msg.drive.speed = base_speed_;
        
        // 오차 방향에 맞게 조향각 적용 (하드웨어 특성에 따라 - 부호)
        drive_msg.drive.steering_angle = -steering_output;

        // 3. 모터 명령 퍼블리시
        drive_pub_.publish(drive_msg);
        ROS_INFO("Error: %.2f | Steering: %.2f", error, steering_output);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "xycar_control_node");
    XycarController controller;
    ros::spin(); // 콜백 대기
    return 0;
}
