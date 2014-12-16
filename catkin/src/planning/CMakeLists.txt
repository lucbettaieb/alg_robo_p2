cmake_minimum_required(VERSION 2.8.3)
project(planning)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  stdr_msgs
  algp2_msgs
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES controller
#  CATKIN_DEPENDS roscpp std_msgs stdr_msgs
#  DEPENDS system_lib
)

include_directories(
	INCLUDE_DIRS include 
  ${catkin_INCLUDE_DIRS}
)

add_executable(global_planner src/global_planner.cpp)
add_executable(local_planner src/local_planner.cpp)

# add_executable(simple_steering_pose src/simple_steering_pose.cpp)

target_link_libraries(global_planner
   ${catkin_LIBRARIES}
)

target_link_libraries(local_planner
   ${catkin_LIBRARIES}
)
# target_link_libraries(simple_steering_pose 
#    ${catkin_LIBRARIES}
# )
