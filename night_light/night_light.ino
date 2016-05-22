/*
 
 Night Light
 Lights an LED when the room is dark.

 By John Keefe, January 2016
 http://keefe.cc/night-light

 Based on example code created by David Cuartielles
 modified 30 Aug 2011 by Tom Igoe
 http://www.arduino.cc/en/Tutorial/AnalogInput

 This code is in the public domain.
 
 */

int sensorPin = A0;   // select the input pin for the photoresistor
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value from the sensor
int darkPoint = 0;    // if the sensor value is less than this, the room is "dark"

// This code runs once when Arduino is turned on or reset
void setup() {
  
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

  // this is needed to use the Serial Monitor
  Serial.begin(9600);
  
}

// This code loops forever
void loop() {
  
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  
  // if the sensorValue is less than the darkPoint
  // power the LED:
  if (sensorValue < darkPoint) {
    digitalWrite(ledPin, HIGH);
  }

  // if the sensorValue is greater than or equal to the
  // darkPoint, don't power the LED:
  if (sensorValue >= darkPoint) {
    digitalWrite(ledPin, LOW);
  }

  // output the sensor value to the Serial Monitor
  Serial.println(sensorValue);

  // add a little delay to help read the numbers!
  delay(100); 
}