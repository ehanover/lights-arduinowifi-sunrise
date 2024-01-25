#define FIREWORKS_DELAY_MIN 10
#define FIREWORKS_DELAY_MAX 70

#define FIREWORKS_LIFETIME 40

#define FIREWORKS_NUM_COLORS 4
const unsigned char fireworksColors[] = { // brightness is handled in main file with setBrightness()
  50, 130, 255, // blue
  220, 150, 90, // yellow
  255, 100, 50, // red
  220, 70, 240, // purple
};

int x0;
int x1;
size_t c0;
size_t c1;
int t0;
int t1;


void firework(int ledIndex, int center, int colorIndex, int t) {
  int diff = abs(ledIndex - center);
  if(diff < 9) { // sets 2*width
    if(t < FIREWORKS_LIFETIME) {
      // full width decreasing
      float b = ((float)t)/FIREWORKS_LIFETIME;
      b /= 1 + (diff / 5.0);

      strip.setPixelColor(ledIndex, rgbToColor(fireworksColors[colorIndex], fireworksColors[colorIndex+1], fireworksColors[colorIndex+2], b));

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
    t0 = FIREWORKS_LIFETIME + random(0, 50);
    c0 = random(0, FIREWORKS_NUM_COLORS) * 3;
  }

  t1 -= 1;
  if(t1 < 0) {
    x1 = random(0, LED_COUNT);
    t1 = FIREWORKS_LIFETIME + random(10, 70);
    c1 = random(0, FIREWORKS_NUM_COLORS) * 3;
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
