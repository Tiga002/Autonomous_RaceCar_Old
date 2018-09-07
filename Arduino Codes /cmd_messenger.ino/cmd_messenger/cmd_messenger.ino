#include <Servo.h>
String readString;
String data;
String flag;

int count = 0;
int esc_CMD = 1570, servo_CMD = 1350;
Servo servo;
Servo esc;



void setup()
{
  Serial.begin(9600);
  servo.attach(9); //attach it to pin 9
  esc.attach(10); //attach it to pin 10
  servo.writeMicroseconds(1350);
  esc.writeMicroseconds(1500);
  
}
void loop()
{
  
  //Serial.println("Hi");
  while (!Serial.available()) {}

  while (Serial.available()) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }

  int i = 0;
  while (readString.length() > 0 && i < readString.length())
  {
    char c = readString[i];
    if (c == ' ')
    {
      esc_CMD = count;
      count = 0;
    }
    else
      count = count * 10 + (c - '0');

    i++;
  }

  if (count > 0 && count <= 2500)
    servo_CMD = count;
  Serial.println(servo_CMD);
  if (servo_CMD > 2500)
  {
    servo_CMD = 2500;
  }
  if (servo_CMD < 250)
  {
    servo_CMD = 250;
  }


    esc.writeMicroseconds(esc_CMD);
    servo.writeMicroseconds(servo_CMD);

  
  //    if (readString.length() < 0)
  //    {
  //      Serial.println(servo_CMD);
  //    }

  count = 0;
  readString = "";
  Serial.flush();

}
