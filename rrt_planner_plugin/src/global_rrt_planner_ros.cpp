

#include <rrt_planner_plugin/global_rrt_planner_ros.h>
#include <pluginlib/class_list_macros.h>

//register this planner as a BaseGlobalPlanner plugin
PLUGINLIB_EXPORT_CLASS(global_rrt_planner::GlobalRRTPlanner, nav_core::BaseGlobalPlanner)


namespace global_rrt_planner {



GlobalRRTPlanner::GlobalRRTPlanner() {

	tf_ = new tf::TransformListener(ros::Duration(10));
    initialize(std::string("GlobalRRTPlanner"), std::string("odom"));
}



GlobalRRTPlanner::GlobalRRTPlanner(std::string name, std::string frame_id, tf::TransformListener* tf) {

	tf_ = tf;
    //initialize the planner
    initialize(name, frame_id);
}



GlobalRRTPlanner::~GlobalRRTPlanner() {

	if(rrt_wrapper_)
		delete rrt_wrapper_;
	if(tf_)
		delete tf_;
}


void GlobalRRTPlanner::initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros) {

    initialize(name, std::string("odom"));
}


 
void GlobalRRTPlanner::initialize(std::string name, std::string frame_id) {

    if (!initialized_) {
        ros::NodeHandle private_nh("~/" + name);
        frame_id_ = frame_id;

		sleep(4.0);
		rrt_wrapper_ = new RRT_ros::RRT_ros_wrapper(tf_);


        //private_nh.param("old_navfn_behavior", old_navfn_behavior_, false);

        plan_pub_ = private_nh.advertise<nav_msgs::Path>("global_plan", 1);
        //potential_pub_ = private_nh.advertise<nav_msgs::OccupancyGrid>("potential", 1);

        make_plan_srv_ = private_nh.advertiseService("make_plan", &GlobalRRTPlanner::makePlanService, this);

        //dsrv_ = new dynamic_reconfigure::Server<global_rrt_planner::GlobalRRTPlannerConfig>(ros::NodeHandle("~/" + name));
        //dynamic_reconfigure::Server<global_rrt_planner::GlobalRRTPlannerConfig>::CallbackType cb = boost::bind(
        //        &GlobalRRTPlanner::reconfigureCB, this, _1, _2);
        //dsrv_->setCallback(cb);

        initialized_ = true;
    } else
        ROS_WARN("This planner has already been initialized, you can't call it twice, doing nothing");

}



bool GlobalRRTPlanner::makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal,
                           std::vector<geometry_msgs::PoseStamped>& plan) {
	double cost = 0.0;
    return makePlan(start, goal, plan, cost);
}


bool GlobalRRTPlanner::makePlan(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal, 
std::vector< geometry_msgs::PoseStamped > &plan, double &cost) {
//bool GlobalRRTPlanner::makePlan(const geometry_msgs::PoseStamped& start, const geometry_msgs::PoseStamped& goal, double tolerance, std::vector<geometry_msgs::PoseStamped>& plan) {

	plan.clear();

	bool explore = false;
	if(goal.header.frame_id.empty())
	{
		printf("Global rrt planner. Goal empty! Passing to EXPLORATION MODE!!! \n");
		explore = true;
	} 
	
	//geometry_msgs::PoseStamped start;
	//start.header.stamp = ros::Time::now();
	//start.header.frame_id = "base_link";
	//start.pose.position.x = 0.0;
	//start.pose.position.y = 0.0;
	//start.pose.position.z = 0.0;
	//start.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);
		
	
	plan = rrt_wrapper_->RRT_plan(explore, start, goal, 0.0, 0.0);
	cost = (double)rrt_wrapper_->get_path_cost();
	
	//Visualize the tree nodes of the resulting path
	if(plan.empty())
		return false;

	publishPlan(plan);
	return true;

}





void GlobalRRTPlanner::publishPlan(const std::vector<geometry_msgs::PoseStamped>& path) {
    if (!initialized_) {
        ROS_ERROR(
                "This planner has not been initialized yet, but it is being used, please call initialize() before use");
        return;
    }

    //create a message for the plan
    nav_msgs::Path gui_path;
    gui_path.poses.resize(path.size());

    gui_path.header.frame_id = frame_id_;
    gui_path.header.stamp = ros::Time::now();

    // Extract the plan in world co-ordinates, we assume the path is all in the same frame
    for (unsigned int i = 0; i < path.size(); i++) {
        gui_path.poses[i] = path[i];
    }

    plan_pub_.publish(gui_path);
}



bool GlobalRRTPlanner::makePlanService(nav_msgs::GetPlan::Request& req, nav_msgs::GetPlan::Response& resp) {

    makePlan(req.start, req.goal, resp.plan.poses);

    resp.plan.header.stamp = ros::Time::now();
    resp.plan.header.frame_id = frame_id_;

    return true;
}




} //end namespace global_rrt_planner
