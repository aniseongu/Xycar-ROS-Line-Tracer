// Author: 안성우 (성균관대학교 소프트웨어학과)
// Project: Xycar ROS Line Tracer - Vision Perception Node (C++)

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/Float32.h>

class XycarVisionNode {
private:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    ros::Publisher error_pub_;
    int threshold_val_;

public:
    XycarVisionNode() : it_(nh_) {
        // 카메라 이미지 구독 (/usb_cam/image_raw)
        image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, &XycarVisionNode::imageCallback, this);
        // 계산된 오차 퍼블리셔
        error_pub_ = nh_.advertise<std_msgs::Float32>("/line_error", 10);
        
        threshold_val_ = 60; // 이진화 임계값
    }

    void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        cv::Mat gray, blur, thresh, roi;
        // 1. 전처리 (그레이스케일 & 가우시안 블러)
        cv::cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);

        // 2. 이진화 (검은색 라인 추출)
        cv::threshold(blur, thresh, threshold_val_, 255, cv::THRESH_BINARY_INV);

        // 3. ROI 설정 (하단 30% 영역)
        int h = thresh.rows;
        int w = thresh.cols;
        roi = thresh(cv::Rect(0, h * 0.7, w, h * 0.3));

        // 4. 모멘트를 이용한 무게중심 계산
        cv::Moments m = cv::moments(roi);
        if (m.m00 > 0) {
            float cx = m.m10 / m.m00;
            float error = cx - (w / 2.0); // 중앙과의 오차

            std_msgs::Float32 error_msg;
            error_msg.data = error;
            error_pub_.publish(error_msg);
        }
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "xycar_vision_node");
    XycarVisionNode vision_node;
    ros::spin();
    return 0;
}
