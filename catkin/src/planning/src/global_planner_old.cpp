/*
* global_planner
* A node that will generate a global path through a quadtree from a pose to a destination.
*
*	Luc A. Bettaieb
* 	bettaieb@case.edu
*
*	12.12.14
*/
#include <ros/ros.h>

#include <algp2_msgs/Map2D.h>

#include <vector>

ros::Subscriber sub_map2d_;
bool gotMap = false;

std::vector< std::vector<int> > map_(746, std::vector<int>(775, 0)); //regular map
int map_width = 746;
int map_height = 775;


struct obstaclePoint{
	int x;
	int y;
	bool occ;
};

std::vector<obstaclePoint> obstacles;

obstaclePoint makeObstacle(int ex, int wy){
	obstaclePoint p;
	p.x = ex;
	p.y = wy;
	p.occ = true;

	return p;
}

class QNode {
	QNode *nw, *ne, *sw, *se; 	// Pointers to children nodes of the individual QNode
	int x, y; 					// Coordinates
	bool occupied; 				// Data / Value

	public:
		QNode(int ex, int wy, bool occ){
			x = ex;
			y = wy;
			occupied = occ;
		}; //Constructor for QNode

		QNode getNW();
		QNode getNE();
		QNode getSW();
		QNode getSE();
};

class Quadtree {
	QNode root;
	public:
		Quadtree(QNode r){
			root = r;
		};
		QNode grow(obstaclePoint p);
		void insert(obstaclePoint p);
};

obstaclePoint getComparisonPoint(int level, int section){ //0- NE, 1- SE, 2- SW, 3- NW
	obstaclePoint p;
	p.occ = false;

	if(level < 1){ //If on the first level... (level 0)
		p.x = map_width/2;
		p.y = map_height/2;
		return p;

	} else{
		

	}
}

QNode grow(obstaclePoint p){
	QNode newNode(p.x, p.y, p.occ);
	return newNode;
}

void insert(obstaclePoint p){

}


//-----------Regular functions----------//
void processObstacles(){
	//Now insert them into a quad tree.

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

	//Time to insert the obstacles into a the quadtree.
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

	sub_map2d_ = nh.subscribe("/map2d", 10, gotMap2dCB);

	ros::spin();
	return 0;
}