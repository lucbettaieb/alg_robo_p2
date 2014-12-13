/*
* Mapper
* A node that will do mapping things.  But srsly, it will:
* 	- U-s-e- -a-n- -a-p-p-r-o-x-i-m-a-t-e- -c-e-l-l- -d-e-c-o-m-p-o-s-i-t-i-o-n- 
*     -t-o- -c-r-e-a-t-e- -a- -q-u-a-d-t-r-e-e- -f-o-r- -g-l-o-b-a-l- -p-l-a-n-n-i-n-g- 
*     *(This is going to be moved to global_planner because it will be difficult to broadcast a tree data structure over ros)
*	- Convert the occupancy grid map published on /map to an easier to use /map2d for decomposition into a quadtree
*	- Use a costmap_2d to provide a basis for local planning
*	- Maybe some other stuff?  I don't know yet.
*
*	Luc A. Bettaieb
* 	bettaieb@case.edu
*
*	12.8.14
*/

/*---------------Includes--------------*/
#include <ros/ros.h>
#include <algp2_msgs/Map2D.h>

#include <tf/transform_listener.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <nav_msgs/OccupancyGrid.h>

#include <vector>

/*---------------Fields--------------*/
bool gotGrid = false;
bool map2dReadyToPub = false;

ros::Subscriber sub_map_;
ros::Publisher pub_map2d_;
std::vector< std::vector<int> > map_(___**width, std::vector<int>(___**height, 0)); //global map to better represent occupancy grid map

algp2_msgs::Map2D pubby_map_;

/*---------------Functions--------------*/
void convertOccupancyGridTo2DVector(nav_msgs::OccupancyGrid grid, std::vector< std::vector<int> > &map){		
	ROS_INFO("convertOccupancyGridTo2DVector");
	int row = 0;
	int col = 0;
	
	for(int j = 0; j < grid.info.height; j++, col++){
		ROS_INFO("entering loop");
		if(j % grid.info.width == 0) {
			ROS_INFO("new row, resetting columns");
			row++;
			col = 0;
		}
		std::cout << "j: " << j << std::endl;
		map.at(row).at(col) = grid.data.at(j);
		//map[row, col] = grid[j] 
		//Thanks sds
	}
}

void gotMapCB(const nav_msgs::OccupancyGrid &grid){
	ROS_INFO("Got map cb");
	if(!gotGrid){ //If a grid has not already been created...  No need to make another as the environment is static.
		convertOccupancyGridTo2DVector(grid, map_);
	}
	
	gotGrid = true; //Should have finished nicely!
}

void stuff2DVecIntoMap2DAndPublish(){
	ROS_INFO("stuff2DVecIntoMap2DAndPublish");
	for(int j = 0; j < map_.size(); j++){  //Should just need to push back 
		for(int i = 0; i < map_.at(0).size(); i++){
			pubby_map_.columns.at(j).data.at(i) = map_.at(j).at(i); //test this
		}
	}
	map2dReadyToPub = true;
}

/*---------------Main--------------*/
int main(int argc, char** argv){
	ros::init(argc, argv, "mapper");
	ros::NodeHandle nh;
	
	//tf::TransformListener tf_(ros::Duration(10));
	//costmap_2d::Costmap2DROS costmap_("costmap", tf_);
	pub_map2d_ = nh.advertise<algp2_msgs::Map2D>("/map2d", 1);

	/*These should be handled in a launch file */
	//nh.setParam("costmap/global_frame", "/map"); //Default characteristics.
	//nh.setParam("costmap/robot_base_frame", "/robot0");
	//nh.setParam("costmap/rolling_window", true);
	ROS_INFO("Gonna loop now:");
	while(ros::ok()){
		sub_map_ = nh.subscribe("/map", 10, gotMapCB);

		if(gotGrid)
			stuff2DVecIntoMap2DAndPublish();
		if(map2dReadyToPub)
			pub_map2d_.publish(pubby_map_);
		

		ros::spinOnce();
	}
	return 0;
}