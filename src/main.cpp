#include <Arduino.h>

#include "ledcontroller.hpp"

LEDController *leds = new LEDController();

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  leds->set(255, 0, 0);
  delay(500);
  leds->set(0, 255, 0);
  delay(500);
  leds->set(0, 0, 255);
  delay(500);
}