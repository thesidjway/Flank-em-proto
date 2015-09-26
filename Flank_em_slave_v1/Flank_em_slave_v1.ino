// ---------------------------------------------------------------------------
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>

#define SONAR_NUM     3 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 66 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define THRES 10
#define THRESH_UP 50
#define ERROR_THRESH 3
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
int left;
int right;
int forw;
NewPing sonar[SONAR_NUM] = {     // Sensor object array. trig followed by echo 
  NewPing(24, 25, MAX_DISTANCE), // left
  NewPing(22, 23, MAX_DISTANCE), //forw
  NewPing(28, 29, MAX_DISTANCE), // right
 
};

void setup() 
{
      Serial.begin(115200);
     pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
     for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
     pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
      pinMode(9, OUTPUT); //en1
      pinMode(8, OUTPUT); //en2
      pinMode(10,OUTPUT); //left Motor input 1
      pinMode(13,OUTPUT); //left Motor input 2
      pinMode(12,OUTPUT); //right Motor input 1
      pinMode(11,OUTPUT); //right Motor input 2
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);  
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
    }  
    left = cm[0];
    forw = cm[1];
    right = cm[2];
}
void intersectionright() //code to be run if there is a right turn intersection
{
  Serial.println("In intersection right");
 
  get_distance_all_sensors();
  int old_r[3] = {left, forw, right};
  while(!(fabs(left-old_r[1])<= ERROR_THRESH && fabs(forw-old_r[2])<= ERROR_THRESH))
  {
    turnright();
    get_distance_all_sensors(); 
    Serial.print("right : ");Serial.println(right);
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
    Serial.println("left : ");Serial.println(left);
  }
  
}

void goforward()
{
  digitalWrite(10,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
}

void turnleft()
{
  digitalWrite(10,LOW); 
  digitalWrite(13,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
}

void turnright()
{
  digitalWrite(10,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
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
