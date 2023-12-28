#define SUNRISE_NUM_INCS 10

int sunriseIncCount;
unsigned long sunriseMsBetweenIncs;

void sunriseStart() {
    strip.clear();
    strip.show();
  
    // data type must store 86400 (seconds in a day) which is more than 2 bytes
    unsigned long bySeconds = 0;
    bySeconds |= ((unsigned long) postContent[1]) << 16;
    bySeconds |= ((unsigned long) postContent[2]) << 8;
    bySeconds |= ((unsigned long) postContent[3]);
    Serial.print("sunrise BY seconds=");
    Serial.println(bySeconds);
    unsigned long byMillis = bySeconds * 1000;

    unsigned long overMillis = postContent[7] * 60L * 1000;
    Serial.print("sunrise OVER millis=");
    Serial.println(overMillis);

    sunriseIncCount = 1;
    sunriseMsBetweenIncs = overMillis / SUNRISE_NUM_INCS;
    Serial.print("sunrise between millis=");
    Serial.println(sunriseMsBetweenIncs);

    sunriseTask = timer.in(byMillis - overMillis, sunriseIncrementStart);
}

bool sunriseIncrementStart(void* arg) {
  // Starting the process of brightness incrementing
  Serial.println("@@ sunrise inc start");

  timer.cancel(sunriseTask);
  sunriseTask = timer.every(sunriseMsBetweenIncs, sunriseIncrement);

  return false;
}

bool sunriseIncrement(void* arg) {
  
  if(sunriseIncCount >= SUNRISE_NUM_INCS) {
    Serial.println("@@ SUNRISE DONE!");

    strip.fill(rgbToColor(0, 200, 0));
    strip.show();
    // TODO start rave?
    return false;
  } else {
    Serial.println("@@ sunrise inc-ing");

    strip.fill(rgbToColor(postContent[4], postContent[5], postContent[6], (float)sunriseIncCount/SUNRISE_NUM_INCS));
    strip.show();
    sunriseIncCount += 1;

    return true;
  }
}
