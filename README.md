# 🏎️ Xycar Full-Stack Autonomous Driving System
**ROS Noetic** 기반의 **WSL2** 환경에서 구현한 실시간 자율주행 시스템입니다. 고성능 RC 플랫폼인 **Xycar**를 활용하여 인지(Perception), 판단(Decision), 제어(Control)의 전 과정을 독자적으로 설계 및 최적화했습니다.

## 📺 Project Demo
*(여기에 track.avi를 움짤(GIF)로 변환해서 넣어줘. 가장 강력한 무기야!)*

## 🚀 핵심 기술 고도화 포인트

### 1. 하드웨어 최적화 및 C++ 리팩토링
* **성능 최적화:** 초기 Python으로 구현된 제어 파이프라인을 **C++**로 리팩토링하여 연산 지연(Latency)을 최소화하고 30fps 이상의 실시간 비전 처리를 달성했습니다.
* **Ackermann Steering:** 차량의 물리적 조향 특성을 반영한 Ackermann 모델 기반의 제어 명령(`AckermannDriveStamped`)을 구현했습니다.

### 2. 강건한 비전 인식 (Computer Vision)
* **다중 색상 공간 분석:** 조명 변화에 대응하기 위해 HSV뿐만 아니라 **HLS, LAB** 색상 공간을 테스트하여 최적의 차선 및 정지선 인식 임계값을 도출했습니다.
* **Hough Transform:** 허프 변환을 통해 차선을 벡터화하고, 기울기 필터링을 통해 도로의 노이즈를 제거하여 안정적인 차선 중앙값(Mid-point)을 추적합니다.
* **객체 인식:** 허프 원 변환(Hough Circles)을 활용해 신호등의 상태를 실시간으로 판별하는 로직을 통합했습니다.

### 3. 센서 데이터 정제 (Sensor Filtering)
* **Outlier 제거:** 초음파 센서에서 발생하는 간헐적인 튀는 값(예: 140cm 스파이크)을 처리하기 위해 **Moving Average** 및 **Median Filter**를 적용하여 제어 명령의 안정성을 확보했습니다.
* **Field Tuning:** 시뮬레이션에 의존하지 않고 실제 트랙에서 반복 주행하며 조향 임계값(Threshold)과 PID 게인 값을 최적화했습니다.

## 🛠️ Tech Stack
* **Framework:** ROS 1 Noetic
* **Language:** **C++14**, Python 3.8
* **Library:** OpenCV 4.x, `cv_bridge`, `roscpp`, `rospy`
* **Infrastructure:** Windows Subsystem for Linux (WSL2), 3D ROS Simulator (Digital Twin)

## 🏗️ System Architecture
1. **Perception Node (C++):** 카메라 이미지 전처리(Blur, Gray) → 허프 변환 → 차선 오차 계산
2. **Filter Node (C++):** 초음파 센서 데이터 수집 → Median Filtering → 노이즈 제거된 거리 정보 제공
3. **Control Node (C++):** 오차값 수신 → PID 제어 연산 → Xycar VESC 모터 제어

## 🏁 How to Run
```bash
# Workspace 빌드
cd ~/catkin_ws && catkin_make
source devel/setup.bash

# 실차 주행 실행
roslaunch mobility_line_tracer hough_drive.launch
