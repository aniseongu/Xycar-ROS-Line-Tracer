cmake_minimum_required(VERSION 3.0.2)
project(mobility_line_tracer)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
  cv_bridge
  image_transport
  ackermann_msgs
)

find_package(OpenCV REQUIRED)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

# 비전 노드 빌드
add_executable(vision_node src/vision_node.cpp)
target_link_libraries(vision_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

# 제어 노드 빌드
add_executable(control_node src/control_node.cpp)
target_link_libraries(control_node ${catkin_LIBRARIES})
