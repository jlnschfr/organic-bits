Adafruit_NeoPixel strips[] = {
  Adafruit_NeoPixel(numPixels, 2, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numPixels, 3, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numPixels, 4, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numPixels, 5, NEO_GRB + NEO_KHZ800), Adafruit_NeoPixel(numPixels, 6, NEO_GRB + NEO_KHZ800)
};

void initLight() {
  for (int i = 0; i < maxLights; i++)
  {
    strips[i].begin();
    strips[i].show(); // Initialize all pixels to 'off'
  }
}

void resetPixel() {
  for (int i = 0; i < maxLights; i++)
  {
    for (int j = 0; j < numPixels; j++) {
      strips[i].setPixelColor(j, strips[i].Color(0, 0, 0));
    }
  }
}

void showPixel() {
  for (int i = 0; i < maxLights; i++) {
    strips[i].show();
  }
}

void fadeOutPixel() {
  //Serial.println("fade out");
  for (int i = 0; i < maxLights; i++) {
    for (int j = 0; j < numPixels; j++) {
      uint32_t color = strips[i].getPixelColor(j);

      if (color != 0 && allowRelease[i][j] == true) {
        int currentBlue =  color & 255;
        int currentGreen = (color >> 8) & 255;
        int currentRed =   (color >> 16) & 255;

        if ((currentRed - 10) > 0) {
          currentRed = currentRed - 10;
        } else if (delayValue > 1) {
          currentRed = delayValue;
        } else {
          currentRed = 0;
        }

        if ((currentGreen - 10) > 0) {
          currentGreen = currentGreen - 10;
        } else if (delayValue > 1) {
          currentGreen =  delayValue;
        } else {
          currentGreen = 0;
        }

        if ((currentBlue - 10) > 0) {
          currentBlue = currentBlue - 10;
        } else if (delayValue > 1) {
          currentBlue = delayValue;
        } else {
          currentBlue = 0;
        }

        strips[i].setPixelColor(j, strips[i].Color(currentRed, currentGreen, currentBlue));

        if (currentRed == 0 && currentGreen == 0 && currentBlue == 0) {
          allowRelease[i][j] = false;
        }
      }
    }
  }
}

void fadeInPixel() {
  //Serial.println("fadeInPixel");

  for (int i = 0; i < maxLights; i++) {
    for (int j = 0; j < numPixels; j++) {
      uint32_t color = strips[i].getPixelColor(j);

      if (color > 0 && color < 16777215 && allowRelease[i][j] == false) {
        //Serial.println("fade in");

        int currentBlue =  color & 255;
        int currentGreen = (color >> 8) & 255;
        int currentRed =   (color >> 16) & 255;

        if ((currentRed + 10) < cutoffValue) {
          currentRed = currentRed + 10;
        } else {
          currentRed = cutoffValue;
        }

        if ((currentGreen + 10) < cutoffValue) {
          currentGreen = currentGreen + 10;
        } else {
          currentGreen = cutoffValue;
        }

        if ((currentBlue + 10) < cutoffValue) {
          currentBlue = currentBlue + 10;
        } else {
          currentBlue = cutoffValue;
        }

        strips[i].setPixelColor(j, strips[i].Color(currentRed, currentGreen, currentBlue));

        if (currentBlue == cutoffValue && currentGreen == cutoffValue && currentRed == cutoffValue) {
          allowRelease[i][j] = true;
        }
      }
    }
  }
}
