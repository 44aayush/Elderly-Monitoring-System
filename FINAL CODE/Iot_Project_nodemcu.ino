#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <Blynk.h>

WidgetLCD lcd(V5);


char auth[] = " J1SE4lwbjoKPej6lM3ktEnArz3VBkfLo";



// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

String myString; // complete message from arduino, which consists of snesors data
char rdata; // received characters


String firstVal, secondVal,thirdVal; // sensors 
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L,sensorvalue1); 

}

void loop()
{
   if (Serial.available() == 0 ) 
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
   // Serial.print(rdata);
    if( rdata == '\n')
    {

String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2);
 


firstVal = l;
secondVal = m;
thirdVal = n;

  myString = "";
// end new code
    }
  }

}


void sensorvalue1()
{

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, firstVal);
  Blynk.virtualWrite(V4, secondVal);
 

      lcd.clear();
    lcd.print(0,0,"BPM:");
    lcd.print(11,0,thirdVal);
     lcd.print(0,1,"Health Monitor");

}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return (found > index)? data.substring(strIndex[0], strIndex[1]) : "";
}
   
