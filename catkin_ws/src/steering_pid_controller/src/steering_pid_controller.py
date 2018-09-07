#!/usr/bin/env python

from __future__ import division
import rospy
#from data_service.msg import TimeData
#from barc.msg import ECU
from std_msgs.msg import UInt16, Float32
from math import pi,sin,radians
import time
import serial
from numpy import zeros, hstack, cos, array, dot, arctan
#from input_map import angle_2_servo, servo_2_angle

import serial
import json
import syslog,time,sys
global object_detected_index, flag
#################################################################

port = '/dev/ttyACM0'

# occupy the port all the time to save time
ard = serial.Serial(port,9600,timeout=1)
#global object_detected_index
def SendToSerial(esc, servo):
    global ard

    send =""
    send += str(int(esc))
    send += " "
    send += str(int(servo))

    ard.flush()
    send = str(send)
    #print ("Python value sent: ")
    #print (send)
    ard.write(send)

    #for testing
    time.sleep(0.01)
    msg = ard.readline().strip('\n\r') #ard.read(ard.inWaiting()).strip('\n\r') 
    print ("Message from arduino: ")
    print (msg)
    print ('\n')
    #print msg

    #exit()
#################################################################

# Steering Offset Global Variables

offset = 0.0
offset_prev = 0.0
object_detected_index = 0
def offset_callback(data):
    # Store current and previous Lane Pixel Offset
    global servo_pub
    global esc_pub
    global offset
    global offset_prev    
    global object_detected_index, flag
    rate = rospy.Rate(1)
    if data is not None:
        offset_prev = offset
        offset = float(data.data)
	sign = offset
        offset = abs(offset)
        

    # set node rate
  
    # Get Controller Parameters
        Kp = 40
        Kd = 0
        servo_CMD = 0
        
        if object_detected_index == 2:
            offset = offset + 4
           

        PID_output = steering_pid_control(offset, offset_prev, Kp, Kd)
        if sign > 0:
            servo_CMD = 1350 + PID_output
            print(sign)
        elif sign < 0:
            servo_CMD = 1350 - PID_output
            print(sign)
        else: 
	    None
        
        if servo_CMD >= 2000:
            servo_CMD = 2000
        elif servo_CMD < 800:
	    servo_CMD = 800
        else:
            None
        
        if object_detected_index == 1:  # person detected
            esc_CMD = 1500
   
        elif object_detected_index == 3: # Slow detected
            esc_CMD = 1570
        elif object_detected_index == 4:
            esc_CMD = 1585
        else: 
            esc_CMD = 1585

    # send command signal
        #servo_pub.publish(UInt16(servo_CMD))
        #esc_pub.publish(esc_CMD)
        SendToSerial(esc_CMD, servo_CMD)
        print('Current Error:')
        print(offset)
        print('PID output is :')
        print(PID_output)
        print('Servo Command output is :')
        print(servo_CMD)
        print('ESC command output is:')
        print(esc_CMD)
        print("==============================================")
            
    return 0

def steering_pid_control(offset, offset_prev, Kp, Kd):

    # Current Error, Derivative of Error, Intergral of Error
    if offset < 1:
        return 0
    e = offset
    ed = e - offset_prev
    #ei = ei + (e / float(rateHz))
    # Calulate Turn Angle
    pid_output = (e * Kp)
    # Saturate Turn Value
    #if (pid_output < -70.0):
      #  pid_output = -70.0
    #elif (pid_output > 70.0):
     #   pid_output = 70.0
    return pid_output

def object_detected_callback(data):
    global object_detected_index
    if data is not None:
        object_detected_index = data.data
        print("Hiiiii")
        print(object_detected_index)
    else:
        object_detected_index = 0
    return 0

#############################################################
def main_auto():
    # initialize ROS node
    global esc_pub
    global servo_pub
    rospy.init_node('auto_mode', anonymous=True)
    #esc_pub= rospy.Publisher('esc', UInt16, queue_size = 1)
    #servo_pub = rospy.Publisher('servo', UInt16, queue_size = 1)
    steering_offset_subscriber = rospy.Subscriber("lane_offset", Float32, offset_callback)
    object_detected_subscriber = rospy.Subscriber("object_detected", UInt16, object_detected_callback)
    rospy.spin()
#############################################################
if __name__ == '__main__':
	try:
		main_auto()
	except rospy.ROSInterruptException:
		pass
