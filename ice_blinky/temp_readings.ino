/*
 * Get readings from a simple TMP36 Thermistor
 * 
 * With flat side facing you, connect the legs to the Arduino like this:
 * - Leftmost leg to Arduino's 5V
 * - Middle leg to Arduino's A0
 * - Rightmost leg to Arduino GND
 * 
 * By John Keefe
 * johnkeefe.net
 * June 2016
 * 
 * This code is in the public domain
 */

#define SENSORPIN 0

float sensor_value;
float voltage;
float tempC;
float tempF;

void setup() {
  // Start the Serial Monitor stream
  Serial.begin(9600);
  pinMode(13, OUTPUT);  
}

void loop() {
  sensor_value = analogRead(SENSORPIN);
  Serial.print("Sensor Value: ");
  Serial.println(sensor_value);
  
  voltage = sensor_value * 5000 / 1024;
  Serial.print("Voltage (millivolts): ");
  Serial.println(voltage);
  
  tempC = (voltage-500) / 10;
  Serial.print("Degrees Celsius: ");
  Serial.println(tempC);

  tempF = (tempC * 9/5) + 32;
  Serial.print("Degrees Fahrenheit: ");
  Serial.println(tempF);
  Serial.println();
  
  delay(5000);

}