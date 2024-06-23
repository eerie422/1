#include "ros/ros.h"
#include "topic_package/Num.h"  
#include <iostream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<topic_package::Num>("number_topic", 10);

    ros::Rate loop_rate(1);

    while (ros::ok())
    {
        std::cout << "Enter an integer number: ";
        int number;
        std::cin >> number;

        topic_package::Num msg;
        msg.num = number;

        pub.publish(msg);

        ROS_INFO("Published: %d", msg.num);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
