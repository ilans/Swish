#include <Adafruit_NeoPixel.h>

#include "Trail.h"

#include <StandardCplusplus.h>
#include <vector>

#define PIN 10
int strip_len = 150;
int loc;
bool loc_is_on;
int next_loc;
int reader;
int tag;
bool in_motion = false;
bool direction;

int cnt = 0;

using namespace std;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(strip_len, PIN, NEO_GRB + NEO_KHZ800);
int rfid_locations[] = {0,22,44,66,87,108,129,148};
vector<Trail> trails;

void setup() {
  // delay(2000);
  Serial.begin(115200);
  Serial1.begin(115200);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  randomSeed(analogRead(0));

  loc = GetRandLoc();

  loc = 7;
  ShowRFID(rfid_locations[loc], strip.Color(255,0,0));
}

void loop() {
  if(Serial1.available()>8){
    reader = Serial1.parseInt();
    tag = Serial1.parseInt();
    if(reader>tag){
      int tmp = tag;
      tag = reader;
      reader = tmp;
    }

    if(tag>0 && !in_motion){
      Serial.print(reader);
      Serial.print(":");
      Serial.println(tag);

      if(reader == loc){
        Serial.println("hit");

        switch (cnt) {
            case 0:
              next_loc = 0;
              direction = 0;
              break;
            default:
              next_loc = GetRandLoc();
              while(next_loc == loc) { next_loc = GetRandLoc(); }
              direction = random(2);
              break;
        }
        cnt++;

        trails.push_back(Trail(173,255,47,rfid_locations[loc],rfid_locations[next_loc], direction));

        Serial.print("Trail from ");
        Serial.print(loc);
        Serial.print(" to ");
        Serial.print(next_loc);
        Serial.print(" in direction ");
        Serial.println(direction);

        in_motion = true;
      }
    }
  }

  for(int i=0; i<trails.size(); i++){
    ShowRFID(rfid_locations[loc], strip.Color(0,0,0));
    if(trails[i].move()){
      trails.erase(trails.begin()+i);
      loc = next_loc;
      ShowRFID(rfid_locations[loc], strip.Color(255,0,0));
      in_motion = false;
    }
  }
}

void ShowRFID(int first_led, uint32_t color){
  if(color>0 || (color==0 && loc_is_on)){
    for(int i=0; i<2; i++){
      strip.setPixelColor(first_led+i, color);
    } 
    loc_is_on = color>0;
    strip.show();
  }
}

int GetRandLoc(){
  return random(sizeof(rfid_locations)/sizeof(int));
}


// test:
//       trails.push_back(Trail(173,255,47,rfid_locations[7],rfid_locations[0], false));
//?       trails.push_back(Trail(173,255,47,rfid_locations[0],rfid_locations[0], false));
// Trail from 6 to 3 in direction 1
// Trail from 3 to 2 in direction 1

