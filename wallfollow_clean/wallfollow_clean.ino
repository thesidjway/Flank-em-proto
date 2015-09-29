#include <NewPing.h>

#define SONAR_NUM     3 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 66 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define THRES 12
#define THRESH_UP 50
#define ERROR_THRESH 8
#define IN1 10
#define IN2 13
#define IN4 11
#define IN3 9
#define EN1 12
#define EN2 6
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

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
void stall();
int left;
int right;
int forw;
NewPing sonar[SONAR_NUM] = {     // Sensor object array. trig followed by echo 
  NewPing(37, 36, MAX_DISTANCE), // left
  NewPing(39, 38, MAX_DISTANCE), //forw
  NewPing(35, 34, MAX_DISTANCE), // right
 
};

void setup() 
{
      Serial.begin(115200);
      pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
      for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
      pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
      pinMode(EN1, OUTPUT); //en1
      pinMode(EN2, OUTPUT); //en2
      pinMode(IN1,OUTPUT); //left Motor input 1
      pinMode(IN2,OUTPUT); //left Motor input 2
      pinMode(IN4,OUTPUT); //right Motor input 1
      pinMode(IN3,OUTPUT); //right Motor input 2
      digitalWrite(EN2, HIGH);
      digitalWrite(EN1, HIGH);  
}

void loop()
{
     
 get_distance_all_sensors();
  Serial.print("Left: ");
  Serial.print(left);
  Serial.print(" Forw: ");
  Serial.print(forw);
  Serial.print(" Right: ");
  Serial.println(right);
  if(left<THRES && right != 0 && left != 0 && right<THRES && (forw>THRES||forw == 0))
    goforward();
  else if((left >= THRESH_UP) && (right<THRES && right != 0))
    intersectionleft();
  else if((left<THRES && left!= 0) && (right >= THRESH_UP))
    intersectionright();
   else
   {
    stall();
   }
  // The rest of your code would go here.
}
void get_distance_all_sensors()
{
     for (uint8_t i = 0; i < SONAR_NUM; i++) 
      { // Loop through all the sensors.
       if (millis() >= pingTimer[i]) 
       {         // Is it this sensor's time to ping?
          pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
          if (i == 0 && currentSensor == SONAR_NUM - 1) //oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
          sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
          currentSensor = i;                          // Sensor being accessed.
          cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
          sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
      }
    }  //
    left = cm[0];
    forw = cm[1];
    right = cm[2];
}
void intersectionright() //code to be run if there is a right turn intersection
{
  get_distance_all_sensors();
  int old_r[3] = {left, forw, right};
  while(!(fabs(left-old_r[1])<= ERROR_THRESH && fabs(forw-old_r[2])<= ERROR_THRESH))
  {
    turnright();
    get_distance_all_sensors(); 
    Serial.println("right");
    Serial.print("IN1 : ");Serial.println(digitalRead(IN1));
    Serial.print("IN2 : ");Serial.println(digitalRead(IN2));
    Serial.print("IN3 : ");Serial.println(digitalRead(IN3));
    Serial.print("IN4 : ");Serial.println(digitalRead(IN4));
    
  }
}

void intersectionleft() //code to be run if there is a left turn intersection
{
  Serial.println("In intersection left");
  get_distance_all_sensors();
  int old_r[3] = {left, forw, right};
  while(!(fabs(right-old_r[1])<= ERROR_THRESH && fabs(forw-old_r[0])<= ERROR_THRESH))
  {
    turnleft();
    get_distance_all_sensors();
    Serial.println("left");
  
  }
  
}

void goforward()
{
  Serial.println("###########GO FORWARD ###########");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
    Serial.print("IN1 : ");Serial.println(digitalRead(IN1));
    Serial.print("IN2 : ");Serial.println(digitalRead(IN2));
    Serial.print("IN3 : ");Serial.println(digitalRead(IN3));
    Serial.print("IN4 : ");Serial.println(digitalRead(IN4));
}

void turnleft()
{
  Serial.println("###########LEFT TURN ###########");
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
    Serial.print("IN1 : ");Serial.println(digitalRead(IN1));
    Serial.print("IN2 : ");Serial.println(digitalRead(IN2));
    Serial.print("IN3 : ");Serial.println(digitalRead(IN3));
    Serial.print("IN4 : ");Serial.println(digitalRead(IN4));
}

void turnright()
{
  Serial.println("###########RIGHT TURN ###########");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  
    Serial.print("IN1 : ");Serial.println(digitalRead(IN1));
    Serial.print("IN2 : ");Serial.println(digitalRead(IN2));
    Serial.print("IN3 : ");Serial.println(digitalRead(IN3));
    Serial.print("IN4 : ");Serial.println(digitalRead(IN4));
}
void correctright()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN1, 150);
  analogWrite(EN2, 200);
}
void correctleft()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN1, 200);
  analogWrite(EN2, 150);
}
void stall()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}
  
void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
