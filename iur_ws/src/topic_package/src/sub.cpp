#include "ros/ros.h"
#include "topic_package/Num.h"  

void numCallback(const topic_package::Num::ConstPtr& msg)
{
    // 接收到消息后的回调函数
    ROS_INFO("Received: %d", msg->num);
    
    // 对接收到的数据加1
    int incremented_num = msg->num + 1;

    ROS_INFO("Incremented: %d", incremented_num);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    // 创建一个Subscriber，订阅名为 "number_topic" 的话题，消息类型为 topic_package::Num
    ros::Subscriber sub = nh.subscribe("number_topic", 10, numCallback);

    // 使用 ros::spin() 进入循环等待消息到达，并调用回调函数处理消息
    ros::spin();

    return 0;
}
