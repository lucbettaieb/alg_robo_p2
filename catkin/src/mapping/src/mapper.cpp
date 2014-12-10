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

#include <tf/transform_listener.h>
#include <costmap_2d/costmap_2d_ros.h>



int main(int argc, char** argv){
	ros::init(argc, argv, "mapper");
	ros::NodeHandle nh;
	
	tf::TransformListener tf_(ros::Duration(10));
	costmap_2d::Costmap2DROS costmap_("costmap", tf_);

	//nh.setParam("costmap/global_frame", "/map"); //Default characteristics.
	nh.setParam("costmap/robot_base_frame", "/robot0");
	nh.setParam("costmap/rolling_window", true);


	ros::spin();
	return 0;
}