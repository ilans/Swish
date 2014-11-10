#include <Adafruit_NeoPixel.h>

#include "Trail.h"

#include <StandardCplusplus.h>
#include <vector>

#define PIN 10
int strip_len = 150;
int r;

using namespace std;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(strip_len, PIN, NEO_GRB + NEO_KHZ800);
int rfid_locations[] = {0,22,44,66,87,108,129,148};
vector<Trail> trails;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  r = random(0, rfid_locations.size());
  LightRFID(rfid_locations[r], strip.Color(255,0,0)
}

void loop() {

  for(int i=0; i<trails.size(); i++){
    if(trails[i].move()){
      trails.erase(trails.begin()+i);
    }
  }

  while(Serial1.available()>0){
    Serial.print((char)Serial1.read());
  }
}

void LightRFID(int first_led, int color){
  for(int i=0; i<2; i++){
    strip.setPixelColor(first_led+i, color);
  }
}

