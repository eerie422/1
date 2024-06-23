iur_ws
### 基础编程题1

这个 ROS 包提供了一个将键盘输入的整型数据发布并由订阅节点接收后加1并打印输出的功能。

## 前提条件

- ROS Noetic
- C++

## 安装

1.	构建包：
 cd ~/catkin_ws
   catkin_make
   ```

2. 刷新环境：
 source ~/catkin_ws/devel/setup.bash
   ```

## 使用方法

1. 启动 ROS 主节点：
   roscore
   ```

2. 在新的终端中，运行发布方节点：
   rosrun topic_packgae publish
   ```

3. 在另一个终端中，运行订阅方节点：
   rosrun topic_package sub
   ```

## 工作原理

1. 发布节点（`publish.cpp`）会提示您输入一个整型数据。这个数据将被发布到指定的话题中。
2. 订阅节点（`sub.cpp`）会接收发布的数据，并对其加1，然后打印输出。

## 使用示例

终端1（发布节点）：

```
Enter an integer number: 1
```

终端2（订阅节点）：

```
Received: 1
Incremented: 2
```

## 节点

- `publish.cpp`: 接收键盘输入的整型数据并发布。
- `sub.cpp`: 订阅发布的整型数据，进行加1处理并打印输出。

## 话题

- `/number_topic` (topic_package/Num): 发布和订阅整型数据。

## 文件结构

```
topic_package/
├── CMakeLists.txt
├── package.xml
├── src/
│   ├── publish.cpp
│   ├── sub.cpp
└── msg/
    └── Num.msg
```

### `CMakeLists.txt`
cmake_minimum_required(VERSION 3.0.2)
project(topic_package)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

add_message_files(
  FILES
  Num.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(publish src/publish.cpp)
add_dependencies(publish {${PROJECT_NAME}_generate_message_cpp}
target_link_libraries(publish ${catkin_LIBRARIES})

add_executable(sub src/sub.cpp)
add_dependencies(sub {${PROJECT_NAME}_generate_message_cpp}
target_link_libraries(sub ${catkin_LIBRARIES})
```

### `publish.cpp`
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

        my_ros_package::Num msg;
        msg.num = number;

        pub.publish(msg);

        ROS_INFO("Published: %d", msg.num);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
```

### `sub.cpp`
#include "ros/ros.h"
#include "topic_package/Num.h"

void numCallback(const topic_package::Num::ConstPtr& msg)
{
    int incremented_num = msg->num + 1;
    ROS_INFO("Received: %d, Incremented: %d", msg->num, incremented_num);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("number_topic", 10, numCallback);

    ros::spin();

    return 0;
}
```

### `Num.msg`
Int64 num
```

## 故障排除

如果遇到任何问题：
1. 确保所有依赖项都已安装。
2. 确保在构建后已经刷新了工作空间。
3. 检查所有节点是否正确运行。
4. 查看终端中的ROS日志消息，以获取更多的错误信息。

通过以上步骤，你可以成功运行你所创建的ROS包，并完成发布节点和订阅节点的通讯演示。


rui_ws

##使用ROS控制RViz中的机器人模型关节角度
概述
本README提供了如何创建一个ROS包，该包包括一个节点用于控制RViz中机器人模型的关节角度。步骤包括创建新的ROS包，使用mini2_description包在RViz中显示机器人模型，以及创建一个节点发布关节状态信息以控制机器人的关节角度。

前提条件
已安装并正确配置ROS（Noetic/foxy/galactic/其他版本）。
在您的ROS工作空间中已有mini2_description包（或等效包）。
已安装RViz。
创建ROS包并控制关节角度的步骤
创建新ROS包

bash
复制代码
cd ~/catkin_ws/src
catkin_create_pkg my_robot_control std_msgs sensor_msgs roscpp
cd ~/catkin_ws
catkin_make
source devel/setup.bash
设置mini2_description包
确保mini2_description包在您的工作空间的src目录中。该包应包括必要的URDF文件和display.launch文件。

在RViz中启动机器人模型

bash
复制代码
roslaunch mini2_description display.launch
创建关节状态发布节点
在my_robot_control包中创建一个新节点，用于发布关节状态消息。

节点脚本 (src/joint_state_publisher.cpp)

cpp
复制代码
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "joint_state_publisher");
    ros::NodeHandle nh;

    ros::Publisher joint_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 10);
    ros::Rate loop_rate(10);

    sensor_msgs::JointState joint_state;
    joint_state.name.resize(1);
    joint_state.position.resize(1);

    while (ros::ok()) {
        joint_state.header.stamp = ros::Time::now();
        joint_state.name[0] = "joint1";  // 替换为实际关节名称
        joint_state.position[0] = 1.0;  // 替换为所需的关节角度

        joint_pub.publish(joint_state);

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
CMakeLists.txt
确保包含以下行以构建节点：

cmake
复制代码
add_executable(joint_state_publisher src/joint_state_publisher.cpp)
target_link_libraries(joint_state_publisher ${catkin_LIBRARIES})
Package.xml
确保声明了依赖关系：

xml
复制代码
<depend>roscpp</depend>
<depend>sensor_msgs</depend>
构建包

bash
复制代码
cd ~/catkin_ws
catkin_make
source devel/setup.bash
运行关节状态发布节点
在一个终端中，启动RViz显示：

bash
复制代码
roslaunch mini2_description display.launch
在另一个终端中，运行关节状态发布节点：

bash
复制代码
rosrun my_robot_control joint_state_publisher
代码与设置说明
joint_state_publisher.cpp：该脚本初始化一个ROS节点，发布关节状态到/joint_states话题。关节状态消息包括关节的名称和所需的角度。
CMakeLists.txt：配置构建过程以编译joint_state_publisher节点。
Package.xml：指定包所需的依赖项。
注意事项
脚本中的关节名称和位置应与机器人模型的URDF中定义的名称和位置相匹配。
根据需要调整循环频率和关节位置，以动态控制机器人的关节。
通过遵循这些步骤，您将能够使用ROS节点控制在RViz中显示的机器人模型的关节角度。





