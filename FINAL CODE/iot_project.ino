#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#define PulseWire A0       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <SoftwareSerial.h>
#include <stdlib.h>
SoftwareSerial nodemcu(1,2); //rx, tx pins

//  Variables



//#define temppin A2
int Vo;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"





String cdata; // complete data, consisting of sensors values
int sdata1 = 0; // temperature centigrade
int sdata2 = 0; // temperature Farenheit

char buff[10];
String tempc; 
String tempf;
void setup() {   

  Serial.begin(9600);          // For Serial Monitor
  nodemcu.begin(9600);
   
  // set up the LCD's number of columns and rows: 
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  

  }
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
  if (pulseSensor.sawStartOfBeat()) {// Constantly test to see if "a beat happened".   
  Serial.println(myBPM); 
    }
Vo = analogRead(A2);
R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
logR2 = log(R2);
T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
float cel = T - 273.15; //convert Kelvin to Celcius
float fah = (cel * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit
tempc = dtostrf(cel, 3, 2, buff);
  tempf = dtostrf(fah, 3, 2, buff);
  cdata = cdata + tempc+","+tempf +","+myBPM; // comma will be used a delimeter
  Serial.println(cdata); 
  nodemcu.println(cdata);
  cdata = "";
}
