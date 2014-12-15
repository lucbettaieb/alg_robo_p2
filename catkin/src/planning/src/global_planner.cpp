/*
* global_planner
* A node that will generate a global path through a trapezoidal cell decomposition from a pose to a destination.
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

std::vector<geometry_msgs::Point> lowerPoints;
std::vector<geometry_msgs::Point> upperPoints;

visualization_msgs::Marker criticalArea;

ros::Publisher pub_critArea_;

obstaclePoint makeObstacle(int ex, int wy){
	obstaclePoint p;
	p.x = wy; //Flipping obstacle x and y because it was previously messed up... going to see if this helps!
	p.y = ex;
	p.occ = true;

	return p;
}

geometry_msgs::Point makePoint(float ex, float wy){
	geometry_msgs::Point point;
	point.x = ex;
	point.y = wy;
	point.z = 0;

	return point;
}

//-----------Regular functions----------//

void removeDuplicates(){
	float tolerance = 0.02;

	for(int checkMe = 0; checkMe < lowerPoints.size(); checkMe++){
		for(int i = 0; i < lowerPoints.size(); i++){
			if( (i != checkMe) && ((lowerPoints.at(i).x < lowerPoints.at(checkMe).x + tolerance) && (lowerPoints.at(i).x > lowerPoints.at(checkMe).x - tolerance))){
				lowerPoints.erase(lowerPoints.begin()+i);
			}
		}
	}

	for(int checkMe = 0; checkMe < upperPoints.size(); checkMe++){
		for(int i = 0; i < upperPoints.size(); i++){
			if( (i != checkMe) && ((upperPoints.at(i).x < upperPoints.at(checkMe).x + tolerance) && (upperPoints.at(i).x > upperPoints.at(checkMe).x - tolerance))){
				upperPoints.erase(upperPoints.begin()+i);
			}
		}
	}
	//Weird duplicated values have now been removed.  Now they need to be sorted by their x coordinate.

}

void sortByX(){
	
}


void processObstacles(){
	//This function will find critial points of obstacles.
	//Theory: find an obstacle that does not have neighboring obstacles in at least 3 cardinal directions
	int offsetChecker = 3;
	for(int i = 0; i < obstacles.size(); i++){
		//Check for lower extrema
		if(map_.at(obstacles.at(i).y).at(obstacles.at(i).x-offsetChecker) == 0 && map_.at(obstacles.at(i).y).at(obstacles.at(i).x+offsetChecker) == 0 && map_.at(obstacles.at(i).y+offsetChecker).at(obstacles.at(i).x) == 0) {
			//criticalArea for visualizing points
			criticalArea.points.push_back(makePoint(obstacles.at(i).x*0.02, obstacles.at(i).y*0.02));
			lowerPoints.push_back(makePoint(obstacles.at(i).x*0.02, obstacles.at(i).y*0.02));
		}

		//Check for upper extrema
		else if(map_.at(obstacles.at(i).y).at(obstacles.at(i).x-offsetChecker) == 0 && map_.at(obstacles.at(i).y).at(obstacles.at(i).x+offsetChecker) == 0 && map_.at(obstacles.at(i).y-offsetChecker).at(obstacles.at(i).x) == 0){
			criticalArea.points.push_back(makePoint(obstacles.at(i).x*0.02, obstacles.at(i).y*0.02));
			upperPoints.push_back(makePoint(obstacles.at(i).x*0.02, obstacles.at(i).y*0.02));
		}
	}
	//Set up marker variables...
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

	removeDuplicates();
	sortByX();
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

void findMidpoints(){

	for(int i = 0; i < lowerPoints.size(); i++){
		std::cout << "lowerpoint x: " << lowerPoints.at(i).x << "upperPoints x: " << upperPoints.at(i).x << std::endl;
		std::cout << "lowerpoint y: " << lowerPoints.at(i).y << "upperPoints y: " << upperPoints.at(i).y << std::endl;
	}

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

		//Now we have the obstacles and critical points, let's find the midpoints that we're going to navigate to!
		findMidpoints();



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
