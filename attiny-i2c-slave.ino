#include <Wire.h>

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN  PB3

SoftwareSerial SwSerial(-1, PB1);
Adafruit_NeoPixel strip(45, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  delay(100);
  SwSerial.begin(115200);
  SwSerial.println("");
  SwSerial.println("AT Tiny booting up");
    
  Wire.begin(0x14);
  Wire.onReceive(receiveEvent);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)

  strip.rainbow(10000);
  strip.show();
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

void set_rainbow() {
  byte hi = Wire.read();
  byte lo = Wire.read();

  uint16_t hue =  hi << 8 | lo;

  strip.rainbow(hue);
  strip.show();
}


void receiveEvent(int howMany) {

  switch ( Wire.read() ) {
    case 0x05:
      set_led(howMany-1);
      break;

    case 0x06:
      set_rainbow();
      break;
  }

  while (Wire.available()) {
    char c = Wire.read();
    SwSerial.write(c);
  }
}

void loop() {
  //rainbow(10);
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      for (int c = b; c < strip.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
