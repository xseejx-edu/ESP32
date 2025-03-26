#include <Arduino.h>

const int PIR_SENSOR_OUTPUT_PIN = 14;  // PIR sensor O/P pin (DIGITAL)
int warm_up;

void setup() {
  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Waiting For Power On Warm Up");
  delay(20000); // Power On Warm Up Delay
  Serial.println("Ready!");
}

void loop() {
  int sensor_output;
  sensor_output = digitalRead(PIR_SENSOR_OUTPUT_PIN);
  if( sensor_output == LOW ){
    if( warm_up == 1 ){
      Serial.println("Warming Up");
      warm_up = 0;
      delay(2000);
    }
    Serial.println("No object in sight");
    delay(1000);
  }else{
    Serial.println("Object detected");   
    warm_up = 1;
    delay(1000);
  } 
}