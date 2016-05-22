// Based on an example sketch originally written by Paul Badger 
// and now maintained by Paul Stoffregen
// https://github.com/PaulStoffregen/CapacitiveSensor
// Modified by John Keefe May 2016

#include <CapacitiveSensor.h>

CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);   

int LED = 13;
long sensor_reading;

void setup()                    
{
   pinMode(LED, OUTPUT);
   Serial.begin(9600);
}

void loop()                    
{
    sensor_reading =  cs_4_6.capacitiveSensor(30);
    
    Serial.println(sensor_reading);
    
    if (sensor_reading > 100) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }

    delay(10);                             
}