#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

void setup(){
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
  Serial.println(F("tak Boleh T-T"));

for(;;);
}

display.display();
delay(5000);
display.clearDisplay();

}

void loop(){

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("jadi lah kontol");
  display.display();
}