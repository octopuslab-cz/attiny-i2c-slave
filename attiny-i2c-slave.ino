#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN  PB3
Adafruit_NeoPixel strip(45, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Wire.begin(0x14);
  Wire.onReceive(receiveEvent);

  strip.begin();
  strip.show();
  strip.setBrightness(10);

  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
}

void set_num_leds() {
  byte len = Wire.read();
  strip.updateLength(len);
}


void set_led(int howMany) {
  for (int i = 0; i < howMany / 4; i++) {
    int led = Wire.read();
    byte r = Wire.read();
    byte g = Wire.read();
    byte b = Wire.read();
    uint32_t color = strip.Color(r, g, b);

    strip.setPixelColor(led, color);
  }
  strip.show();
}

void fill_led() {
  byte from = Wire.read();
  byte count = Wire.read();
  byte r = Wire.read();
  byte g = Wire.read();
  byte b = Wire.read();
  uint32_t color = strip.Color(r, g, b);

  strip.fill(color, from, count);
  strip.show();
}

void set_brightness() {
  byte bright = Wire.read();

  strip.setBrightness(bright);
  strip.show();
}

void set_rainbow() {
  byte hi = Wire.read();
  byte lo = Wire.read();

  uint16_t hue =  hi << 8 | lo;

  strip.rainbow(hue);
  strip.show();
}

void handle_strip(int howMany) {
  switch (Wire.read()) {
    case 0x03:
      set_num_leds();
      break;

    case 0x04:
      set_brightness();
      break;

    case 0x05:
      set_led(howMany - 1);
      break;

    case 0x06:
      set_rainbow();
      break;

    case 0x08:
      fill_led();
      break;
  }
}

void receiveEvent(int howMany) {
  if (howMany == 1) {
    byte reg = Wire.read();
    return;
  }

  switch (Wire.read()) {
    case 0x05:
      handle_strip(howMany - 1);
      break;
  }

  // Flush extra buffer
  while (Wire.available()) {
    char c = Wire.read();
  }
}

void loop() {

}
