# 🏎️ Xycar-ROS-Line-Tracer
**ROS Noetic** 환경에서 **Xycar(자이카)**와 **OpenCV**를 활용하여 구현한 실시간 검은색 라인 트래킹 프로젝트입니다. 단순한 주행을 넘어, 시스템의 실시간성 확보를 위해 인지 및 제어 파이프라인을 **C++로 최적화**하여 구현했습니다. (2023 겨울 자율주행 강좌 실습)

## 📊 시스템 아키텍처 (ROS Node Graph)
본 시스템은 ROS의 노드 간 비동기 통신 아키텍처를 기반으로 설계되었습니다.
* **`usb_cam_node`**: CSI 카메라로부터 로우 데이터 수집 및 `/usb_cam/image_raw` 토픽 퍼블리시
* **`vision_node` (C++)**: 이미지 수신, OpenCV 기반 이진화 처리 및 라인 중앙점 오차(`/line_error`) 산출
* **`control_node` (C++)**: 오차 데이터 구독 및 **PID 제어** 알고리즘을 통한 차륜 조향각(`AckermannDrive`) 계산 및 퍼블리시

## 🌟 핵심 구현 기능
### 1. C++ 기반 실시간 제어 최적화
* **Real-time Performance:** 자율주행 시스템의 지연 시간(Latency)을 최소화하기 위해 전 주행 파이프라인을 C++로 작성하여 Jetson Nano의 리소스를 효율적으로 관리했습니다.
* **Ackermann Steering Control:** Xycar 전용 메시지 타입인 `AckermannDriveStamped`를 활용하여 실제 차량의 물리적 조향 특성을 반영한 제어 로직을 구현했습니다.

### 2. 고속 비전 처리 파이프라인
* **Image Processing:** OpenCV의 Gaussian Blur와 Binary Thresholding을 활용하여 조명 변화에 강건한 검은색 라인 추출 로직을 설계했습니다.
* **ROI & Moments:** 화면 전체가 아닌 하단 30% 영역(Region of Interest)에 집중하고, 무게중심(Moments) 연산을 적용하여 픽셀 단위의 정밀한 오차를 산출합니다.

### 3. 정밀 PID 조향 제어 (Line Following)
* **Error Compensation:** 차량이 라인을 벗어나려는 순간, **D(미분) 제어** 파라미터를 통해 급격한 조향 변화를 감지하고 반대 방향으로 강한 보정 토크를 발생시켜 이탈을 방지합니다.
* **Parameter Tuning:** 직선 구간의 가속과 곡선 구간의 안정적인 탈출을 위해 P, I, D 게인 값을 반복적으로 튜닝하여 최적의 주행 경로를 확보했습니다.

## 🛠️ 기술 스택 (Tech Stack)
* **Framework:** ROS 1 Noetic
* **Language:** **C++14**, Python 3.8
* **Library:** OpenCV 4.x, `cv_bridge`, `roscpp`, `rospy`
* **Infrastructure:** Ubuntu 20.04 LTS, Docker 컨테이너 독립 환경

## 🛠️ Hardware & Environment
* **Platform:** **Xycar (자율주행 교육용 고성능 RC카 플랫폼)**
* **Computing:** NVIDIA Jetson Nano (4GB)
* **Sensors:** CSI Camera (라인 인식 및 객체 탐지용), IMU (차량 자세 및 가속도 측정)
* **Actuators:** VESC (Electronic Speed Controller), High-Torque Steering Servo Motor

## 📂 패키지 구조 (Package Structure)
```text
mobility_line_tracer/
├── CMakeLists.txt         # C++ 노드 빌드 설정
├── package.xml            # 패키지 의존성 관리
├── launch/
│   └── line_tracer.launch # 전체 시스템 통합 실행 파일
└── src/
    ├── vision_node.cpp    # OpenCV 기반 라인 인식 노드 (C++)
    └── control_node.cpp   # PID 기반 차량 제어 노드 (C++)
