# indires_navigation
ROS metapackage for ground robot navigation and exploration developed for the European Project INDIRES (http://indires.eu/)

![alt text](https://github.com/noeperez/indires_navigation/blob/master/indires_navigation/images/rrt_planning.png)

## Dependencies

* pcl v.1.9.1. Install here: 
* ROS (tested with ROS Kinetic under Ubuntu 16.04). Currently switching to 18.04.

## Contents 

This metapackage contains:

* *rrt_planners*:  
C++ Library and ROS wrapper for path planning in 3D. It contains the following planners:

    - *Simple RRTStar*: RRT* planner in x,y,z coordinates without reasoning about kinodynamic constraints.
    - *RRT*: kinodynamic RRT planner in x,y,z,yaw coordinates.

* *navigation_features_3d*:  
Package for calculation of sample validity and feature functions employed by the cost function of the RRT planners for robot path planning.

* *pcl_filters*:  
ROS package to apply different filters to pointclouds. It makes use of PCL 1.9.

* *global_rrt_planner*:   
ROS plugin that allows to employ the RRT planners as global planner in the move_base architecture for navigation under ROS.

* *local_3d_planner*:   
A local controller in 3D to follow a given global path. It follows the ROS BaseLocalPlanner plugin that allows to employ the controller as local planner in the move_base architecture for navigation under ROS.

* *adapted_move_base*:   
This is a modified version of the original move_base package of ROS. This modification allows to use other global or local planners  (following the ROS move_base premises) that do not use the standard ROS Costamps for planning. Two new boolean parameters are added to indicate the use of the global and/or local costmap (use_global_costmap2d and use_local_costmap2d).

* *costmap_2d*:   
The regular costmap_2d package of ROS has a dependency on PCL. Therefore, as we are using here the version 1.9.1 of the PCL library, we need to include this package with the aim of keeping everything working properly. Only the CMakeLists.txt has been modified to take the PCL version 1.9.1 in comparison with the original ROS package (Kinetic distro). 



The package is a **work in progress** used in research prototyping. Pull requests and/or issues are highly encouraged.
