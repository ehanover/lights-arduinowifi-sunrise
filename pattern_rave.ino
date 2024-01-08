#define FLASH_DELAY_MIN 70
#define FLASH_DELAY_MAX 1000

#define FLASH_NUM_COLORS 6
const uint32_t raveFlashColors[] = { // brightness is handled in main file with setBrightness()
  rgbToColor(255, 0, 0),
  rgbToColor(0, 255, 0),
  rgbToColor(0, 0, 255),
  rgbToColor(255, 0, 255),
  rgbToColor(255, 255, 0),
  rgbToColor(0, 255, 255),
};
int raveFlashIndex = 0;


#define RAINBOW_DELAY_MIN 1
#define RAINBOW_DELAY_MAX 80

#define RAINBOW_HUE_INC 390 // use this for speed of rainbow animation. Higher means faster but animation is less smooth (ie bigger color jumps between frames)
int raveRainbowHue = 0;


void raveStart() {
  strip.setBrightness(postContent[3]);
}

void raveUpdate() {
  if(postContent[1] == 0) {
    // flashing colors option
    strip.fill(raveFlashColors[raveFlashIndex]);

    raveFlashIndex += 1;
    if(raveFlashIndex >= FLASH_NUM_COLORS) {
      raveFlashIndex -= FLASH_NUM_COLORS;
    }

    strip.show();
    patternDelay(postContent[2], FLASH_DELAY_MIN, FLASH_DELAY_MAX);
  } else {
    // rainbow mode option

    // Color wheel has a range of 65536 but it's OK if we roll over
    // 5*65536/256 = 1280 passes through loop
    strip.rainbow(raveRainbowHue); // equivalent to strip.rainbow(hue, 1, 255, 255, true);

    raveRainbowHue += RAINBOW_HUE_INC;
    if(raveRainbowHue >= 65535) {
      raveRainbowHue -= 65535;
    }

    strip.show();
    patternDelay(postContent[2], RAINBOW_DELAY_MIN, RAINBOW_DELAY_MAX);
  }
}
