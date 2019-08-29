//pattern2
int nextPixel[maxLights];
unsigned long nextPixelIntervall[maxLights];

//pattern3
int nextPixelPattern3[maxLights][2];

//pattern4
int prevLight = 0;
int startPixel = (numPixels / 2) - (10 * (currentModulation + 1));
int endPixel = numPixels / 2;
boolean lowerCase = true;

//pattern5
int currentLight = 0;

//pattern6
int currentBlock[maxLights];

void initPatternData() {
  //Pattern2
  for (int i = 0; i < maxLights; i++) {
    nextPixel[i] = (numPixels - 1) - i * 10;
  }

  for (int i = 0; i < maxLights; i++) {
    nextPixelIntervall[i] = millis();
  }

  //Pattern3
  for (int i = 0; i < maxLights; i++) {
    for (int j = 0; j < 2; j++) {
      nextPixelPattern3[i][j] = (numPixels / 2);
    }
  }

  //Pattern6
  for (int i = 0; i < maxLights; i++) {
    currentBlock[i] = 0;
  }
}

//GLIMMEN & GLUEHEN
void showPattern1() {

  int randomLight = random(0, maxLights);
  int randomPixel = random(0, numPixels - 1);

  for (int i = 0; i <= currentModulation; i++) {
    if (attackValue > 1)
    {
      allowRelease[randomLight][randomPixel + i] = false;
      strips[randomLight].setPixelColor(randomPixel + i, strips[randomLight].Color(red, green, blue));
    } else {
      allowRelease[randomLight][randomPixel + i] = true;
      strips[randomLight].setPixelColor(randomPixel + i, strips[randomLight].Color(cutoffValue, cutoffValue, cutoffValue));
    }
  }
}

void hidePattern1() {

  if (releaseValue == 10) {

    int randomLight = random(0, maxLights);
    int randomPixel = random(0, numPixels - 1);

    for (int i = 0; i <= currentModulation; i++) {
      allowRelease[randomLight][randomPixel + i] = true;
      strips[randomLight].setPixelColor(randomPixel + i, strips[randomLight].Color(0, 0, 0));
    }
  }
}

//REGEN
void showPattern2() {

  for (int i = 0; i < maxLights; i++) {
    if (now >= nextPixelIntervall[i]) {

      for (int j = 0; j <= currentModulation; j++) {
        if (attackValue > 1)
        {
          allowRelease[i][nextPixel[i] + j] = false;
          strips[i].setPixelColor(nextPixel[i] + j, strips[i].Color(red, green, blue));
        } else {
          allowRelease[i][nextPixel[i] + j] = true;
          strips[i].setPixelColor(nextPixel[i] + j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
        }
      }

      if ((nextPixel[i] - 1 - currentModulation) >= 0) {
        nextPixel[i] = (nextPixel[i] - 1 - currentModulation);
      } else {
        nextPixel[i] = numPixels - 1;
      }

      if (i == 0) {
        nextPixelIntervall[i] = now + 20;
      } else if (i == 1) {
        nextPixelIntervall[i] = now + 200;
      } else if (i == 2) {
        nextPixelIntervall[i] = now + 50;
      } else if (i == 3) {
        nextPixelIntervall[i] = now + 350;
      }
    }
  }
}

void hidePattern2() {}

//WELLEN
void showPattern3() {
  for (int i = 0; i < maxLights; i++) {
    if (now >= nextPixelIntervall[i]) {
      for (int j = 0; j <= currentModulation; j++) {
        if (attackValue > 1)
        {
          allowRelease[i][nextPixelPattern3[i][0] + j] = false;
          allowRelease[i][nextPixelPattern3[i][1] - j] = false;
          strips[i].setPixelColor(nextPixelPattern3[i][0] + j, strips[i].Color(red, green, blue));
          strips[i].setPixelColor(nextPixelPattern3[i][1] - j, strips[i].Color(red, green, blue));
        } else {
          allowRelease[i][nextPixelPattern3[i][0] + j] = true;
          allowRelease[i][nextPixelPattern3[i][1] - j] = true;
          strips[i].setPixelColor(nextPixelPattern3[i][0] + j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
          strips[i].setPixelColor(nextPixelPattern3[i][1] - j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
        }
      }

      if ((nextPixelPattern3[i][0] + currentModulation) < numPixels - 1)
      {
        nextPixelPattern3[i][0] = nextPixelPattern3[i][0] + 1;
      } else {
        nextPixelPattern3[i][0] = numPixels / 2;
      }

      if ((nextPixelPattern3[i][1] - currentModulation) > 1)
      {
        nextPixelPattern3[i][1] = nextPixelPattern3[i][1] - 1;
      } else {
        nextPixelPattern3[i][1] = numPixels / 2;
      }


      if (i == 0) {
        nextPixelIntervall[i] = now + 250;
      } else if (i == 1) {
        nextPixelIntervall[i] = now + 100;
      } else if (i == 2) {
        nextPixelIntervall[i] = now + 200;
      } else if (i == 3) {
        nextPixelIntervall[i] = now + 50;
      }
    }
  }
}

void hidePattern3() {}


//BLITZEN
void showPattern4() {

  int randomLight = 0;

  for (int i = 0; i <= currentModulation; i++) {

    randomLight = random(0, maxLights);

    while (prevLight == randomLight) {
      randomLight = random(0, maxLights);
    }

    for (int j = startPixel; j < endPixel; j++) {
      if (attackValue > 1)
      {
        allowRelease[randomLight][j] = false;
        strips[randomLight].setPixelColor(j, strips[randomLight].Color(red, green, blue));
      } else {
        allowRelease[randomLight][j] = true;
        strips[randomLight].setPixelColor(j, strips[randomLight].Color(cutoffValue, cutoffValue, cutoffValue));
      }
    }

    if (lowerCase) {
      startPixel = numPixels / 2;
      endPixel = (numPixels / 2) + (10 * (currentModulation + 1));
      lowerCase = false;

    } else {
      startPixel = (numPixels / 2) - (10 * (currentModulation + 1));
      endPixel = numPixels / 2;
      lowerCase = true;
    }

    prevLight = randomLight;

  }
}

void hidePattern4() {
}



//PULSIEREN / WABERN
void showPattern5() {

  if (currentModulation == 0 || currentModulation == 1)
  {
    for (int i = 0; i < numPixels; i++)
    {
      if (attackValue > 1)
      {
        allowRelease[currentLight][i] = false;
        strips[currentLight].setPixelColor(i, strips[currentLight].Color(red, green, blue));
      } else {
        allowRelease[currentLight][i] = true;
        strips[currentLight].setPixelColor(i, strips[currentLight].Color(cutoffValue, cutoffValue, cutoffValue));
      }
    }

  } else if (currentModulation == 2) {

    int light1, light2;

    if (currentLight == 1 || currentLight == 3)
    {
      light1 = 1;
      light2 = 2;
    } else {
      light1 = 0;
      light2 = 3;
    }

    for (int i = 0; i < numPixels; i++)
    {
      if (attackValue > 1)
      {
        allowRelease[light1][i] = false;
        allowRelease[light2][i] = false;
        strips[light1].setPixelColor(i, strips[light1].Color(red, green, blue));
        strips[light2].setPixelColor(i, strips[light2].Color(red, green, blue));
      } else {
        allowRelease[light1][i] = true;
        allowRelease[light2][i] = true;
        strips[light1].setPixelColor(i, strips[light1].Color(cutoffValue, cutoffValue, cutoffValue));
        strips[light2].setPixelColor(i, strips[light2].Color(cutoffValue, cutoffValue, cutoffValue));
      }
    }
  }

  if (currentModulation == 0 || currentModulation == 2) {
    if (currentLight < maxLights - 1)
    {
      currentLight++;
    } else {
      currentLight = 0;
    }
  } else if (currentModulation == 1) {
    if (currentLight > 0)
    {
      currentLight--;
    } else {
      currentLight = maxLights - 1;
    }
  }
}

void hidePattern5() {
  if (releaseValue == 10)
  {

    if (currentModulation == 0 || currentModulation == 1) {
      int lightOff;

      if (currentModulation == 0) {
        if (currentLight == 0)
        {
          lightOff = maxLights - 1;
        } else {
          lightOff = currentLight - 1;
        }
      } else if (currentModulation == 1) {
        if (currentLight == maxLights - 1)
        {
          lightOff = 0;
        } else {
          lightOff = currentLight + 1;
        }
      }

      for (int i = 0; i < numPixels; i++)
      {
        strips[lightOff].setPixelColor(i, strips[lightOff].Color(0, 0, 0));
      }
    } else {
      if (currentLight == 1 || currentLight == 3)
      {
        for (int i = 0; i < numPixels; i++)
        {
          strips[0].setPixelColor(i, strips[0].Color(0, 0, 0));
          strips[3].setPixelColor(i, strips[3].Color(0, 0, 0));
        }
      } else {
        for (int i = 0; i < numPixels; i++)
        {
          strips[1].setPixelColor(i, strips[1].Color(0, 0, 0));
          strips[2].setPixelColor(i, strips[2].Color(0, 0, 0));
        }
      }
    }
  }
}


//ATMEN
void showPattern6() {
  if (currentModulation == 0 || currentModulation == 1)
  {
    int blockCount;
    int counter;
    int startPixel;
    int endPixel;

    if (currentModulation == 0) {
      blockCount = numPixels / 6;
      counter = numPixels / 6;
    } else if (currentModulation == 1) {
      blockCount = numPixels / 6;
      counter = 1;
    }

    for (int i = 0; i < maxLights; i++)
    {
        if (i == 0 || i == 3)
        {
          startPixel = (numPixels - 1) - currentBlock[i];
          endPixel = (numPixels - 1) - currentBlock[i] - blockCount;

          for (int j = startPixel; j > endPixel; j -= counter ) {
            if (attackValue > 1)
            {
              allowRelease[i][j] = false;
              strips[i].setPixelColor(j, strips[i].Color(red, green, blue));
            } else {
              allowRelease[i][i] = true;
              strips[i].setPixelColor(j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
            }
          }

        } else {
          startPixel = 0 + currentBlock[i];
          endPixel = blockCount + currentBlock[i];

          for (int j = startPixel; j < endPixel; j += counter ) {
            if (attackValue > 1)
            {
              allowRelease[i][j] = false;
              strips[i].setPixelColor(j, strips[i].Color(red, green, blue));
            } else {
              allowRelease[i][i] = true;
              strips[i].setPixelColor(j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
            }
          }
        }

        if (currentBlock[i] < numPixels - blockCount) {
          currentBlock[i] = currentBlock[i] + blockCount;
        } else {
          currentBlock[i] = 0;
        }
    }

  } else {
    for (int i = 0; i < maxLights; i++)
    {
      for (int j = 0; j < numPixels; j++ ) {
        if (attackValue > 1)
        {
          allowRelease[i][j] = false;
          strips[i].setPixelColor(j, strips[i].Color(red, green, blue));
        } else {
          allowRelease[i][i] = true;
          strips[i].setPixelColor(j, strips[i].Color(cutoffValue, cutoffValue, cutoffValue));
        }
      }
    }
  }
}

void hidePattern6() {}
