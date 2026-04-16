# Xycar-ROS-Line-Tracer
ROS Noetic 환경에서 Xycar(자이카)와 OpenCV를 활용하여 구현한 실시간 검은색 라인 트래킹(PID 제어) 프로젝트입니다. (2023 겨울 자율주행 강좌)

# 🏎️ Jetson Nano 기반 자율주행 라인 트래킹 프로젝트

이 프로젝트는 실시간 비전 처리와 제어 알고리즘을 결합하여 정해진 트랙(검은색 라인)을 안정적으로 주행하는 것을 목표로 합니다.

## 🌟 핵심 구현 기능
1. **Real-time Image Processing:**
   - Jetson Nano와 CSI 카메라를 활용한 30fps 이상의 영상 데이터 처리
   - OpenCV 기반의 HSV/Binary Thresholding 및 ROI 설정으로 검은색 라인 추출
   - 영상 모멘트를 활용한 실시간 라인 중심점 추적

2. **PID 제어 기반 조향 시스템:**
   - 라인 중앙값과 차량 중심값 사이의 오차(Error)를 입력값으로 사용
   - P, I, D 파라미터 튜닝을 통해 곡선 구간 및 직선 구간에서의 주행 안정성 확보

3. **Embedded Optimization:**
   - [cite_start]Ubuntu 20.04 환경에서 Docker 컨테이너를 활용한 독립적인 개발 환경 구축 [cite: 2]
   - Jetson Nano의 GPU 자원을 고려한 경량화된 비전 알고리즘 설계

## 📊 시스템 아키텍처
Camera → OpenCV Processing (Grayscale/Blur/Threshold) → Error Calculation → PID Controller → Motor Driver (Servo/DC)

## 🛠️ 기술 스택
- **Language:** Python 3.8
- **Library:** OpenCV, NumPy, Smbus (I2C 통신용)
- [cite_start]**Embedded:** NVIDIA Jetson Nano, Ubuntu 20.04 LTS, Docker [cite: 2]

## 🛠️ Hardware & Environment
* [cite_start]**Platform:** Xycar (자율주행 교육용 고성능 RC카 플랫폼) 
* [cite_start]**Computing:** NVIDIA Jetson Nano (4GB) 
* [cite_start]**Sensors:** - CSI Camera (라인 인식 및 객체 탐지용) 
  - 2D LiDAR (장애물 회피 및 SLAM 실습용 - *사용했다면 추가*)
  - IMU (차량 자세 및 가속도 측정)
* **Actuators:** VESC (Electronic Speed Controller), Steering Servo Motor
