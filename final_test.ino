#include <SoftwareSerial.h>
SoftwareSerial bluetooth(0, 1); //RX, TX

// Temperature sensor pins
float tempC; 
float millivolts;
int green = 4;
int red = 3;
int tempPin = A2;

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;
long duration;
float distance;
int safetyDistance;
 
// Smoke sensor pins
int smoke_detector = A0;
int safety_lim = 1000; //Sets  smoke density safe limit


void setup() 
{
  Serial.begin(115200); 
  bluetooth.begin(115200); 

  // Temp part
  pinMode(tempPin,INPUT);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  analogReference(INTERNAL); 

  // Distance part
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Smoke part
  pinMode(buzzer,  OUTPUT);
  pinMode(smoke_detector, INPUT);
}

void loop() 
{
  //Temp part
  digitalWrite(green, HIGH);
  millivolts = analogRead(tempPin);
  tempC = (millivolts*50) / 1023;
  if(tempC < 15)
  {
    String message = (String) "High Temperature Level is detected by " + millivolts;
    Serial.println(message); // Prints the distance on the Serial Monitor
    //bluetooth.println(message); // Prints the distance on the connected device
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(500);
    digitalWrite(red, LOW);
    delay(500);
    
  }
  else
  {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Ultrasunic part
  digitalWrite(trigPin, LOW);
  delay(50); 
  // Sets the trigPin on HIGH state for 50 milliseconds
  digitalWrite(trigPin, HIGH);
  delay(50); 
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds

  distance= duration*0.034/2; // Calculating the distance
  
  safetyDistance = distance;
  if (safetyDistance <= 20)   
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledPin, HIGH);
    //Serial.println(distance);
    String message = (String) "Close Body is detected by " + distance;
    Serial.println(message); 
    //bluetooth.println(message);
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(ledPin, LOW);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Smoke part
  int sensor_read = analogRead(smoke_detector); 
  //Serial.println(sensor_read);
  if (sensor_read > safety_lim)
  //Checks if reading is beyond safety limit
  { 
    digitalWrite(buzzer, HIGH);
    String message = (String) "High Smoke Level is detected by " + sensor_read;
    Serial.println(message);
    //bluetooth.println(message);
  }
    
  delay(100);  // Add a small delay for stability
}

