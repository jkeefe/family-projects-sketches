/************************************************************
ESP8266_Shield_Demo.h
SparkFun ESP8266 AT library - Demo
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates the basics of the SparkFun ESP8266
AT library. It'll show you how to connect to a WiFi network,
get an IP address, connect over TCP to a server (as a client),
and set up a TCP server of our own.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is released under the MIT license.

Distributed as-is; no warranty is given.

---

Modified by John Keefe May 2016
To check OpenWeatherMap for rain forcast in next 12-15 hrs

  If LED in Pin 13 & ground ...
  Quick "heartbeat" flashes during check,
  Steady = No rain in next 12-15 hrs
  Flashing = Bring an umbrella!
  
  Reset or turn off/on to check again

************************************************************/

//////////////////////
// Library Includes //
//////////////////////
// SoftwareSerial is required (even you don't intend on
// using it).
#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>

///////////////////////
// Weather Variables //
///////////////////////
const String apikey = "YourAPIKeyGoesHere";
const String latitude = "YourLatitudeGoesHere";
const String longitude = "YourLongitudeGoesHere";

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
const char mySSID[] = "YourWifiNetworkNameGoesHere";
const char myPSK[] = "YourWifiPasswordGoesHere";

////////////////////////
// Forecast Variables //
////////////////////////
boolean itWillRain = false;

//////////////////
// HTTP Strings //
//////////////////
const char destServer[] = "api.openweathermap.org";
const String httpRequest = "GET /data/2.5/forecast?lat=" + latitude + "&lon=" + longitude + 
                           "&APPID=" + apikey + " HTTP/1.1\n"
                           "Host: api.openweathermap.org\n"
                           "Connection: close\n\n";
   
// All functions called from setup() are defined below the
// loop() function. They modularized to make it easier to
// copy/paste into sketches of your own.
void setup() 
{
  // Initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(9600);

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();

  // displayConnectInfo prints the Shield's local IP
  // and the network it's connected to.
  displayConnectInfo();

  // go check the forecast!
  delay(100);
  clientDemo();
  
  // adding an extra blank line for clarity
  Serial.println();
  
  // finally, let us know if it's going to rain in the next 12-15 hours.
  // note that because itWillRain is a true/false (or boolean) variable,
  if (itWillRain == true) {
    Serial.println("Better bring an umbrella: Rain forecast in the next 12-15 hours.");
    // flash a rain warning!
    for (int i = 0; i <  20; i++){
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);              // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);              // wait for a second
    }
  } else {
    Serial.println("No rain in the forecast for the next 12-15 hours.");
    // give a steady all-clear
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(20000);             // wait for 20 seconds
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  }
  
}

void loop() 
{
  // nothing here
}

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin();
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(test);
  }
  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
    }
  }
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
    Serial.print(F("Connected to: "));
    Serial.println(connectedSSID);
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void clientDemo()
{
  // To use the ESP8266 as a TCP client, use the 
  // ESP8266Client class. First, create an object:
  ESP8266Client client;

  // ESP8266Client connect([server], [port]) is used to 
  // connect to a server (const char * or IPAddress) on
  // a specified port.
  // Returns: 1 on success, 2 on already connected,
  // negative on fail (-1=TIMEOUT, -3=FAIL).
  int retVal = client.connect(destServer, 80);
  if (retVal <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }


  // These are variables we're going to use below. Read on!
  int numberOfFinds = 0;

  // print and write can be used to send data to a connected
  // client connection. Here, it sends the "request," which
  // is the information we're asking for.
  client.print(httpRequest);

  // Here's where we read the data from the website.
  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available()) {

    // Is it raining in the next 12-15 hours? Let's find out.    

    // Openweathermap.org sends the 5-day forecast in a stream of data
    // formatted as seen here: http://openweathermap.org/forecast5#JSON .
    // Here's a full sample: http://keefe.cc/sensing-internet/sample-data.json
    
    // In that stream, there are 40 forecasts, one for every 3 hours 
    // over the next 5 days. So for the next 12-15 hours, we want to check
    // the first five forecasts. 
    
    // Each forecast has an "id" that indicates
    // the type of weather forecast. See chart here:
    // http://openweathermap.org/weather-conditions
    // From the chart, we see that if the "id" starts with a 3 or a 5,
    // it's going to rain (or at least drizzle). The data looks like this:
    //      ... "weather":[{"id":500,"main": ... 

    // Using client.find("<something to find>"), we'll look for
    // the first five appearanes of  "id": ... with the quotes and the colon
    // ...  and then read the very next character to see if it's a 3 or a 5.

    // There's actually one other "id": at the beginning of the stream that is
    // used to identify the city, so we'll ignore the first case, using cases 2 through 6. 

    // Also, when using the client.find() command, we have to wrap the
    // search part in double quotes, like client.find("my search"). But the 
    // thing we're searching for actually *includes* double quotes: "id":  
    // ... and client.find(""id"":) will totally confuse the system.
    // So I use the "hex" value for a double quote, 22, and
    // represent each of them in Arduino-speak as \x22.
    // so "id": becomes \x22id\x22:

    // look for "id": in the stream of data, written as \x22id\x22:
    if ( client.find( "\x22id\x22:" ) ) {

      // found one! add to the number
      numberOfFinds += 1;
      Serial.print("id number ");
      Serial.print(numberOfFinds);
      Serial.print(" ... ");

      // flicker the LED on the first find and every 10 finds 
      // so we know it's working
      if (numberOfFinds == 1 || numberOfFinds % 10 == 0) {
        digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(2);                // wait for a 10th of a second
        digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      }
      
      // remember, we only want the 2nd through 6th id.
      // so looking for the ones greater than 1 and less
      // than 7. The && means "and."
      if ( numberOfFinds > 1 && numberOfFinds < 7) {

        // read the next character
        char nextCharacter = client.read();
        
        // and convert it into a number
        int nextNumber = (int)nextCharacter - 48;

        Serial.print("next number is ");
        Serial.print(nextNumber);
        Serial.print(" ... ");

        // is it a 3 or a 5?
        // (the double bars || mean "or)
        if (nextNumber == 3 || nextNumber == 5) {
          // found a 3 or a 5
          itWillRain = true;
          Serial.println("Rain forecast!");
        } else {
          // not a 3 or a 5
          Serial.println("No rain!");
        }
   
      } else {
        // number of finds wasn't greater than 1 and less than 7
        Serial.println("skipping!");       
      }
      
    }
    
  }

  // This code runs when the connection to the website is done.
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected()) {
    client.stop(); // stop() closes a TCP connection.
  }
}


// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}
