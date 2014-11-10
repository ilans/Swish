#include "Trail.h"
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;
extern int strip_len;

Trail::Trail(uint8_t _r, uint8_t _g, uint8_t _b, int _o, int _t, bool _d, float _l, float _m):
r(_r),g(_g),b(_b),origin(_o),target(_t),length(_l),direction(_d),leds_per_mil(_m){
  r = _r;
  g = _g;
  b = _b;
  origin = _o;
  target = _t;
  length = _l;
  direction = _d;
  leds_per_mil = _m;

  r_fade = r/length;
  g_fade = g/length;
  b_fade = b/length;
  o2t_delta = target - origin;
  o2t_dir = o2t_delta>0 ;
  if (direction){
    if (o2t_delta<0) {
      o2t_delta += strip_len;
    }
  } else if (o2t_delta<0) {
    o2t_delta *= -1;
  } else {
    o2t_delta = origin - target + strip_len;
  }
}

bool Trail::move(){
  int cur_mil = millis();
  if (start_mil==0) {start_mil = cur_mil;}
  int delta_pix = float(cur_mil-start_mil)*leds_per_mil;
  if(prv_delta_pix != delta_pix) {
    prv_delta_pix = delta_pix;
    int head_pix = origin + delta_pix;
    if(!direction) head_pix = origin - delta_pix;
    if(head_pix<0) head_pix += strip_len;
    if(head_pix>strip_len-1) head_pix -= strip_len;
    for(int j=0; j<=length; j++) {
      int p = head_pix-j;
      if(!direction){
        p = head_pix+j;
      }
      if(p<0) p += strip_len;
      if(p>strip_len-1) p -= strip_len;
      if( ( o2t_dir &&  direction) && (p>=origin && p<=target) ||
          ( o2t_dir && !direction) && (p<=origin || p>=target) ||
          (!o2t_dir && !direction) && (p<=origin && p>=target) ||
          (!o2t_dir &&  direction) && (p>=origin || p<=target) )
      {
        strip.setPixelColor(p, strip.Color(r-j*r_fade, g-j*g_fade, b-j*b_fade));
      }
    }
    strip.show();
  }
  if (delta_pix - length >= o2t_delta) return true;
  return false;
}
