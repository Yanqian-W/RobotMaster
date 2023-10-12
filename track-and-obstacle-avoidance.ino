#include<Servo.h> 
// Servo initialization
Servo Servo_left;
Servo Servo_right; 
// Ultrasonic pin
#define TrigPin 12  // output port
#define EchoPin 11  // input port
// Gray detection pin
#define track1 3  // left
#define track2 2  // right

#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define TURNLEFT 3
#define TURNRIGHT 4
float distance = 0;
int Sensor[2] = {0, 0};  // Initialize the value of gray scale sensors

void setup() {
  Serial.begin(9600);
  Servo_left.attach(9);
  Servo_right.attach(5);
  pinMode(EchoPin,INPUT);
  pinMode(TrigPin,OUTPUT);
  pinMode(track1, INPUT);
  pinMode(track2, INPUT);
}

void motorRun(int cmd)
{
  switch(cmd){
    case FORWARD:
      //Serial.println("FORWARD");
      Servo_left.writeMicroseconds(2000);
      Servo_right.writeMicroseconds(1000);
    break;
    case BACKWARD:
      //Serial.println("BACKWARD");
      Servo_left.writeMicroseconds(1000);
      Servo_right.writeMicroseconds(2000);
      break;
    case TURNLEFT:
      //Serial.println("TURN LEFT"); 
      Servo_left.writeMicroseconds(1250);
      Servo_right.writeMicroseconds(1000);
      break;
    case TURNRIGHT:
      //Serial.println("TURN RIGHT");
      Servo_left.writeMicroseconds(2000);
      Servo_right.writeMicroseconds(1750);
      break;
    default:
      //Serial.println("STOP"); 
      Servo_left.writeMicroseconds(1500);
      Servo_right.writeMicroseconds(1500);
  }
}

int getDistance()
{
  digitalWrite(TrigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  distance = pulseIn(EchoPin, HIGH);
  distance= distance/58;  // Convert pulse time to distance in cm
  // print the distance value
  //Serial.print(distance);
  //Serial.println("cm");
  delay(10);
  return distance;
}

void avoidance()
{
  distance = getDistance();     // get the distance of obstacle

  // If distance <= 13cm, start avoiding the obstacle
  if(distance <= 13){    // Make the car bypass the obstacle and return         
    motorRun(TURNLEFT);  // to the track through a series of actions
    delay(1350);                           
    motorRun(FORWARD);
    delay(500);                 
    motorRun(TURNRIGHT);
    delay(1500);
    motorRun(FORWARD);
    delay(1500);
    motorRun(TURNRIGHT);
    delay(1500);
    motorRun(FORWARD);
    delay(500);
    motorRun(TURNLEFT);
    delay(1400);
  }
}

void Sensor_Read()
{
  Sensor[0] = analogRead(A2); 
  Sensor[1] = analogRead(A3);
  //print the value of gray scale sensors
  Serial.print(Sensor[0]);
  Serial.print("   ");
  Serial.println(Sensor[1]);
}

void tracking() 
{
  if (Sensor[0] > 100 && Sensor[1] < 100)  // 1-0 - turn left
    motorRun(TURNLEFT); 
  else if (Sensor[0] < 100 && Sensor[1] > 100)  // 0-1 - turn right
    motorRun(TURNRIGHT); 
  else
    motorRun(FORWARD);
}

void loop() {
  Sensor_Read();  // Constantly read the high and low levels of the tracking module
  tracking(); 
  delay(10);
  avoidance();
}






