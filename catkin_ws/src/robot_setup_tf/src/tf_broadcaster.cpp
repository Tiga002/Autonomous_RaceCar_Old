/* This Node is responsoble for publishing the transform in our system
   base_link = the rational center of our RaceCar
   base_laser = position of the rpLidar
*/

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

// Main Function of the Node
int main(int argc, char** argv)
{
    // Initialize the Node
    ros::init(argc, argv, "robot_tf_publisher");
    ros::NodeHandle n;
  
   // Setup the running rate
    ros::Rate r(100);
   
   // Create a Broadcaster object that will be used to send the base_link --> base_laser transformation
    tf::TransformBroadcaster broadcaster;

    while(n.ok())
    {
        /* Here is the Real Work is done. Sending a transform with the broadcaster requires 5 input
           1 : Rotational Transform expressed in Quaternion form
	   2 : Translational Transform : Base is referenced from the parent frame (base_link)
           3 : Timestamp
	   4 : Name of the Parent Frame
           5 : Name of the Child Frame
        */
         broadcaster.sendTransform(
	     tf::StampedTransform(
     	         tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.1, 0.0, 0.2)),
                 ros::Time::now(),
                 "base_link",
                 "base_laser"));
         r.sleep();
    }
}

