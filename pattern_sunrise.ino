#define SUNRISE_NUM_INCS 10

int sunriseIncCount;
unsigned long sunriseMsBetweenIncs;

void sunriseStart() {
    strip.setBrightness(DEFAULT_MAX_BRIGHTNESS);
    flash(rgbToColor(100, 100, 100), 1000, true);
  
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

    if(overMillis > byMillis) {
      Serial.println("Sunrise target time is too soon! Exiting pattern");
      strip.fill(rgbToColor(100, 0, 0));
      strip.show();
      return;
    }

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

  return true; // this logically should be 'false' but I think it messes up the new task that is just starting
}

bool sunriseIncrement(void* arg) {
  
  if(sunriseIncCount >= SUNRISE_NUM_INCS) {
    Serial.println("@@ SUNRISE DONE!");

    strip.fill(rgbToColor(postContent[4], postContent[5], postContent[6])); // full brightness
    strip.setPixelColor(0, rgbToColor(0,255,0));
    strip.setPixelColor(LED_COUNT - 1, rgbToColor(0,255,0));
    strip.show();

    // Alternatively could trigger rainbow pattern
    return false;
  } else {
    Serial.println("@@ sunrise incrementing");

    strip.fill(rgbToColor(postContent[4], postContent[5], postContent[6], (float)sunriseIncCount/SUNRISE_NUM_INCS));
    strip.show();
    sunriseIncCount += 1;

    return true;
  }
}
