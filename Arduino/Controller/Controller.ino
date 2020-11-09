#include <Scaler.h>
#include <LowPassFilter.h>
#include <PIC.h>

//float Kp = 1.5;
//float Ti = 15;
float Kp = 1.23;
float Ti = 11.6;
float dt = 0.1;
float tf = 2;
float ts = 0.1;

PIC pi(Kp, Ti, dt);
Scaler scaler;
LowPassFilter lpfC(ts, tf);
LowPassFilter lpfI(ts, tf);

int temperaturePin = A0;
int controllerPin = 5;

float setpoint;
float temperature;
float inputScaled;
int input;
int controlSignal;
float filteredInput;

void setup() {
  Serial.begin(9600);

  setpoint = 25;
  
}

void loop() {
  input = analogRead(temperaturePin);
  inputScaled = scaler.ScaleAnalogInput(input);
  inputScaled = lpfI.FilteredValue(inputScaled);
  temperature = scaler.VoltToCelsius(inputScaled);
  controlSignal = int(pi.ControlSignal(setpoint, temperature)*51);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("\t scaled input: ");
  Serial.print(inputScaled);
  Serial.print("\t Control Signal: ");
  Serial.print(controlSignal);
  Serial.print("\n");

  
  analogWrite(controllerPin, controlSignal);

  delay(100);







//  Serial.print("Control Signal: ");
//  Serial.print(pi.ControlSignal(setpoint, temperature));
//  Serial.print("V\tSP: ");
//  Serial.print(setpoint);
//  Serial.print("*C\tTemp: ");
//  Serial.print(temperature);
//  Serial.print("*C\n");
//  delay(500);
//  temperature += 0.1;

}
