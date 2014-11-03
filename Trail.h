#ifndef _TRAIL_H_
#define _TRAIL_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// #include <Adafruit_NeoPixel.h>

class Trail{
public:
  int origin;
  int target;

  Trail(uint8_t _r, uint8_t _g, uint8_t _b, int _origin, int _target);
  bool move();

private:
  float r_fade;
  float g_fade;
  float b_fade;
  unsigned long wait_time;
  float speed = 0.125;
  float length = 30.0;
  unsigned long move_time;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  int delta;
  int pix;
  bool move_right;
};

#endif // _TRAIL_H_
