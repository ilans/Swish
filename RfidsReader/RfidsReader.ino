#include <SoftwareSerial.h>

int antenna_num = 0;
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

void setup() {
  Serial.begin(115200);

  for(int i=0 ; i<8 ; i++){
    rfid_serials[i].begin(9600);
  }
}

void loop() {
  if(antenna_num>=8) antenna_num=0;
  readSerialNew(rfid_serials[antenna_num]);
  antenna_num++;
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
        // Serial.write(antenna_num+1);
        // PrintTag(bytes);
        SendTag(bytes);
        break;
      }
    }
  }
}

void SendTag(byte bytes[]){
  int sum = 0;
  for (int i=0 ; i<7 ; i++){
    sum += bytes[i];
  }
  Serial.print(antenna_num);
  Serial.print(",");
  Serial.println(sum);
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

