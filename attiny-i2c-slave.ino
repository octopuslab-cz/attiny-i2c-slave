#define FW_MAJOR 0x02
#define FW_MINOR 0x01
#define FW_PATCH 0x01

#define I2C_SLAVE_ADDR 0x14

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN PIN_PB3
Adafruit_NeoPixel strip(45, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  strip.begin();
  strip.show();
  strip.setBrightness(5);

  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 255, 0));
  strip.setPixelColor(2, strip.Color(0, 0, 255));
  strip.setPixelColor(3, strip.Color(255, 255, 255));
  strip.show();
}

// ************************
// ** I2C Slave routines **
// ************************

byte i2c_register;
byte i2c_systemreg;

// I2C Read
void requestEvent() {
  switch (i2c_systemreg) {
    case 0x01:
      Wire.write(FW_MAJOR);
      Wire.write(FW_MINOR);
      Wire.write(FW_PATCH);
      break;

    case 0x02:
      Wire.write(0x20);
      break;
  }
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

    case 0x09:
      fill_led_every_x();
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


// 0x09
void fill_led_every_x() {
  byte from = Wire.read();
  byte count = Wire.read();
  byte skipled = Wire.read();
  byte r = Wire.read();
  byte g = Wire.read();
  byte b = Wire.read();
  uint32_t color = strip.Color(r, g, b);

  for (int i = 0; i < count; i+=skipled+1) {
    strip.setPixelColor(i, color);
  }

  strip.show();
}


void loop() {

}
