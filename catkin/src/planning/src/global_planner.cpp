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

/*----------------Fields---------------------*/
bool gotMap = false;
const int width = 1230;
const int height = 630;

/*----------------ROS Fields--------*/
ros::Subscriber sub_map2d_;
ros::Publisher pub_critArea_;
ros::Publisher pub_path_;
ros::Publisher pub_poseArray_;

visualization_msgs::Marker criticalArea;
nav_msgs::Path path;
geometry_msgs::PoseArray poseArray_;

/*----------------Vector Fields-----*/
std::vector< std::vector<int> > map_(height, std::vector<int>(width, 0)); //regular map.at(y).at(x)
std::vector<obstaclePoint> obstacles;
std::vector<geometry_msgs::Point> lowerPoints;
std::vector<geometry_msgs::Point> upperPoints;

/*----------------Helper Functions--*/
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

geometry_msgs::PoseStamped makePoseXY(float x, float y, int iSeq){
	geometry_msgs::PoseStamped poseXY;

	poseXY.header.seq = iSeq;
	poseXY.header.stamp = ros::Time().now();
	poseXY.header.frame_id = "map";

	poseXY.pose.position.x = x;
	poseXY.pose.position.y = y;
	poseXY.pose.position.z = 0;

	return poseXY;
	//Assign quaternion later as it will depend on the next pose.

}

/*----------Regular Functions------*/

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
bool wayToSort(geometry_msgs::Point i, geometry_msgs::Point j){
	return i.x < j.x;
}
void sortByX(){
	std::sort(lowerPoints.begin(), lowerPoints.end(), wayToSort);
	std::sort(upperPoints.begin(), upperPoints.end(), wayToSort);
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
	criticalArea.header.frame_id = "map";
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

	//Now we can kill map_ and obstacles
	//This destroys vectors and frees memory by swapping it with vector that does not really exist.
	//std::vector< std::vector<int> >().swap(map_);
	//std::vector<obstaclePoint>().swap(obstacles);
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
	//Here were going to create a path!
	
	for(int i = 0; i < lowerPoints.size(); i++){	
		path.poses.push_back(makePoseXY(lowerPoints.at(i).x, (upperPoints.at(i).y-0.5*(upperPoints.at(i).y-lowerPoints.at(i).y)), i));
	}
	for(int i = 0; i < path.poses.size(); i++){ //add to marker fo viewin'
		criticalArea.points.push_back(path.poses.at(i).pose.position);
	}

}

void assignAngles(){
	for(int i = 0; i < path.poses.size()-1; i++){
   	 //Iterate through poses minus last because it is the goal pose and has no next!
    	path.poses.at(i).pose.orientation.x = (path.poses.at(i+1).pose.position.x - path.poses.at(i).pose.position.x) * (path.poses.at(i+1).pose.position.y - path.poses.at(i).pose.position.y);
    	//path.poses.at(i).pose.orientation.y = (path.poses.at(i+1).pose.position.x - path.poses.at(i).pose.position.x) * (path.poses.at(i+1).pose.position.y - path.poses.at(i).pose.position.y);
    	path.poses.at(i).pose.orientation.y = 0;
    	path.poses.at(i).pose.orientation.z = (path.poses.at(i+1).pose.position.x - path.poses.at(i).pose.position.x) * (path.poses.at(i+1).pose.position.y - path.poses.at(i).pose.position.y);
    	
    	path.poses.at(i).pose.orientation.w = sqrt((pow((path.poses.at(i+1).pose.position.x - path.poses.at(i).pose.position.x), 2) + pow((path.poses.at(i+1).pose.position.y - path.poses.at(i).pose.position.y), 2)));
  		
  		double divisor = sqrt(pow(path.poses.at(i).pose.orientation.x, 2) + pow(path.poses.at(i).pose.orientation.y, 2) + pow(path.poses.at(i).pose.orientation.z, 2) + pow(path.poses.at(i).pose.orientation.w, 2));
  		path.poses.at(i).pose.orientation.x /= divisor;
  		path.poses.at(i).pose.orientation.y /= divisor;
  		path.poses.at(i).pose.orientation.z /= divisor;
  		path.poses.at(i).pose.orientation.w /= divisor;
  	}
  	//why not assign path a header now?

  	path.header.seq = 1337;  //h4ck3r cr3d
  	path.header.stamp = ros::Time().now();
  	path.header.frame_id = "map";
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
		//Now the midpoints have been calculated, we will need to set the angles of the poses to point toward the next goal
		//!!Quaternions are scary
		assignAngles();

		// for(int i = 0; i < path.poses.size(); i++){
		// 	std::cout << "x: " << path.poses.at(i).pose.position.x << std::endl;
		// 	std::cout << "y: " << path.poses.at(i).pose.position.y << std::endl;
		// 	std::cout << "z: " << path.poses.at(i).pose.position.z << std::endl;

		// 	std::cout << "x: " << path.poses.at(i).pose.orientation.x << std::endl;
		// 	std::cout << "y: " << path.poses.at(i).pose.orientation.y << std::endl;
		// 	std::cout << "z: " << path.poses.at(i).pose.orientation.z << std::endl;
		// 	std::cout << "w: " << path.poses.at(i).pose.orientation.w << std::endl;

		// }
	}
}

void createPoseArray(){
	for(int i = 0; i < path.poses.size(); i++)
		poseArray_.poses.push_back(path.poses.at(i).pose);
	poseArray_.header.seq = 1;
	poseArray_.header.stamp = ros::Time().now();
	poseArray_.header.frame_id = "map";
}

int main(int argc, char** argv){
	ros::init(argc, argv, "global_planner");
	ros::NodeHandle nh;
	//First, let's get the map.
	ROS_INFO("Let's do some global planning! :)");
	pub_critArea_ = nh.advertise<visualization_msgs::Marker>("/criticalArea", 1);
	pub_path_ = nh.advertise<nav_msgs::Path>("/path", 1);
	pub_poseArray_ = nh.advertise<geometry_msgs::PoseArray>("/poseArray",1);

	while(ros::ok()){
		sub_map2d_ = nh.subscribe("/map2d", 10, gotMap2dCB);
		pub_critArea_.publish(criticalArea);
		pub_path_.publish(path);

		createPoseArray();
		pub_poseArray_.publish(poseArray_);
		ros::Duration(0.05).sleep();

		ros::spinOnce();
	}

	ros::spin();
	return 0;
}