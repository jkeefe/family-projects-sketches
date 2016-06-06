/************************************************************
This is a modified version of ESP8266_Phant.ino
SparkFun ESP8266 AT library - Phant Posting Example
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates how to use the TCP client 
functionality of the SparkFun ESP8266 WiFi library to post 
sensor readings to a Phant stream on 
https://data.sparkfun.com

This sketch is set up to post to a publicly available stream
https://data.sparkfun.com/streams/DJjNowwjgxFR9ogvr45Q
Please don't abuse it! But feel free to post a few times to
verify the sketch works. If it fails, check the HTTP response
to make sure the post rate hasn't been exceeded.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.

---

Modified for a temperature sensor
by John Keefe
June 2016

************************************************************/
// The SparkFunESP8266WiFi library uses SoftwareSerial
// to communicate with the ESP8266 module. Include that
// library first:
#include <SoftwareSerial.h>
// Include the ESP8266 AT library:
#include <SparkFunESP8266WiFi.h>

#define SENSORPIN 0

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// const char mySSID[] = "YourWifiNetworkNameGoesHere";
// const char myPSK[] = "YourWifiPasswordGoesHere";

// const char mySSID[] = "WNYC Data News Jetpack";
// const char myPSK[] = "bena-spa-ti";

const char mySSID[] = "The Bigger Dipper";
const char myPSK[] = "mooshroom-hat-animal-thingy";


////////////////////////
// Sparkfun Data Keys //
////////////////////////
// const String publicKey = "YourPublicKeyHere";
// const String privateKey = "YourPrivateKeyHere";

const String publicKey = "6JbY0y2lNJsgvgDw2nAx";
const String privateKey = "Wwv7za91Pwt4v4oMWmz5";

// Phant detsination server:
const String phantServer = "data.sparkfun.com";
String httpHeader = "POST /input/" + publicKey + ".txt HTTP/1.1\n" +
                    "Host: " + phantServer + "\n" +
                    "Phant-Private-Key: " + privateKey + "\n" +
                    "Connection: close\n" + 
                    "Content-Type: application/x-www-form-urlencoded\n";

///////////////////
// Sensor Values //
///////////////////
int waitMinutes = 2;
float sensor_value;
float voltage;
float temp;

void setup() 
{
  int status;
  Serial.begin(9600);
  
  // To turn the MG2639 shield on, and verify communication
  // always begin a sketch by calling cell.begin().
  status = esp8266.begin();
  if (status <= 0)
  {
    Serial.println(F("Unable to communicate with shield. Looping forever."));
    while(1) ;
  }
  
  esp8266.setMode(ESP8266_MODE_STA); // Set WiFi mode to station
  if (esp8266.status() <= 0) // If we're not already connected
  {
    if (esp8266.connect(mySSID, myPSK) < 0)
    {
      Serial.println(F("Error connecting"));
      while (1) ;
    }    
  }
  
  // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(esp8266.localIP());

  Serial.println(F("Waiting one minute to get started ..."));
  delay(60000);
  
}

void loop()
{

  Serial.println(F("Latest reading ..."));
  sensor_value = analogRead(SENSORPIN); 
  Serial.print(F("Sensor Value: "));
  Serial.println(sensor_value);
  
  voltage = sensor_value * 5000 / 1024;
  Serial.print(F("Voltage (in mV): "));
  Serial.println(voltage);
  
  temp = (voltage-500) / 10;
  Serial.print(F("Degrees Celsius: "));
  Serial.println(temp);

  postToPhant(temp);
  Serial.println(F("Done! See the current data posted online at:"));
  Serial.print  (F("      https://data.sparkfun.com/streams/"));
  Serial.println(publicKey);
  Serial.println(F("Waiting for next reading ..."));
  Serial.println();
  delay(waitMinutes * 60000);
  
}

void postToPhant(float temperature)
{
  // Create a client, and initiate a connection
  ESP8266Client client;
  
  if (client.connect(phantServer, 80) <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }
  Serial.println(F("Connected."));
  
  // Set up our Phant post parameters:
  String params;
  params += "temp=" + String(temperature);
  
  Serial.println(F("Posting to SparkFun!"));

  client.print(httpHeader);
  client.print("Content-Length: "); client.println(params.length());
  client.println();
  client.print(params);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char
  
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}
