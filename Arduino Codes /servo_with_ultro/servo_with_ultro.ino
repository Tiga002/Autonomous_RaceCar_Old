#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>
#define USB_USBCON
#define Trig 2 
#define Echo 3 
#define LED 13 
void servo_cb(const std_msgs::UInt16& cmd_msg);
void esc_cb(const std_msgs::UInt16& cmd_msg);

ros::NodeHandle nh;
Servo servo;
Servo esc;
int cm; //距离  
float temp; 
int servo_value;
int esc_value;
std_msgs::UInt16 servo_back_value;
std_msgs::UInt16 esc_back_value;
std_msgs::UInt16 distance;
ros::Publisher pub_back_distance("chatter", &distance);
ros::Publisher pub_back_servo("servo_back", &servo_back_value);
ros::Publisher pub_back_esc("esc_back", &esc_back_value);
ros::Subscriber<std_msgs::UInt16> servo_sub("servo", servo_cb);
ros::Subscriber<std_msgs::UInt16> esc_sub("esc", esc_cb);
void setup(){
  pinMode(LED, OUTPUT);
  pinMode(Trig, OUTPUT);  
  pinMode(Echo, INPUT);  
  Serial.begin(57600);
  nh.initNode();
  nh.subscribe(servo_sub);
  nh.subscribe(esc_sub);
  nh.advertise(pub_back_distance);
  nh.advertise(pub_back_servo);
  nh.advertise(pub_back_esc);
  servo.attach(9); //attach it to pin 9
  esc.attach(10); //attach it to pin 10
  servo.writeMicroseconds(1350);
  esc.writeMicroseconds(1500);
}

void loop(){
  digitalWrite(Trig, LOW); //给Trig发送一个低电平  
  //delayMicroseconds(2);    //等待 2微妙  
  digitalWrite(Trig,HIGH);  
  //delayMicroseconds(10);    
  digitalWrite(Trig, LOW);   
  temp = float(pulseIn(Echo, HIGH)); //存储回波等待时间
  cm = int((temp * 17 )/1000); //把回波时间换算成cm  
  distance.data = cm;
  pub_back_distance.publish( &distance );
  nh.spinOnce();
  delay(2);
  //;
}
void servo_cb(const std_msgs::UInt16& cmd_msg) {
  servo.writeMicroseconds(cmd_msg.data);
  servo_back_value.data = cmd_msg.data;
  digitalWrite(13, HIGH-digitalRead(13));
  pub_back_servo.publish(&servo_back_value);
  ///pub_back_esc.publish(&esc_value);
}
void esc_cb(const std_msgs::UInt16& cmd_msg) {
  esc.writeMicroseconds(cmd_msg.data);
  esc_back_value.data = cmd_msg.data;
  pub_back_esc.publish(&esc_back_value);
  digitalWrite(13, HIGH-digitalRead(13));
}

