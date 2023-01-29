#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <my_msgs/SignalData.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pub_node_display");
  ros::NodeHandle nh;
  ros::Publisher pub_number = nh.advertise<my_msgs::SignalData>("/signal/topic", 10);
  ros::Rate loop_rate(1);
  my_msgs::SignalData count;
  
  while(ros::ok())
  {
    pub_number.publish(count);
    ROS_INFO("pub %d", count.data);
    count.data++;
    ros::spinOnce();
    loop_rate.sleep();
  }
}

