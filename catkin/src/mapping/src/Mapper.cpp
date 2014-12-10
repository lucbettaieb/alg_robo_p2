/*
* Mapper
* A node that will do mapping things.  But srsly, it will:
* 	- Use an approximate cell decomposition to create a quadtree for global planning
*	- Use a costmap_2d to provide a basis for local planning
*	- Maybe some other stuff?  I don't know yet.
*
*	Luc A. Bettaieb
* 	bettaieb@case.edu
*
*	12.8.14
*/

#include <ros/ros.h>




int main(int argc, char** argv){
	ros::init(argc, argv, "mapper");
	ros::NodeHandle nh;


	ros::spin();
	return 0;
}