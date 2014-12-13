cmake_minimum_required(VERSION 2.8.3)
project(algp2_msgs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  nav_msgs
  geometry_msgs
)

add_message_files(
   FILES
   Map2D.msg
 )


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
   nav_msgs

 )

catkin_package(
   DEPENDS message_runtime
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)



