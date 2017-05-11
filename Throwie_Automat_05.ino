#include <Adafruit_NeoPixel.h>

#define PIN 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(55, PIN, NEO_GRB + NEO_KHZ800);

int Switch;
int SwitchPin = 2;

void setup() {
  pinMode(SwitchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SwitchPin), flash, CHANGE);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  pulsieren();

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);

  }
  strip.show();
  delay(wait);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delayMicroseconds(1000*wait);
    //delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delayMicroseconds(1000*wait);
    //delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();
      delayMicroseconds(1000*wait);
      //delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      //delay(wait);
      delayMicroseconds(1000*wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void pulsieren() {
  for (int j = 0; j < 255; j = j + 10) {
    for (int i = 0; i < 255; i++) {
      colorWipe(strip.Color(j, i, i), 5);
    }
    for (int i = 255; i > 0; i--) {
      colorWipe(strip.Color(j, i, i), 5);
    }
  }
  for (int j = 0; j < 255; j = j + 10) {
    for (int i = 0; i < 255; i++) {
      colorWipe(strip.Color(i, i, j), 5);
    }
    for (int i = 255; i > 0; i--) {
      colorWipe(strip.Color(i, i, j), 5);
    }
  }
  for (int j = 0; j < 255; j = j + 10) {
    for (int i = 0; i < 255; i++) {
      colorWipe(strip.Color(i, j, i), 5);
    }
    for (int i = 255; i > 0; i--) {
      colorWipe(strip.Color(i, j, i), 5);
    }
  }
}

void flash() {
  detachInterrupt(digitalPinToInterrupt(SwitchPin));
  for (int i = 0; i < 10; i++) {
    colorWipe(strip.Color(0, 0, 255), 20);
    colorWipe(strip.Color(0, 0, 0), 20);
    colorWipe(strip.Color(0, 0, 255), 20);
    colorWipe(strip.Color(0, 0, 0), 20);
  }
  //theaterChaseRainbow(0);
  for (int i = 0; i < 255; i++) {
    colorWipe(strip.Color(255, i, i), 0);
  }
  for (int i = 0; i < 255; i++) {
    colorWipe(strip.Color(i, 255, i), 0);
  }
  for (int i = 0; i < 255; i++) {
    colorWipe(strip.Color(i, i, 255), 0);
  }
  colorWipe(strip.Color(0, 0, 0), 0);
  attachInterrupt(digitalPinToInterrupt(SwitchPin), flash, CHANGE);
}


