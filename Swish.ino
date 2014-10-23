#include <Adafruit_NeoPixel.h>

#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <avr/pgmspace.h>

 #include <SoftwareSerial.h>

// #include "Trail.h"

#define PIN 10

int antenna_num = 0;

int rfid_locations[] = {2,26,46,67,86,109,127,145};
SoftwareSerial rfid_serials[8]{
  SoftwareSerial(2,11),
  SoftwareSerial(3,11),
  SoftwareSerial(4,11),
  SoftwareSerial(5,11),
  SoftwareSerial(6,11),
  SoftwareSerial(7,11),
  SoftwareSerial(8,11),
  SoftwareSerial(9,11)
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// std::basic_oserialstream<char, std::char_traits<char>, Serial_> cout(Serial); //when using Leonardo
using namespace std;

// vector<Trail> trails;

void setup() {
  Serial.begin(115200);

  for(int i=0 ; i<8 ; i++){
    rfid_serials[i].begin(9600);
  }

  // strip.begin();
  // strip.setBrightness(255);
  // strip.show();

  // for(int r=0; r<8; r++){
  //   int first_pix = rfid_locations[r];
  //   for(int i=0; i<2; i++){
  //     strip.setPixelColor(first_pix+i, strip.Color(0, ,0255));
  //   }
  // }
  // strip.show();


  // trails.push_back(Trail(173,255,47,rfid_locations[1],rfid_locations[6]));
}

void loop() {
  // for(int i=0; i<trails.size(); i++){ trails[i].move(); }

  readSerialNew(rfid_serials[antenna_num]);
  antenna_num++;
  if(antenna_num == sizeof(rfid_serials)/sizeof(*rfid_serials)){
    antenna_num = 0;
  }
}

void readSerialNew(SoftwareSerial& ser){
  ser.listen();
  delay(2);
  String tag_str = "";
  int count = 0;
  byte bytes[7];
  bool found = false;
  while (ser.available()>0){
    byte b = ser.read();
    if(b == 0x02){
      // Serial.print("found");
      found = true;
    } else if(found){
      if(count>=5){
        bytes[count-5] = b;
      }
      count++;
      if (count==12){
        PrintTag(bytes);
        break;
      }
    }
  }
}

void PrintTag(byte bytes[]){
  Serial.print(antenna_num);
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
