#include <Arduino.h>

#define MIC_PIN 34
#define SAMPLE_RATE 44100
#define NUM_SAMPLES 44100 // 1 second of audio at 44.1kHz

void setup() {
  Serial.begin(115200);
}

void loop() {
  /*int16_t audioData[NUM_SAMPLES];
  for (int i = 0; i < NUM_SAMPLES; i++) {
    audioData[i] = analogRead(MIC_PIN) - 512; // Center the signal
  }

  // Send audio data to serial
  for (int i = 0; i < NUM_SAMPLES; i++) {
    Serial.write((uint8_t)(audioData[i] & 0xFF));
    Serial.write((uint8_t)((audioData[i] >> 8) & 0xFF));
  }

  delay(1000); // Wait for 1 second before recording again*/

}
