#include <ros/ros.h>

#include <algp2_msgs/Map2D.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>

#include <vector>

struct obstaclePoint{
	int x;
	int y;
	bool occ;
};

obstaclePoint makeObstacle();