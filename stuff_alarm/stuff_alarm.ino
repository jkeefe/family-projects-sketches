/*
 Analog Input
 http://www.arduino.cc/en/Tutorial/AnalogInput

 Created by David Cuartielles
 modified 30 Aug 2011 by Tom Igoe
 modified March 2016 by John Keefe

 This example code is in the public domain.

 */

int sensorPin = A0;   // set the input pin for the sensor
int buzzerPin = 13;   // set the pin for the buzzer
int sensorValue = 0;  // variable to store the sensor value
int movedValue = 100; // threshold value to trigger the buzzer

void setup() {
  // declare the buzzerPin as an OUTPUT:
  pinMode(buzzerPin, OUTPUT);

  // start up the Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  
  // if the sensorValue is less than the movedValue,
  // turn the buzzer on (with HIGH power). 
  // Otherwise turn it off (with LOW power).
  if (sensorValue < movedValue) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // print the sensor value to the Serial Monitor for calibration 
  Serial.println(sensorValue);
}