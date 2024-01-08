uint32_t rgbToColor(unsigned char r, unsigned char g, unsigned char b) {
  return strip.gamma32(strip.Color(r, g, b));
}

uint32_t rgbToColor(unsigned char r, unsigned char g, unsigned char b, float brightness) {
  return strip.gamma32(strip.Color(r * brightness, g * brightness, b * brightness));
}

float mapF(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t hueMix(unsigned char h1, unsigned char h2, float fac) {
  // good reference https://iter.ca/post/hue-avg/
  float x1 = cos(6.28 * h1 / 255.0);
  float y1 = sin(6.28 * h1 / 255.0);
  float x2 = cos(6.28 * h2 / 255.0);
  float y2 = sin(6.28 * h2 / 255.0);

  float r = atan2(
    (y1*(1-fac) + y2*fac) / 2,
    (x1*(1-fac) + x2*fac) / 2
  );
  
  if(r < 0) {
    r += 6.28;
  }
  unsigned int hue = (r / 6.28) * 65535;
  // unsigned int hue = map(r, 0, 1024, 0, 65535);

  return strip.gamma32(strip.ColorHSV(hue, 255, 255)); // neopixel hue is 16-bit
  // return strip.gamma32(strip.ColorHSV(map(h1, 0, 255, 0, 65535), 255, 255));
}

void flash(uint32_t color, int duration, int finishBlack) {
  strip.fill(color);
  strip.show();
  delay(duration);

  if(finishBlack) {
    strip.clear();
    strip.show();
  }
}

// https://circuitjournal.com/arduino-force-inline
// inline void patternDelay(unsigned char, int, int) __attribute__((always_inline));
inline void patternDelay(unsigned char v, int min, int max) {
  delay(map(v, 255, 0, min, max)); // TODO try map() vs mapF()
}