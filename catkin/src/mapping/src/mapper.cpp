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
std::vector< std::vector<int> > map_; //global map to better represent occupancy grid map

algp2_msgs::Map2D pubby_map_;

/*---------------Functions--------------*/
void convertOccupancyGridTo2DVector(nav_msgs::OccupancyGrid grid, std::vector< std::vector<int> > &map){		
	int row = 0;
	int col = 0;
	
	for(int j = 0; j < grid.info.height; j++, col++){
		if(j % grid.info.width == 0) {
			row++;
			col = 0;
		}
		map.at(row).at(col) = grid.data.at(j);
		//map[row, col] = grid[j] 
		//Thanks sds
	}
}

void gotMapCB(const nav_msgs::OccupancyGrid &grid){
	if(!gotGrid){ //If a grid has not already been created...  No need to make another as the environment is static.
		convertOccupancyGridTo2DVector(grid, map_);
	}
	
	gotGrid = true; //Should have finished nicely!
}

void stuff2DVecIntoMap2DAndPublish(){
	
	for(int j = 0; j < map_.size; j++){  //Should just need to push back 
		pubby_map_.push_back(map_.at(j)); //test this
	}
	map2dReadyToPub = true;
}

/*---------------Main--------------*/
int main(int argc, char** argv){
	ros::init(argc, argv, "mapper");
	ros::NodeHandle nh;
	
	tf::TransformListener tf_(ros::Duration(10));
	costmap_2d::Costmap2DROS costmap_("costmap", tf_);
	pub_map2d_ = nh.advertise<algp2_msgs::Map2D>("/map2d", 1);

	/*These should be handled in a launch file */
	//nh.setParam("costmap/global_frame", "/map"); //Default characteristics.
	//nh.setParam("costmap/robot_base_frame", "/robot0");
	//nh.setParam("costmap/rolling_window", true);

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