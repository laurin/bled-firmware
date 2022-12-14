#include "ledcontroller.hpp"
#include "FastLED.h"
#include "consts.hpp"

LEDController::LEDController()
{
  FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
}

void LEDController::set(uint8_t r, uint8_t g, uint8_t b)
{
  this->r = r;
  this->g = g;
  this->b = b;
  fill_solid(this->leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
}

void LEDController::test()
{
  this->set(255, 0, 0);
  delay(200);
  this->set(0, 255, 0);
  delay(200);
  this->set(0, 0, 255);
  delay(200);
  this->set(0, 0, 0);
}