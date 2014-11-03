#include "Trail.h"

// extern Adafruit_NeoPixel strip;

Trail::Trail(uint8_t _r, uint8_t _g, uint8_t _b, int _origin, int _target){
  r = _r;
  g = _g;
  b = _b;
  origin = _origin;
  target = _target;
  pix = origin;
  r_fade = r/length;
  g_fade = g/length;
  b_fade = b/length;
  delta = target-origin;
  wait_time = (unsigned long)(1.0/speed);
  move_right = target-origin > 0 ? true : false ;
}

bool Trail::move(){
  int cur_time = millis();
  if(cur_time-move_time >= 0){
    move_time = cur_time+wait_time;
    for(int j=0; j<=length; j++) {
      if(move_right){
        // strip.setPixelColor(pix-j, strip.Color(r-j*r_fade, g-j*g_fade, b-j*b_fade));
      } else {
        // strip.setPixelColor(strip.numPixels()-1-(pix-j), strip.Color(r-j*r_fade, g-j*g_fade, b-j*b_fade));
      }
    }
    // strip.show();
    pix += move_right ? 1 : -1;
    if(pix==target) return true;
  }
  return false;
}
