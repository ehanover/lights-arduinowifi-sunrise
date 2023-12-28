uint32_t rgbToColor(unsigned char r, unsigned char g, unsigned char b) {
  return strip.gamma32(strip.Color(r, g, b));
}

uint32_t rgbToColor(unsigned char r, unsigned char g, unsigned char b, float brightness) {
  return strip.gamma32(strip.Color(r * brightness, g * brightness, b * brightness));
}

float mapF(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void flash(uint32_t color, unsigned long duration) {
  strip.fill(color);
  strip.show();
  delay(duration);
  strip.clear();
  strip.show();
}

void patternDelay(unsigned char v, unsigned long min, unsigned long max) {
  delay(map(255 - v, 0, 255, min, max)); // TODO try map() vs mapF()
}