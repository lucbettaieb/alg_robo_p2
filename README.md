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
			This node will run an algorithm to perform a approximate cell-decomposition on the shown map.  This will

	</li>
	<li><b>Global Planner</b><br />


	</li>
	<li><b>Local Planner/Controller</b><br />


	</li>
</ul>