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

#include <vector>

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

int main(int argc, char** argv){
	ros::init(argc, argv, "global_planner");

	ros::spin();
	return 0;
}