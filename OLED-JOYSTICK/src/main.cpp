#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define JOY_V 35
#define JOY_H 34
int joyVer;
int joyHor;

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("Display started"));
  }
  display.display();
  delay(5000);
  display.clearDisplay();
  // Joystick
  pinMode(JOY_V, INPUT_PULLUP);
  pinMode(JOY_H, INPUT_PULLUP);
}

void loop(){
  // Read joystick values
  joyVer = analogRead(JOY_V);
  joyHor = analogRead(JOY_H);

  Serial.printf("VER: %d | HOR: %d\n",joyVer, joyHor);

  // Display
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("xSeejx Code");
  display.display();
}