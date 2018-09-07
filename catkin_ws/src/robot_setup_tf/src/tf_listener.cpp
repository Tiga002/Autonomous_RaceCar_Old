/* This Node will take in the published transform,
   to transform a point taken from "base_laser" to "base_link" frame 
*/

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PointStamped.h>

/* Creating a Listerner Object which can automatically subscribes the transform message */
void transformPoint(const tf::TransformListener& listener)
{
    /* Here we demo a laser message (points) in the base_laser,
       and we would like to transform it back to base_link
    */
    geometry_msgs::PointStamped laser_point;
    laser_point.header.frame_id = "base_laser";
    //Set this points are taken from "base_laser"
    // we will just use the most recent transform available
    laser_point.header.stamp = ros::Time();
    // For Demo, Just setting a arbitrary point
    laser_point.point.x = 1.0;
    laser_point.point.y = 0.2;
    laser_point.point.z = 0.0;
    
    try{
        geometry_msgs::PointStamped base_point;
        /* To perform the Transformation, we call transformPoint() with three arguments: 
           1 : desired frame's name
           2 : Point we are transforming
           3 : Where our transformed point will be saved 
        */
        listener.transformPoint("base_link", laser_point, base_point);
        
        //Display the Transformation
        ROS_INFO("base_laser : [ %.2f, %.2f, %.2f] ---> base_link : [ %.2f, %.2f, %.2f] at time %.2f",                  laser_point.point.x, laser_point.point.y, laser_point.point.z,
                  base_point.point.x, base_point.point.y, base_point.point.z,
                  base_point.header.stamp.toSec());
    }
    catch(tf::TransformException& ex){
        ROS_ERROR("Recevied an exception: %s", ex.what());
    }
}

// Node Main Function
int main(int argc, char** argv)
{
    // Initialize the Node
    ros::init(argc, argv, "robot_tf_listener");
    ros::NodeHandle n;
    
    // Create the Listener Object
    tf::TransformListener listener(ros::Duration(10));
 
    // Transform a point once every second
    ros::Timer timer = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener)));

    ros::spin();

}


