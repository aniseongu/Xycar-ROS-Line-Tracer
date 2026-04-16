#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <std_msgs/Int32.h>

class VisionNode {
    ros::NodeHandle nh;
    ros::Subscriber sub;
    ros::Publisher pub;

public:
    VisionNode() {
        sub = nh.subscribe("/usb_cam/image_raw", 1, &VisionNode::imgCallback, this);
        pub = nh.advertise<std_msgs::Int32>("/lane_midpoint", 1);
    }

    void imgCallback(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        cv::Mat frame = cv_ptr->image;
        cv::Mat gray, blur, edge, roi;

        // 1. 전처리 (Gray -> Blur -> Canny)
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);
        cv::Canny(blur, edge, 60, 150);

        // 2. ROI 설정 (강의 내용 반영)
        roi = edge(cv::Rect(0, 300, 640, 150));

        // 3. 허프 변환 직선 검출
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(roi, lines, 1, CV_PI/180, 30, 30, 10);

        // 왼쪽/오른쪽 차선 분리 및 중앙점 계산 로직 (단순화 버전)
        int l_count = 0, r_count = 0;
        double l_pos = 0, r_pos = 0;

        for (auto& l : lines) {
            double slope = (double)(l[3]-l[1])/(l[2]-l[0]);
            if (slope < -0.1) { l_pos += (l[0]+l[2])/2.0; l_count++; }
            else if (slope > 0.1) { r_pos += (l[0]+l[2])/2.0; r_count++; }
        }

        if (l_count > 0 && r_count > 0) {
            std_msgs::Int32 mid_msg;
            mid_msg.data = (l_pos/l_count + r_pos/r_count) / 2;
            pub.publish(mid_msg);
        }
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "vision_node");
    VisionNode vn;
    ros::spin();
    return 0;
}
