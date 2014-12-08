<h3>Algorithmic Robotics, Project 2</h3>
<h4>A Goal-Subdividing Motion Planning System</h4>

<p>This project will be demonstrating the potential for a goal-subdividing robotic motion planning system.  The motivation for this project is to provide a better way for CWRU's Autonomous Snowplow to be seeded a goal and perform the motions required to reach the goal. </p>

<p> Previously, a lot of slipping was occuring while the robot was driving toward its goal.  It is my hope, that by subdiving the path into separate miniature goals, a more closed-loop system can be created; causing the SnowMower to have less motion error and be able to perform its task more effectively.</p>

<h5>Nodes</h5>
<ul>
	<li><b>Main Controller</b><br />
			This node will query a user for a <x, y, theta>.  It will then stuff that data to a global Pose2D goal and publish it.  After publishing, it will listen on /robot0/odom and continually check if the robot has arrived at the designated goal within some tolerance.  When the robot has arrived at the goal pose, the node will query the user for a new goal.

	</li>
	<li><b>Global/Local Mapper</b><br />
			This node will run an algorithm to perform a approximate cell-decomposition on the shown map.  This will be used for the global path plan and will avoid obstacles that show up in the cell decomposition.  Additionally, the mapping node will use costmap_2d to create a costmap which will assist in local navigation to subgoals.

	</li>
	<li><b>Global Planner</b><br />
			This node will listen for a Pose2D either from the main controller or from RViz's publish goal pose utility.  After the global goal pose is determined, the node will create a path through the global map to the goal pose.


	</li>
	<li><b>Local Planner/Controller</b><br />
			Will data from the global map and path to create a series of subgoals and add them to a queue.  Using data from costmap_2d, the node will drive the robot forward towards the local goal while avoiding areas with higher 'cost' as assigned by the costmap_2d.  This essentially can be viewed as a discrete potential field model which solves the problem of local minima by dividing the path into smaller, more reachible subgoals. <b>thought:  perhaps gradually decrease cost manually of spaces/cells in the costmap as they near the goal.</b>  Upon reaching the subgoal within some tolerance, the queue will pop and new subgoal will in play. <b>I should try to make it so the robot does not slow down between goals. </b>


	</li>
</ul>