#include <ThingSpeak.h>
#include <WiFi.h>
#include "HiddenInfo.h"
#include <Scaler.h>
#include <LowPassFilter.h>
#include <PIC.h>

//Setting up Variables with values
//float Kp = 1.5;
//float Ti = 15;
float Kp = 1.23;
float Ti = 11.6;
float dt = 0.1;
float tf = 2;
float ts = 0.1;

//Setting up objects
PIC pi(Kp, Ti, dt);
Scaler scaler;
LowPassFilter lpfI(ts, tf);

//Setting up pins
int temperaturePin = A0;
int controllerPin = 5;

//Setting up variables without values
float setpoint;
float temperature;
float inputScaled;
int input;
int controlSignal;
float filteredInput;
float result;

//Setting up wifi
char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;
WiFiClient  client;

//Setting up variables for ThingSpeak
unsigned long ChannelNumberAirHeater = SECRET_CH_ID_AIRHEATER;
unsigned long ChannelNumberController = SECRET_CH_ID_CONTROLLER;
const char * WriteAPIKeyAirHeater = SECRET_WRITE_APIKEY_AIRHEATER;
const char * ReadAPIKeyController = SECRET_READ_APIKEY_CONTROLLER;
unsigned int temperatureField = 1;
unsigned int kpField = 1;
unsigned int tiField = 2;
unsigned int setPointField = 3;
int statuscode;
int writeTimer;
int readTimer;
int nextWriteTime = 15000;


void setup() {
  Serial.begin(9600);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  writeTimer = 0;
  readTimer = 30000;
  
}

void loop() {
  
  //Make sure wifi is connected and if not reconnect
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(1000);     
    } 
    Serial.println("\nConnected.");
  }

  if(readTimer >= 30000)
  {
    setpoint = ReadThingSpeak(setPointField, setpoint);
    Kp = ReadThingSpeak(kpField, Kp);
    Ti = ReadThingSpeak(tiField, Ti);
    pi.SetParams(Kp, Ti);
    readTimer = 0;
  }
  
  UpdateControlSignal();
  
  analogWrite(controllerPin, controlSignal);
  
  delay(100);
  writeTimer += 100;
  readTimer += 100;

  WriteThingSpeak();
  
  
}

float ReadThingSpeak(int field, float currentValue)
{
    result = ThingSpeak.readFloatField(ChannelNumberController, field, ReadAPIKeyController);
    int statuscode = ThingSpeak.getLastReadStatus();
    if(statuscode == 200)
    {
    return result;
    }
    else
    {
      Serial.println("Problem reading channel. HTTP error code " + String(statuscode));
      return currentValue;
    }
}

void UpdateControlSignal()
{
    //Update control signal
  input = analogRead(temperaturePin);
  inputScaled = scaler.ScaleAnalogInput(input);
  inputScaled = lpfI.FilteredValue(inputScaled);
  temperature = scaler.VoltToCelsius(inputScaled);
  controlSignal = int(pi.ControlSignal(setpoint, temperature)*51);
}
void WriteThingSpeak()
{
  //Write only every approx 15s
  if(writeTimer >= nextWriteTime)
  {
    statuscode = ThingSpeak.writeField(ChannelNumberAirHeater, temperatureField, temperature, WriteAPIKeyAirHeater);
    if(statuscode == 200){
      Serial.println("Channel update successful.");
      writeTimer = 0;
      nextWriteTime = 15000;
     }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(statuscode));
      writeTimer = 0;
      nextWriteTime = 1000; //Try again approx every second
    }
    
  }
}
