cmake_minimum_required(VERSION 2.8.3)
project(mapping)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  stdr_msgs
  algp2_msgs
  tf
  costmap_2d
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES controller
#  CATKIN_DEPENDS roscpp std_msgs stdr_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(mapper src/mapper.cpp)

target_link_libraries(mapper
   ${catkin_LIBRARIES}
)