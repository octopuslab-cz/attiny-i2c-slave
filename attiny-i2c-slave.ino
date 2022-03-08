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

// ************************
// ** I2C Slave routines **
// ************************

byte i2c_register;
byte i2c_systemreg;

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

// I2C Write
void receiveEvent(int howMany) {
  i2c_register = Wire.read();
  i2c_systemreg = Wire.read();

  switch (i2c_register) {
    case 0x01:
      handle_system(howMany - 2);
      break;

    case 0x05:
      handle_strip(howMany - 2);
      break;
  }

  // Flush extra buffer
  while (Wire.available()) {
    char c = Wire.read();
  }
}

// *********************************
// ** Handle system routines 0x01 **
// *********************************

void handle_system(int howMany) {
}

// **************************
// ** WS Led routines 0x05 **
// **************************

void handle_strip(int howMany) {
  // we have read request
  if (howMany == 0) {
    return;
  }

  switch (i2c_systemreg) {
    case 0x03:
      set_num_leds();
      break;

    case 0x04:
      set_brightness();
      break;

    case 0x05:
      set_led(howMany);
      break;

    case 0x06:
      set_rainbow();
      break;

    case 0x08:
      fill_led();
      break;
  }
}

// 0x03
void set_num_leds() {
  byte len = Wire.read();
  strip.updateLength(len);
}

// 0x04
void set_brightness() {
  byte bright = Wire.read();

  strip.setBrightness(bright);
  strip.show();
}

// 0x05
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

// 0x06
void set_rainbow() {
  byte hi = Wire.read();
  byte lo = Wire.read();

  uint16_t hue =  hi << 8 | lo;

  strip.rainbow(hue);
  strip.show();
}

// 0x08
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

void loop() {

}
