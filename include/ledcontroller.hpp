#pragma once

#include "FastLED.h"
#include "consts.hpp"

class LEDController
{
private:
  CRGB leds[NUM_LEDS];

public:
  LEDController();
  void set(uint8_t r, uint8_t g, uint8_t b);
};
