#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

class JoyToTwist
{
private:
  ros::NodeHandle nh;
  ros::Publisher vel_pub;
  ros::Subscriber joy_sub;
  ros::Timer timer;

  sensor_msgs::Joy joy_;
  bool is_joy_;

public:
  JoyToTwist();
  void joyCallback(const sensor_msgs::Joy& joy);
  void timerCallback(const ros::TimerEvent& e);
};

JoyToTwist::JoyToTwist()
{
    joy_sub = nh.subscribe("/joy", 100, &JoyToTwist::joyCallback, this);
    vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 100);
    timer = nh.createTimer(ros::Duration(0.01), &JoyToTwist::timerCallback, this);
}

void JoyToTwist::joyCallback(const sensor_msgs::Joy& msg)
{
  joy_ = msg;
  is_joy_ = true;
}

void JoyToTwist::timerCallback(const ros::TimerEvent& e) {
  (void) e;

  if (!is_joy_) return;

  geometry_msgs::Twist cmd_vel;
  cmd_vel.linear.x = joy_.axes[1];
  cmd_vel.angular.z = joy_.axes[3];

  vel_pub.publish(cmd_vel);
}



int main(int argc, char *argv[])
{
  ros::init(argc, argv, "joy_to_twist");
  JoyToTwist joytotwist;

  ros::spin();
}