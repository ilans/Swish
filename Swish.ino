#include <Adafruit_NeoPixel.h>

#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <avr/pgmspace.h>

#define PIN 8

std::basic_oserialstream<char, std::char_traits<char>, Serial_> cout(Serial);

using namespace std;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

class Trail{
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint32_t len;
  unsigned long start_time =0;
  float speed;
  int dir;
};

vector<Trail> trails;
uint32_t pix = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'

  for(int i=0; i<2; i++) {
    Trail t;
    trails.push_back(t);
  }

  trails[0].r = 173;
  trails[0].g = 255;
  trails[0].b = 47;
  trails[0].len = 30;
  trails[0].speed = 0.125;
  trails[0].dir = 1;

  trails[1].r = 255;
  trails[1].g = 20;
  trails[1].b = 147;
  trails[1].len = 30;
  trails[1].speed = 0.125;
  trails[1].dir = -1;
}

void loop() {
//  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  // Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127,   0,   0), 50); // Red
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

// rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
  
  // ilan(75,0,130, 30, 8); // indigo
  // ilan(173,255,47, 30, 8); // green yellow
  // ilan(255,69,0, 30, 8); // orange
  // ilan(255,20,147, 30, 8); // deep pink
  // ilan(119,136,153, 30, 8); // light slate gray

  // ilan(75,0,130, 30, 8, -1); // indigo

  // red += 3;
  // green += 1;
  // blue -= 3; 

  // ilan(wrap255(red),wrap255(green),wrap255(blue), 25, 8); // indigo

  // if(pix >= strip.numPixels()+30) pix=0;
  // for(int i=0; i<trails.size(); i++){ advTail(i); }
  // pix++;
}

float wrap255(int val){
  if(val>255) val %= 255;
  if(val>0) val += 255;
  return val;
}

// Fill the dots one after the other with a color
void advTail(int idx) {
  if(trails[idx].start_time == 0){
    trails[idx].start_time = millis();
  }

  if(millis()-trails[idx].start_time >= (unsigned long)(1.0/trails[idx].speed)){
    float f_r = trails[idx].r/float(trails[idx].len);
    float f_g = trails[idx].g/float(trails[idx].len);
    float f_b = trails[idx].b/float(trails[idx].len);
    // for(uint16_t i=0; i<strip.numPixels()+trails[idx].len; i++) {
        for(int j=0; j<=trails[idx].len; j++) {
            if(trails[idx].dir==1){
              strip.setPixelColor(pix-j, strip.Color(trails[idx].r-j*f_r, trails[idx].g-j*f_g, trails[idx].b-j*f_b));
            } else {
              strip.setPixelColor(strip.numPixels()-1-(pix-j), strip.Color(trails[idx].r-j*f_r, trails[idx].g-j*f_g, trails[idx].b-j*f_b));
            }
        }
        strip.show();
    // }
  }
}

// Fill the dots one after the other with a color
void ilan(float r, float g, float b, uint32_t len, uint8_t wait, int dir) {
  float f_r = r/float(len);
  float f_g = g/float(len);
  float f_b = b/float(len);
  for(uint16_t i=0; i<strip.numPixels()+len; i++) {
      for(int j=0; j<=len; j++) {
          if(dir==1){
            strip.setPixelColor(i-j, strip.Color(r-j*f_r, g-j*f_g, b-j*f_b));
          } else {
            strip.setPixelColor(strip.numPixels()-1-(i-j), strip.Color(r-j*f_r, g-j*f_g, b-j*f_b));
          }
      }
      strip.show();
      delay(wait);
  }
}