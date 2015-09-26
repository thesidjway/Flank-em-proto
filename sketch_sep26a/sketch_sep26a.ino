#define IN1 6
#define IN2 4
#define IN4 2
#define IN3 7
#define EN1 3
#define EN2 5

void setup() {
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);
pinMode(EN1,OUTPUT);
pinMode(EN2,OUTPUT);
}

void loop() {
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
digitalWrite(EN1,HIGH);
digitalWrite(EN2,HIGH);
}
