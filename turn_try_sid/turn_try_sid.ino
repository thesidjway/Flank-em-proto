#include <NewPing.h>
#define SONAR_NUM     3 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 66 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define THRES 20
#define THRESH_UP 10
#define ERROR_THRESH 20
#define TURN_THRES 40
#define THRES_FOR 10


#define IN1 26
#define IN2 27
#define IN4 24
#define IN3 25
#define EN1 2
#define EN2 5


int flag_turn = 0;
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
int cycle = 0;
void turnright();
void turnleft();
void correctright();
void correctleft();
void goforward();
void stall();
int left;
int right;
int forw;
int leftstatus = 0;
int movestatus = 1;

NewPing sonar[SONAR_NUM] = {     // Sensor object array. trig followed by echo
  NewPing(37, 36, MAX_DISTANCE), // left
  NewPing(47, 46, MAX_DISTANCE), //forw
  NewPing(35, 34, MAX_DISTANCE), // right
};

void setup()
{
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

  pinMode(IN1, OUTPUT); //left Motor input 1
  pinMode(IN2, OUTPUT); //left Motor input 2
  pinMode(IN4, OUTPUT); //right Motor input 1
  pinMode(IN3, OUTPUT); //right Motor input 2

}


void loop()
{

  get_distance_all_sensors();
 



  if (movestatus == 1)
  {
    while (forw > THRES_FOR)
    {
      if (right == left)
      {
        goforward();
      }
      else if (right > left)
      {
        correctleft();
      }
      else if (left > right)
      {
        correctright();
      }
      get_distance_all_sensors();
    }
    movestatus = 0;
    stall();
  }

  else if (movestatus == 0)
  {
    int temp_left, temp_forw, temp_right;
    temp_left = left;
    temp_forw = forw;
    temp_right = right;

    if (left > THRES && right < THRES)
      leftstatus = 1;
    else if (left < THRES && right > THRES)
      leftstatus = 0;

    if (leftstatus == 1)
    {
      while (fabs(right - temp_forw) > THRESH_UP && fabs(forw - temp_right) > THRESH_UP)
      {
        turnleft();
        get_distance_all_sensors();
      }
    }
    else if (leftstatus == 0)
    {
      while (fabs(right - temp_forw) > THRESH_UP && fabs(forw - temp_right) > THRESH_UP)
      {
        turnright();
        get_distance_all_sensors();
      }
    }
    movestatus = 1;
  }
}

void get_distance_all_sensors()
{
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  { // Loop through all the sensors.
    if (millis() >= pingTimer[i])
    { // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) //oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
        sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = MAX_DISTANCE;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }  //
  left = cm[0];
  forw = cm[1];
  right = cm[2];
  
  Serial.print("Left: ");
  Serial.print(left);
  Serial.print(" Forw: ");
  Serial.print(forw);
  Serial.print(" Right: ");
  Serial.println(right);
}

void goforward()
{
  Serial.println("###########GOING FORWARD ###########");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN1, 90);
  analogWrite(EN2, 100);
}

void turnleft()
{
  Serial.println("###########LEFT TURN ###########");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void turnright()
{
  Serial.println("###########RIGHT TURN ###########");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}
void correctright()
{
  Serial.println("********CORRECTING TOWARDS RIGHT***********");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(EN1, 100);
  analogWrite(EN2, 105);
}

void correctleft()
{
  Serial.println("%%%%%%%%CORRECTING TOWARDS LEFT%%%%%%%%%%");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN1, 120);
  analogWrite(EN2, 80);
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
