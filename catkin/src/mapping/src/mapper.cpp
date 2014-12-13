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
#include <nav_msgs/OccupancyGrid.h>

#include <vector>

bool gotGrid = false;
ros::Subscriber sub_map_;

std::vector< std::vector<int> > map_; //global map to better represent occupancy grid map

class QNode {
	QNode *nw, *ne, *sw, *se; //Pointers to children nodes of the individual QNode
	bool occupied;

	public:
		QNode getNW();
		QNode getNE();
		QNode getSW();
		QNode getSE();
};

class Quadtree {
	QNode root;
	public:
		Quadtree();
		void grow(bool nwOcc, bool neOcc, bool swOcc, bool seOcc);
};

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

int main(int argc, char** argv){
	ros::init(argc, argv, "mapper");
	ros::NodeHandle nh;
	
	tf::TransformListener tf_(ros::Duration(10));
	costmap_2d::Costmap2DROS costmap_("costmap", tf_);

	/*These should be handled in a launch file */
	//nh.setParam("costmap/global_frame", "/map"); //Default characteristics.
	//nh.setParam("costmap/robot_base_frame", "/robot0");
	//nh.setParam("costmap/rolling_window", true);

	sub_map_ = nh.subscribe("/map", 10, gotMapCB);



	ros::spin();
	return 0;
}