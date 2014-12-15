/*
* global_planner
* A node that will generate a global path through a quadtree from a pose to a destination.
*
*	Luc A. Bettaieb
* 	bettaieb@case.edu
*
*	12.12.14
*/

#include <planning/global_planner.h>

ros::Subscriber sub_map2d_;
bool gotMap = false;
const int width = 1230;
const int height = 630;

std::vector< std::vector<int> > map_(height, std::vector<int>(width, 0)); //regular map.at(y).at(x)
std::vector<obstaclePoint> obstacles;

visualization_msgs::Marker criticalArea;

ros::Publisher pub_critArea_;

obstaclePoint makeObstacle(int ex, int wy){
	obstaclePoint p;
	p.x = wy; //Flipping obstacle x and y because it was previously messed up... going to see if this helps!
	p.y = ex;
	p.occ = true;

	return p;
}

geometry_msgs::Point makePoint(int ex, int wy){
	geometry_msgs::Point point;
	point.x = (float)ex;
	point.y = (float)wy;
	point.z = 0;

	return point;
}

//-----------Regular functions----------//
void processObstacles(){
	//This function will find critial points of obstacles.
	//Theory: find an obstacle that does not have neighboring obstacles in at least 3 cardinal directions


	for(int i = 0; i < obstacles.size(); i++){
		//ros::Duration(0.01).sleep();
		//std::cout << "x: " <<obstacles.at(i).x << "y: " <<obstacles.at(i).y <<std::endl;
		if(map_.at(obstacles.at(i).y).at(obstacles.at(i).x-3) == 0 && map_.at(obstacles.at(i).y).at(obstacles.at(i).x-+3) == 0 && map_.at(obstacles.at(i).y+3).at(obstacles.at(i).x) == 0) {
			//add all critical points to polygon
			std::cout << "x: " <<obstacles.at(i).x << "y: " <<obstacles.at(i).y <<std::endl;

			criticalArea.points.push_back(makePoint(obstacles.at(i).x*0.02, obstacles.at(i).y*0.02));
			criticalArea.header.stamp = ros::Time();
			criticalArea.header.frame_id = "world";
			criticalArea.ns = "my_namespace";
			criticalArea.id = 0;
			criticalArea.type = visualization_msgs::Marker::POINTS;
			criticalArea.action = visualization_msgs::Marker::ADD;
			criticalArea.color.a = 1.0;
			criticalArea.color.r = 0.0;
			criticalArea.color.g = 1.0;
			criticalArea.color.b = 0.0;
			criticalArea.scale.x = 0.1;
			criticalArea.scale.y = 0.1;
			criticalArea.pose.orientation.x = 0.0;
			criticalArea.pose.orientation.y = 0.0;
			criticalArea.pose.orientation.z = 0.0;
			criticalArea.pose.orientation.w = 1.0;

		}
			
		

		//if(map_.at(obstacles.at(i).y-1).at(obstacles.at(i).x) == 100 && map_.at(obstacles.at(i).y+4).at(obstacles.at(i).x) == 0){
		 	//std::cout << "i: " << i <<" obstacle: " << map_.at(obstacles.at(i).x).at(obstacles.at(i).y) <<" obstacle y+1: " << map_.at(obstacles.at(i).x).at(obstacles.at(i).y+1) << " obstacle y-1: " << map_.at(obstacles.at(i).x).at(obstacles.at(i).y-1) << std::endl;
		// 	std::cout << "x: " <<obstacles.at(i).x << "y: " <<obstacles.at(i).y <<std::endl;
		// }

		//if(map_.at(obstacles.at(i).x).at(obstacles.at(i).y))
	}
	//publish polygon
	
}

void findObstacles(){
	ROS_INFO("Finding obstacles...");
	double start =ros::Time::now().toSec();

	for(int i = 0; i < map_.size(); i++){
		for(int j = 0; j < map_.at(0).size(); j++){
			if(map_.at(i).at(j) == 100)
				obstacles.push_back(makeObstacle(i,j));
		}
	}
	double end = ros::Time::now().toSec();

	ROS_INFO("%i obstacles found in %f seconds.", (int)obstacles.size(), (float)(end-start));

	processObstacles();
}

void gotMap2dCB(const algp2_msgs::Map2D &map2d){
	if(!gotMap){
		ROS_INFO("Got map.");
		//Got the map.  Now parse back into regular 2d Vector
		//std::cout << "col size" << map2d.columns.size() << std::endl;
		//std::cout << "row size" << map2d.columns.at(0).data.size() << std::endl;
		for(int i = 0; i < map2d.columns.size(); i++){
			for(int j = 0; j < map2d.columns.at(0).data.size(); j++){
				map_.at(i).at(j) = map2d.columns.at(i).data.at(j);
			}
		}
		gotMap = true;
		findObstacles();

	}
}



int main(int argc, char** argv){
	ros::init(argc, argv, "global_planner");
	ros::NodeHandle nh;
	//First, let's get the map.
	ROS_INFO("Let's do some global planning! :)");
	pub_critArea_ = nh.advertise<visualization_msgs::Marker>("/criticalArea", 1);
	
	while(ros::ok()){
		sub_map2d_ = nh.subscribe("/map2d", 10, gotMap2dCB);
		pub_critArea_.publish(criticalArea);
		ros::Duration(0.05).sleep();

		ros::spinOnce();
	}


	ros::spin();
	return 0;
}