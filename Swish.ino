// #include <SoftwareSerial.h>
#include <AltSoftSerial.h>

int antenna_num = 0;

// byte prebytes[] = {0x02, 0x31, 0x32, 0x30, 0x30, 0x37};
byte prebytes[] = {0x30, 0x30, 0x37};

int rfid_locations[] = {2,26,46,67,86,109,127,145};
// SoftwareSerial rfid_serials[8]{
//   SoftwareSerial(2,12),
//   SoftwareSerial(3,12),
//   SoftwareSerial(4,12),
//   SoftwareSerial(5,12),
//   SoftwareSerial(6,12),
//   SoftwareSerial(7,12),
//   SoftwareSerial(10,12),
//   SoftwareSerial(11,12)
// };

AltSoftSerial altSerial;

void setup() {
  Serial.begin(115200);

  for(int i=0 ; i<3 ; i++){
    pinMode(i+4,OUTPUT);
    digitalWrite(i+4,0);
  }

  // for(int i=0 ; i<8 ; i++){
  //   rfid_serials[i].begin(9600);
  // }

  altSerial.begin(9600);

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

  // readSerialNew(rfid_serials[antenna_num]);
  readSerialMux();
  antenna_num++;
  if(antenna_num == 8){
    antenna_num = 0;
    // Serial.println();
  }

  // while(Serial.available()>0) {
  //   byte b = Serial.read();
  //   Serial.print("0x");
  //   if(b<10){
  //     Serial.print("0");
  //   }
  //   Serial.print(b, HEX);
  //   Serial.print(" ");
  // }
}

void readSerialMux(){
  // Serial.print(antenna_num);

  String tag_str = "";
  int count = 0;
  byte bytes[3] = {0,0,0};
  byte id[7];
  bool avail = false;
  bool found = false;
  bool done = false;

  //set mux
  for(int i=0 ; i<3 ; i++){
    digitalWrite(i+4,bitRead(antenna_num,i));
  }

  //clear buffer
  altSerial.flushInput();

  // if(altSerial.available()>0) {
  //   byte b = altSerial.read();
  //   Serial.print("0x");
  //   if(b<10){
  //     Serial.print("0");
  //   }
  //   Serial.print(b, HEX);
  //   Serial.print(" ");
  // }

  delay(3);

  //wait for data to arrive
  unsigned long mil = millis();

  while (altSerial.available()>0) {
    byte b = altSerial.read();

    // avail = true;
    // Serial.print("0x");
    // if(b<10){
    //   Serial.print("0");
    // }
    // Serial.print(b, HEX);
    // Serial.print(" ");

    if(!found){
      bytes[0] = bytes[1];
      bytes[1] = bytes[2];
      bytes[2] = b;
      for(int a=0 ; a<3 ; a++){
        if(bytes[a] == prebytes[a]){
          if(a==2){
            found = true;
            Serial.print("found: ");
          }
        } else { break; }
      }
    } else {
      id[count] = b;
      count++;
      if (count==7){
        PrintTag(id);
        // altSerial.write(antenna_num+1);
        break;
      }
    }
    delay(1);
  }

  if(avail) Serial.println();
}

// void readSerialNew(SoftwareSerial& ser){
//   ser.listen();
//   delay(2);
//   String tag_str = "";
//   int count = 0;
//   byte bytes[7];
//   bool found = false;
//   while (ser.available()>0){
//     byte b = ser.read();
//     if(b == 0x02){
//       // Serial.print("found");
//       found = true;
//     } else if(found){
//       if(count>=5){
//         bytes[count-5] = b;
//       }
//       count++;
//       if (count==12){
//         PrintTag(bytes);
//         altSerial.write(antenna_num+1);
//         break;
//       }
//     }
//   }
// }

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
