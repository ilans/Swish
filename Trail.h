#ifndef _TRAIL_H_
#define _TRAIL_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

class Trail{
public:
  int origin;
  int target;

  Trail(uint8_t _r, uint8_t _g, uint8_t _b, int _o, int _t, bool _d,  float _l = 30.0, float _m = 0.05);
  bool move();

private:
  float r_fade;
  float g_fade;
  float b_fade;
  float leds_per_mil = 0.05;
  float length = 30.0;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  int start_mil = 0;
  bool direction; // true if right->left
  bool o2t_dir; // true if target located to the right or the origin
  int o2t_delta; // amount of pixels between origin and target in set direction
  int prv_delta_pix = 0;
};

#endif // _TRAIL_H_
