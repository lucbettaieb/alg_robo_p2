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

std::vector< std::vector<int> > map_(746, std::vector<int>(775, 0));
std::vector

struct obstaclePoint{
	int x;
	int y;
	bool occ;
};

class QNode {
	QNode *nw, *ne, *sw, *se; 	// Pointers to children nodes of the individual QNode
	int x, y; 					// Coordinates
	bool occupied; 				// Data / Value

	public:
		QNode(int x, int y, bool occupied); //Constructor for QNode
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

void gotMap2dCB(const algp2_msgs::Map2D &map2d){
	//Got the map.  Now parse back into regular 2d Vector
	//std::cout << "col size" << map2d.columns.size() << std::endl;
	//std::cout << "row size" << map2d.columns.at(0).data.size() << std::endl;
	for(int i = 0; i < map2d.columns.size(); i++){
		for(int j = 0; j < map2d.columns.at(0).data.size(); j++){
			map_.at(i).at(j) = map2d.columns.at(i).data.at(j);
		}
	}

}

int main(int argc, char** argv){
	ros::init(argc, argv, "global_planner");
	ros::NodeHandle nh;
	//First, let's get the map.
	sub_map2d_ = nh.subscribe("/map2d", 10, gotMap2dCB);

	//Second, we'll need to find all of the obstacles and their associated positions within the map.

	std::cout << "col size" << map_.size() << std::endl;
	std::cout << "row size" << map_.at(0).size() << std::endl;

	ros::spin();
	return 0;
}