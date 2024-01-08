#define SWIRL_DELAY_MIN 5
#define SWIRL_DELAY_MAX 300

#define GRADIENT_SIZE 300
uint32_t swirlGradient[GRADIENT_SIZE];

float r0 = 0;
float r1 = 1; // random starting offset

void calculateGradient() {
  const int third = GRADIENT_SIZE / 3;
  const float facScale = 1.0 / third;

  float fac;
  for(int i=0; i<third; i++) {
    fac = i * facScale;
    swirlGradient[i] = hueMix(postContent[1], postContent[2], fac);
  }
  for(int i=third; i<third*2; i++) {
    fac = (i-third) * facScale;
    swirlGradient[i] = hueMix(postContent[2], postContent[3], fac);
  }
  for(int i=third*2; i<GRADIENT_SIZE; i++) {
    fac = (i-third*2) * facScale;
    swirlGradient[i] = hueMix(postContent[3], postContent[1], fac);
  }
}

float noise(int i) {
  float si = (6.28 * i) / LED_COUNT;
  return 0.3 * (1 + sin(0.7 * si + r0)) + 0.2 * (1 + sin(r1 - 0.4 * si));
}

void swirlStart() {
  strip.setBrightness(postContent[5]);
  flash(rgbToColor(100, 100, 100), 200, false);

  calculateGradient();
}

void swirlUpdate() {
  r0 += 0.017;
  if(r0 >= 6.28) {
    r0 -= 6.28;
  }
  r1 += 0.006;
  if(r1 >= 6.28) {
    r1 -= 6.28;
  }

  for(int i=0; i<LED_COUNT; i++) {
    float n = noise(i);
    strip.setPixelColor(i, swirlGradient[int(n * GRADIENT_SIZE)]);
  }

  strip.show();
  patternDelay(postContent[4], SWIRL_DELAY_MIN, SWIRL_DELAY_MAX);
}
