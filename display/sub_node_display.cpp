#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <my_msgs/SignalData.h>

void NumberCallback(const my_msgs::SignalData &msg)
{
    ROS_INFO("sub %d", msg.data);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "sub_node_display");
    ros::NodeHandle nh;
    ros::Subscriber sub_number = nh.subscribe("/signal/topic2", 10, NumberCallback); 
    ros::spin();
}
