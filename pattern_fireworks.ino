#define FIREWORKS_DELAY_MIN 20
#define FIREWORKS_DELAY_MAX 80

#define FIREWORKS_LIFETIME 35

int x0;
int x1;
uint32_t c0 = 0;
uint32_t c1 = 1;
int t0;
int t1;


void firework(int ledIndex, int center, uint32_t color, int t) {
  int diff = abs(ledIndex - center);
  if(diff < 9) { // sets 2*width
    if(t < FIREWORKS_LIFETIME) {
      // full width decreasing
      float b = ((float)t)/FIREWORKS_LIFETIME;
      b /= 1 + (diff / 5.0);

      // TODO fix
      uint32_t tempColor;
      if(color == 0) {
        tempColor = rgbToColor(255, 80, 80, b);
      } else {
        tempColor = rgbToColor(0, 120, 255, b);
      }

      strip.setPixelColor(ledIndex, tempColor);

    } else if(t < FIREWORKS_LIFETIME + 10 && ledIndex == center) {
      // center pixel increasing
      float b = (10 - (t - FIREWORKS_LIFETIME))/20.0;

      strip.setPixelColor(ledIndex, rgbToColor(255, 255, 255, b));
    }
  }
}

void fireworksStart() {
  strip.setBrightness(postContent[2]);
  flash(rgbToColor(100, 100, 100), 200, true);

  t0 = 0; // force reset upon next update
  t1 = 0;
}

void fireworksUpdate() {
  t0 -= 1;
  if(t0 < 0) {
    x0 = random(0, LED_COUNT);
    // c0 = raveFlashColors[random(0, FLASH_NUM_COLORS)];
    t0 = FIREWORKS_LIFETIME + random(0, 50);
  }

  t1 -= 1;
  if(t1 < 0) {
    x1 = random(0, LED_COUNT);
    // c1 = raveFlashColors[random(0, FLASH_NUM_COLORS)];
    t1 = FIREWORKS_LIFETIME + random(10, 60);
  }

  // overlapping the patterns is acceptable
  for(int i=0; i<LED_COUNT; i++) {
    firework(i, x0, c0, t0);
  }
  for(int i=0; i<LED_COUNT; i++) {
    firework(i, x1, c1, t1);
  }

  strip.show();
  // TODO this animation shouldn't be controlled like this
  patternDelay(postContent[1], FIREWORKS_DELAY_MIN, FIREWORKS_DELAY_MAX);
}
