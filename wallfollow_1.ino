#include <NewPing.h>
#include<Servo.h>
Servo myservo;
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 30 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define THRES 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
void intersctionright();
void intersectionleft();
void turnright();
void turnleft();
void correctright();
void correctleft();
void moveservo(int);
int getdistleft();
int getdistright();
int getdistfor();
void goforward();


void setup() {
  myservo.attach(7); //Attaching Servo input pin to pin no. 7
  pinMode(2,OUTPUT); //left Motor input 1
  pinMode(3,OUTPUT); //left Motor input 2
  pinMode(4,OUTPUT); //right Motor output 1
  pinMode(5,OUTPUT); //right Motor output 2
  Serial.begin(115200); 
}

void loop() {
  int left=getdistleft(); 
  int right=getdistright();
  int forw=getdistfor();
  Serial.print("Left: ");
  Serial.print(left);
  Serial.print(" Forw: ");
  Serial.print(forw);
  Serial.print(" Right: ");
  Serial.println(right);
  if(left<THRES && right<THRES && forw>THRES)
    goforward();
  else if(left>THRES && right<THRES)
    intersectionleft();
  else if(left<THRES && right>THRES)
    intersectionright();
}

void intersectionright() //code to be run if there is a right turn intersection
{
  Serial.println("In intersection right");
  myservo.write(90);
  while(getdist()<MAX_DISTANCE-1)
  {
    turnright();
  }
}

void intersectionleft() //code to be run if there is a left turn intersection
{
  Serial.println("In intersection left");
  myservo.write(90);
  while(getdist()<MAX_DISTANCE-1)
  {
    turnleft();
  }
  
}
  
int getdist() //function for getting distance
{           
  //Serial.println(" IN GETDIST");
  int uS = sonar.ping();
  int dist=uS/ 59; //for converting microseconds to cm distance 
  return dist; 
}

void moveservo(int x)
{
  //Serial.println(" IN MOVE SERVO");
  myservo.write(x);
}

int getdistleft() //rotates the servo to the left and finds the distance
{
  //Serial.println(" IN GETDIST LEFT");
  moveservo(0);
  delay(500);
  int y=getdist();
  return y;
}

int getdistfor() //rotates the servo to the front and finds the distance
{
  moveservo(90);
  delay(500);
  int y=getdist();
  return y;
}

int getdistright() //rotates the servo to the right and finds the distance
{
  moveservo(180);
  delay(1000);
  int y=getdist();
  return y;
}

void goforward()
{
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
}

void turnleft()
{
  digitalWrite(2,LOW); 
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
}

void turnright()
{
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
}
void correctright()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  analogWrite(1, 150);
  analogWrite(6, 200);
}
void correctleft()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  analogWrite(1, 200);
  analogWrite(6, 150);
}
