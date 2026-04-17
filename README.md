# 🏎️ Xycar-ROS-Autonomous-Driving (C++ Optimized)
**ROS Noetic** 기반의 **WSL2** 환경에서 구현한 실시간 자율주행 시스템입니다. 
2023년 교육 과정을 통해 습득한 기초 알고리즘을 바탕으로, 2026년 현재 임베디드 성능 최적화와 안정성 향상을 위해 전체 시스템을 **C++ 클래스 구조로 재설계 및 리팩토링**한 프로젝트입니다.

## 📌 Project History & Evolution
* **Initial Development (2023.12):** 자율주행 아카데미를 통한 Python 기반 인지/제어 로직 Prototype 구현
* **Refactoring & Optimization (2026.04):** - 실시간성 확보를 위해 Python 노드를 **C++14**로 전면 리팩토링
  - 하드웨어 제어 주기 최적화 및 객체지향적(OOP) 설계를 통한 시스템 안정성 확보

## 📊 System Architecture
본 시스템은 ROS의 비동기 통신 아키텍처를 기반으로 하며, PC(WSL2)와 차량 간의 원격 분산 통신 환경에서 작동합니다.

1. **Perception Node (C++):**
   - OpenCV 기반 고속 비전 처리 파이프라인 (Hough Transform 기반 차선 검출)
   - 조명 변화에 강건한 **HLS/LAB** 색상 공간 기반 차선 및 정지선 추출
2. **Filter Node (C++):**
   - 초음파 센서 데이터의 노이즈 제거를 위한 **Median Filter** 구현
   - 센서 스파이크(Outlier) 제거를 통해 불필요한 급제동 및 조향 진동 방지
3. **Control Node (C++):**
   - Ackermann Steering 모델 기반의 조향 제어 로직 구현
   - Anti-windup 로직이 포함된 **PID Control** 클래스 자체 설계

## 🌟 Key Engineering Highlights

### 1. Field Tuning: 실차 주행 기반 파라미터 최적화
시뮬레이션과 실제 환경의 괴리를 극복하기 위해 직접 자이카를 구동하며 제어 수치를 최적화했습니다.
* **Heuristic Control:** 좌우 초음파 센서의 편차(`diff`)가 임계값(10cm) 이상 발생할 때 즉각적으로 조향각(50)을 보정하여 벽면 충돌을 방지하는 알고리즘 적용.
* **Gain Tuning:** 곡선 구간 진입 시의 오버슈트를 제어하기 위해 PID 제어기의 D(미분) 항을 반복 튜닝하여 주행 댐핑(Damping) 성능 최적화.

### 2. Robust Vision Pipeline
* **Hough Transform 최적화:** 단순 이진화를 넘어 허프 변환을 통해 차선을 직선 방정식으로 벡터화하고, 기울기 필터링을 적용하여 도로 위 노이즈를 효과적으로 제거했습니다.
* **Traffic Logic:** `HoughCircles`와 ROI 내부 픽셀 평균 밝기 분석을 결합하여 신호등의 상태(Red/Green)를 실시간으로 판별합니다.

## 🛠️ Tech Stack
| Category | Technology |
| :--- | :--- |
| **Framework** | ROS 1 Noetic |
| **Language** | **C++14**, Python 3.8 |
| **Library** | OpenCV 4.x, cv_bridge, roscpp, std_msgs, ackermann_msgs |
| **Infrastructure** | WSL2 (Ubuntu 20.04), 3D ROS Simulator (Digital Twin) |

## 📂 Repository Structure
```text
.
├── CMakeLists.txt         # C++ 빌드 및 라이브러리 링크 설정
├── package.xml            # 패키지 의존성(roscpp, ackermann_msgs 등) 관리
├── launch/                # 통합 실행 파일
│   └── xycar_drive.launch
├── include/               # 알고리즘 클래스 정의 (PID, MedianFilter)
│   └── xycar_lib.h
└── src/                   # 핵심 소스 코드 (C++)
    ├── vision_node.cpp    # 차선 인식 및 영상 처리 전용 노드
    └── control_node.cpp   # 차량 제어 및 센서 퓨전 전용 노드
```
⚖️ Credits & Disclaimer
Original Course: 본 프로젝트의 알고리즘 기초 및 하드웨어 연동 이론은 '자이트론(Xytron) 자율주행 교육' 커리큘럼을 기반으로 학습되었습니다.

Implementation: 저작권 및 보안 준수를 위해 제공된 예제 코드를 사용하지 않고, 모든 로직을 C++로 직접 재작성(Scratch Implementation) 하였습니다.

Notice: 교육용 영상 및 자료 원본은 포함하고 있지 않으며, 개인의 기술 최적화 연구 목적으로만 공개된 코드입니다.
