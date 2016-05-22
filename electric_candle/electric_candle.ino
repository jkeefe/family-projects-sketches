/* Modern Device Wind Sensor Sketch for Rev C Wind Sensor 
 * 
 *  
 Hardware Setup: 
 Wind Sensor Signals    Arduino
 GND                    GND
 +V                     5V
 RV                     A1    // modify the definitions below to use other pins
 TMP                    A0    // modify the definitions below to use other pins
 
 Paul Badger 2014
 Original at https://github.com/moderndevice/Wind_Sensor
 Licensed for use on official Modern Device hardware
 
 Revised by John Keefe 2016
 
 Hardware setup:
 Wind Sensor is powered from a regulated five volt source.
 RV pin and TMP pin are connected to analog inputs.
 
 */

#define analogPinForRV    1   // change to pins you the analog pins are using
#define analogPinForTMP   0

const float zeroWindAdjustment =  .2; 
int TMP_Therm_ADunits;  
float RV_Wind_ADunits;   
float RV_Wind_Volts;
unsigned long lastMillis;
int TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_MPH;

int led = 13;              // candle LED 
const int buttonPin = 2;   // the pushbutton pin
int buttonState = 0;       // variable for reading the pushbutton status

void setup() {
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    
  
  // turn LED on
  digitalWrite(led, HIGH);

}

void loop() {
  
  buttonState = digitalRead(buttonPin);

  if (millis() - lastMillis > 200){   
    
    TMP_Therm_ADunits = analogRead(analogPinForTMP);
    RV_Wind_ADunits = analogRead(analogPinForRV);
    RV_Wind_Volts = (RV_Wind_ADunits *  0.0048828125);

    // these are all derived from regressions from raw data as such they depend on a lot of experimental factors
    // such as accuracy of temp sensors, and voltage at the actual wind sensor, (wire losses) which were unaccouted for.
    TempCtimes100 = (0.005 *((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits)) - (16.862 * (float)TMP_Therm_ADunits) + 9075.4;  

    zeroWind_ADunits = -0.0006*((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits) + 1.0727 * (float)TMP_Therm_ADunits + 47.172;  //  13.0C  553  482.39

    zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;  
    
   WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) /.2300) , 2.7265);   
    
    if (WindSpeed_MPH > 6) {
      douseCandle();
    }
    
    if (buttonState == HIGH) {
      lightCandle();
    }
    
    lastMillis = millis();    
  } 

}

void douseCandle() {

  // turn LED off
  digitalWrite(led, LOW);
  
}

void lightCandle() {

  digitalWrite(led, HIGH);
  
}