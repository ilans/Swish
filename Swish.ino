#include <Adafruit_NeoPixel.h>

#include "Trail.h"

#include <StandardCplusplus.h>
#include <vector>
// #include <iterator>
// #include <avr/pgmspace.h>

#define PIN 10
int strip_len = 150;

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

  // for(int r=0; r<8; r++){
  //   int first_pix = rfid_locations[r];
  //   for(int i=0; i<2; i++){
  //     strip.setPixelColor(first_pix+i, strip.Color(0,0,255));
  //   }
  // }
  // strip.show();

  trails.push_back(Trail(173,255,47,rfid_locations[6],rfid_locations[1], false));
  trails.push_back(Trail(173,255,47,rfid_locations[4],rfid_locations[2], true));
}

void loop() {

  for(int i=0; i<trails.size(); i++){
    if(trails[i].move()){
      trails.erase(trails.begin()+i);
      trails.push_back(Trail(173,255,47,rfid_locations[6],rfid_locations[1], false));
    }
  }

  while(Serial1.available()>0){
    Serial.print((char)Serial1.read());
  }
}