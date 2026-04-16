cmake_minimum_required(VERSION 3.0.2)
project(mobility_line_tracer)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  cv_bridge
  ackermann_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(vision_node src/vision_node.cpp)
target_link_libraries(vision_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(control_node src/control_node.cpp)
target_link_libraries(control_node ${catkin_LIBRARIES})
