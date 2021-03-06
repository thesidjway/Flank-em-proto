#define S0 8
#define S1 9
#define OE 10
#define S2 11
#define S3 12
#define OUT 13

#define REDTHRES 20
#define BLUETHRES 45
#define GREENTHRES 25
#define YELLOWTHRES 17

int red = 0;  
int green = 0; 
int blue = 0;  
void colour();
void setup()   
{  
  Serial.begin(9600); 
  pinMode(S0, OUTPUT);  
  pinMode(S1, OUTPUT);  
  pinMode(S2, OUTPUT);  
  pinMode(S3, OUTPUT);  
  pinMode(OUT, INPUT);  
  digitalWrite(S0, HIGH);  
  digitalWrite(S1, HIGH);  
}  
    
void loop() 
{  
  colour(); 
  Serial.print("R Intensity:");  
  Serial.print(red, DEC);  
  Serial.print(" G Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" B Intensity : ");  
  Serial.print(blue, DEC);  

  else if (green < red && green < blue && green<GREENTHRES) 
  Serial.println(" - (Green colour)");  
  else if(green<blue && red<blue && blue>YELLOWTHRES)
  Serial.println(" - (Yellow colour)");
  else if (red < blue && red < green && red<REDTHRES)
  Serial.println(" - (Red colour)");  
  else if (blue < red && blue < green && blue<BLUETHRES)   
  Serial.println(" - (Blue colour)"); 
  
  else
  Serial.println();  
  delay(300);    
 }  
    
void colour()  
{    
  digitalWrite(S2, LOW);  
  digitalWrite(S3, LOW);  
  red = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);  
  digitalWrite(S2,LOW);
  digitalWrite(S3, HIGH);  
  blue = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);  
  digitalWrite(S2,HIGH);
  digitalWrite(S3, HIGH);  
  green = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);  
}
