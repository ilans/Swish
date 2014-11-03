#include <Adafruit_NeoPixel.h>

// #include <AltSoftSerial.h>

#include "Trail.h"

#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <avr/pgmspace.h>

#define PIN 10

// std::basic_oserialstream<char, std::char_traits<char>, Serial_> cout(Serial);
using namespace std;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// byte prebytes[] = {0x02, 0x31, 0x32, 0x30, 0x30, 0x37};
byte prebytes[] = {0x30, 0x30, 0x37};
// int rfid_locations[] = {2,26,46,67,86,109,127,145};
int rfid_locations[] = {0,22,44,66,87,108,129,149};
// AltSoftSerial altSerial;
int antenna_num = 0;
vector<Trail> trails;

void setup() {
  Serial.begin(9600);

  for(int i=0 ; i<3 ; i++){
    pinMode(i+4,OUTPUT);
    digitalWrite(i+4,0);
  }

  // altSerial.begin(9600);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  for(int r=0; r<8; r++){
    int first_pix = rfid_locations[r];
    for(int i=0; i<2; i++){
      strip.setPixelColor(first_pix+i, strip.Color(0,0,255));
    }
  }
  strip.show();


  // trails.push_back(Trail(173,255,47,rfid_locations[1],rfid_locations[6]));
}

void loop() {
  // for(int i=0; i<trails.size(); i++){ trails[i].move(); }

  readSerialMux();
  antenna_num++;
  if(antenna_num == 8){
    antenna_num = 0;
  }
}

void readSerialMux(){
  byte bytes[3] = {0,0,0};
  byte id[7];
  int c = 0;
  bool found = false;

  //set mux address
  for(int i=0 ; i<3 ; i++){
    digitalWrite(i+4,bitRead(antenna_num,i));
  }

  //clear buffer
  // altSerial.flushInput();

  //wait for data to arrive
  delay(3);
  unsigned long mil = millis();

  while (Serial.available()>0) {
    byte b = Serial.read();
    if(!found){
      bytes[0] = bytes[1];
      bytes[1] = bytes[2];
      bytes[2] = b;
      for(int a=0 ; a<3 ; a++){
        if(bytes[a] == prebytes[a]){
          if(a==2){
            found = true;
          }
        } else { break; }
      }
    } else {
      id[c] = b;
      c++;
      if (c==7){
        PrintTag(id);
        break;
      }
    }
    delay(1);
  }

  if(found) Serial.println();
}

void PrintTag(byte bytes[]){
  Serial.print(antenna_num+1);
  Serial.print(": ");
  for (int i=0 ; i<7 ; i++){
    int b = bytes[i];
    Serial.print("0x");
    if(b<10){
      Serial.print("0");
    }
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();
}
