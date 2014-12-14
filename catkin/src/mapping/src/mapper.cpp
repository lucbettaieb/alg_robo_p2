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
bool alreadyStuffed = false;

ros::Subscriber sub_map_;
ros::Publisher pub_map2d_;
//FOR NOW:  This will be configured for use with the default map loaded by roslaunch stdr_world stdr_server.launch
//Width and height manually have been set to 775 and 746 corresponding with stdr_server.launch map file

std::vector< std::vector<int> > map_(746, std::vector<int>(775, 0)); //global map to better represent occupancy grid map

algp2_msgs::Map2D pubby_map_;

/*---------------Functions--------------*/
void convertOccupancyGridTo2DVector(nav_msgs::OccupancyGrid grid, std::vector< std::vector<int> > &map){		
	ROS_INFO("convertOccupancyGridTo2DVector...");
	
	int row = 0;
	int col = 0;

	for(int i = 0; i < grid.data.size(); i++, col++){
		if((i >= 744) && ((i % grid.info.width) == 0) ){
			col = 0;
			row++;
		}
		//std::cout << "i: " << i << "| row: " << row << "| col: " << col<< std::endl;

		map.at(row).at(col) = grid.data.at(i);
	}
	ROS_INFO("...done converting occupancy grid to 2d vector");
}

void gotMapCB(const nav_msgs::OccupancyGrid &grid){
	if(!gotGrid){ //If a grid has not already been created...  No need to make another as the environment is static.
		ROS_INFO("Got map cb");
		convertOccupancyGridTo2DVector(grid, map_);
		gotGrid = true; //Should have finished nicely!
	}
}

void stuff2DVecIntoMap2DAndPublish(){
	if(!alreadyStuffed){
		ROS_INFO("stuff2DVecIntoMap2DAndPublish");

		pubby_map_.columns.resize(746);

		for(int i = 0; i < 746; i++){
			pubby_map_.columns.at(i).data.resize(0);
		}
		
		for(int j = 0; j < map_.size(); j++){  //Should just need to push back 
			for(int i = 0; i < map_.at(0).size(); i++){

				//std::cout << "i: " << i << "| j: " << j << std::endl;
				//pubby_map_.columns.at(j).data.at(i) = map_.at(j).at(i); //test this
				pubby_map_.columns.at(j).data.push_back(map_.at(j).at(i));
			}
		}

		//std::cout << "row size pubby: "<<pubby_map_.columns.at(0).data.size() << std::endl;

		map2dReadyToPub = true;
		alreadyStuffed = true;
	}
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
		if(map2dReadyToPub){
			ROS_INFO("Publishing map!");
			ros::Duration(0.1).sleep();
			pub_map2d_.publish(pubby_map_);
		}

		ros::spinOnce();
	}
	return 0;
}