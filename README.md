# 🏎️ Xycar-ROS-Autonomous-Driving (C++ Optimized)
**ROS Noetic** 환경에서 **Xycar(자이카)**를 활용하여 구현한 실시간 자율주행 시스템입니다. 
단순한 예제 수행을 넘어, 시스템의 실시간성 확보를 위해 인지/제어 파이프라인을 **C++로 직접 리팩토링 및 최적화**한 개인 프로젝트입니다.

## 📌 Project Overview
- **개발 환경:** Windows Subsystem for Linux (WSL2), Ubuntu 20.04 LTS
- **핵심 기술:** ROS Noetic, C++14, OpenCV, PID Control
- **주요 성과:** Python 기반의 초기 로직을 C++ 클래스 구조로 재설계하여 연산 지연(Latency) 최소화 및 주행 안정성 향상

## 📊 System Architecture
시스템은 ROS의 비동기 통신 아키텍처를 기반으로 하며, 각 노드는 독립적인 역할을 수행합니다.

1. **Perception Node (C++):** - OpenCV 기반 고속 비전 처리
   - Hough Transform(허프 변환)을 이용한 차선 벡터 추출
   - ROI(관심 영역) 최적화로 Jetson Nano 환경에서의 실시간성 확보
2. **Filter Node (C++):**
   - 초음파 센서 데이터의 노이즈 제거를 위한 **Median Filter** 및 **Low Pass Filter** 구현
   - 센서 스파이크(Outlier) 제거로 제어 명령의 급격한 변동 방지
3. **Control Node (C++):**
   - Ackermann Steering 모델 기반의 조향 제어
   - **PID Control** (Anti-windup 적용)을 통한 정밀한 조향각 보정
   - 실차 주행 테스트(Field Tuning)를 통한 최적의 파라미터 도출

## 🌟 Key Implementation Details

### 1. Robust Perception (강건한 인지 알고리즘)
단순한 색상 인식을 넘어 다양한 환경 변수에 대응하기 위해 다음 기술을 적용했습니다.
* **Multi-Color Space:** HSV 뿐만 아니라 **HLS, LAB** 색상 공간을 활용하여 조명 변화에 강건한 차선 검출 파라미터 확보
* **Hough Transform Tuning:** 캐니 에지(Canny Edge) 검출 후 허프 변환 파라미터를 세밀하게 조정하여 곡선 구간에서도 안정적인 직선 방정식 도출

### 2. High-Performance Control (고성능 제어 최적화)
강의의 파이썬 예제 코드를 분석하고, 이를 **객체지향 C++**로 직접 재설계했습니다.
* **PID Controller Class:** 적분 오차의 무한 누적을 방지하는 Anti-windup 로직을 클래스 내부에 구현
* **Latency Reduction:** 데이터 직렬화 및 통신 오버헤드를 줄이기 위해 효율적인 데이터 구조 사용

### 3. Field Tuning & Troubleshooting
실차 주행 중 발생한 조향 진동 및 차선 이탈 문제를 해결하기 위해 다음과 같은 엔지니어링 접근을 수행했습니다.
* 초음파 센서 간의 편차(`diff`)를 활용한 긴급 회피 조향 임계값(Threshold) 수동 최적화
* PID 제어기의 D(미분) 항 튜닝을 통해 조향 반응의 댐핑(Damping) 효과를 주어 안정적인 주행 달성

## 🛠️ Tech Stack
| Category | Technology |
| :--- | :--- |
| **Framework** | ROS 1 Noetic |
| **Language** | C++14, Python 3.8 |
| **Library** | OpenCV 4.x, roscpp, std_msgs, sensor_msgs, ackermann_msgs |
| **Tool** | WSL2, CMake, Catkin, Rviz |

## 📂 Repository Structure
```text
.
├── CMakeLists.txt          # C++ 빌드 설정
├── package.xml             # 의존성 관리
├── launch/                 # 통합 실행 파일
│   └── xycar_drive.launch
├── include/                # 헤더 파일 (PID, Filter Class)
│   └── xycar_lib.h
└── src/                    # C++ 소스 코드
    ├── vision_node.cpp     # 차선 인식 노드
    └── control_node.cpp    # 차량 제어 노드
