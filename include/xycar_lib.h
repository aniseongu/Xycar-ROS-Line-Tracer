#ifndef XYCAR_LIB_H
#define XYCAR_LIB_H

#include <vector>
#include <numeric>
#include <algorithm>

// 1. PID 제어 클래스
class PID {
private:
    double kp, ki, kd;
    double prev_error = 0, integral = 0;
public:
    PID(double p, double i, double d) : kp(p), ki(i), kd(d) {}
    double control(double error) {
        double derivative = error - prev_error;
        integral += error;
        // Anti-windup: 적분 오차 누적 방지
        integral = std::max(std::min(integral, 10.0), -10.0); 
        prev_error = error;
        return (kp * error) + (ki * integral) + (kd * derivative);
    }
};

// 2. 센서 필터 클래스 (중앙값 필터)
class MedianFilter {
private:
    int size;
    std::vector<double> buffer;
public:
    MedianFilter(int n = 5) : size(n) {}
    double update(double val) {
        buffer.push_back(val);
        if (buffer.size() > size) buffer.erase(buffer.begin());
        std::vector<double> sorted = buffer;
        std::sort(sorted.begin(), sorted.end());
        return sorted[sorted.size() / 2];
    }
};

#endif
