#include <Adafruit_NeoPixel.h>
#include <MIDI.h>

#define maxLights 4
#define numPixels 60

bool allowRelease[maxLights][numPixels];

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);

//first button, second led
#define maxPattern 6
int patternValues[maxPattern];
int prevPatternValues[maxPattern];
int patternButtons[maxPattern][2] = {
    {42, 43},
    {44, 45},
    {46, 47},
    {48, 49},
    {50, 51},
    {52, 53}};

#define maxModulation 3
int modulationValues[maxModulation];
int prevModulationValues[maxModulation];
int modulationButtons[maxModulation][2] = {
    {28, 29},
    {30, 31},
    {32, 33}};

// cutoff, release, attack, delay
#define maxPotis 4
int potiValues[maxPotis];
int prevPotiValues[maxPotis];
int potis[maxPotis] = {A0, A1, A2, A3};

int currentPattern = 0;
int currentModulation = 0;
int cutoffValue = 255;
int releaseValue = 1;
int attackValue = 1;
int delayValue = 1;

int red = 1;
int green = 1;
int blue = 1;

unsigned long now, nextLightInterval, nextPotisInterval, nextButtonsInterval, nextFadeOutInterval, nextFadeInInterval;

void setup()
{
  MIDI.setHandleNoteOn(handleNoteOn);
  // MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(1);
  MIDI2.begin(2);

  Serial.begin(9600);
  Serial.println("init midi in");

  initLight();
  initPatternData();

  nextPotisInterval = millis();
  nextLightInterval = millis();
  nextButtonsInterval = millis();
  nextFadeOutInterval = millis();
  nextFadeInInterval = millis();

  //INIT ATTACK 2-DIMENSIONAL ARRAY
  initAttackArray();

  //INIT PATTERN BUTTONS/LEDs
  for (int i = 0; i < maxPattern; i++)
  {
    pinMode(patternButtons[i][0], INPUT);
    digitalWrite(patternButtons[i][0], HIGH);
    pinMode(patternButtons[i][1], OUTPUT);
  }

  //INIT MODULATION BUTTONS/LEDs
  for (int j = 0; j < maxModulation; j++)
  {
    pinMode(modulationButtons[j][0], INPUT);
    digitalWrite(modulationButtons[j][0], HIGH);
    pinMode(modulationButtons[j][1], OUTPUT);
  }

  //INIT FIRST PATTERN & FIRST MODULATON
  changePattern(0);
  changeModulation(0);
}

void initAttackArray()
{
  for (int i = 0; i < maxLights; i++)
  {
    for (int j = 0; j < numPixels; j++)
    {
      allowRelease[i][j] = true;
    }
  }
}

void handleNoteOn(byte channel, byte noteByte, byte velocityByte)
{
  //Serial.println("handleNoteOn");
  if (noteByte > 35 && noteByte < 42)
  {
    changeProgramAutomatically(noteByte, velocityByte);
  }

  showCurrentPattern();
}

void handleNoteOff(byte channel, byte noteByte, byte velocityByte)
{
  //Serial.println("handleNoteOff");
  hideCurrentPattern();
}

void changeProgramAutomatically(byte noteByte, byte velocityByte)
{
  int modulationByte = map(velocityByte, 0, 127, 0, 2);
  changeModulation(modulationByte);

  if (noteByte == 36)
  {
    changePattern(0);
  }
  else if (noteByte == 37)
  {
    changePattern(1);
  }
  else if (noteByte == 38)
  {
    changePattern(2);
  }
  else if (noteByte == 39)
  {
    changePattern(3);
  }
  else if (noteByte == 40)
  {
    changePattern(4);
  }
  else if (noteByte == 41)
  {
    changePattern(5);
  }
}

void checkPotis()
{
  for (int i = 0; i < maxPotis; i++)
  {
    potiValues[i] = analogRead(potis[i]);

    if (potiValues[i] != prevPotiValues[i] && potiValues[i] != prevPotiValues[i] + 1 && potiValues[i] != prevPotiValues[i] - 1)
    {
      if (i == 0)
      {
        cutoffValue = map(potiValues[i], 0, 1023, 10, 255);
        int midiCutoffValue = map(potiValues[i], 0, 1023, 0, 127);
        MIDI2.sendControlChange(1, midiCutoffValue, 1);
      }
      else if (i == 1)
      {
        attackValue = map(potiValues[i], 0, 1023, 1, 10);
        int midiAttackValue = map(potiValues[i], 0, 1023, 0, 127);
        MIDI2.sendControlChange(2, midiAttackValue, 1);
      }
      else if (i == 2)
      {
        releaseValue = map(potiValues[i], 0, 1023, 1, 10);
        int midiReleaseValue = map(potiValues[i], 0, 1023, 0, 127);
        MIDI2.sendControlChange(3, midiReleaseValue, 1);
      }
      else if (i == 3)
      {
        delayValue = map(potiValues[i], 0, 1023, 1, 255);
        int midiDelayValue = map(potiValues[i], 0, 1023, 0, 127);
        MIDI2.sendControlChange(4, midiDelayValue, 1);
      }
      prevPotiValues[i] = potiValues[i];
    }
  }
}

void checkButtons()
{

  //CHECK PATTERN
  for (int i = 0; i < maxPattern; i++)
  {
    patternValues[i] = digitalRead(patternButtons[i][0]);

    if (patternValues[i] == LOW && prevPatternValues[i] == HIGH)
    {
      changePattern(i);
      MIDI2.sendNoteOn(i, 127, 1);
    }
    prevPatternValues[i] = patternValues[i];
  }

  //CHECK MODULATION
  for (int j = 0; j < maxModulation; j++)
  {
    modulationValues[j] = digitalRead(modulationButtons[j][0]);

    if (modulationValues[j] == LOW && prevModulationValues[j] == HIGH)
    {
      changeModulation(j);
    }

    prevModulationValues[j] = modulationValues[j];
  }
}

void changePattern(int nextPattern)
{
  resetPixel();
  showPixel();

  for (int k = 0; k < maxPattern; k++)
  {
    digitalWrite(patternButtons[k][1], LOW);
  }
  digitalWrite(patternButtons[nextPattern][1], HIGH);

  currentPattern = nextPattern;
}

void changeModulation(int nextModulation)
{

  if (nextModulation == 0)
  {
    digitalWrite(modulationButtons[0][1], HIGH);
    digitalWrite(modulationButtons[1][1], LOW);
    digitalWrite(modulationButtons[2][1], LOW);
  }
  else if (nextModulation == 1)
  {
    digitalWrite(modulationButtons[0][1], HIGH);
    digitalWrite(modulationButtons[1][1], HIGH);
    digitalWrite(modulationButtons[2][1], LOW);
  }
  else if (nextModulation == 2)
  {
    digitalWrite(modulationButtons[0][1], HIGH);
    digitalWrite(modulationButtons[1][1], HIGH);
    digitalWrite(modulationButtons[2][1], HIGH);
  }

  currentModulation = nextModulation;
}

void showCurrentPattern()
{

  //Serial.println(currentPattern);

  if (currentPattern == 0)
  {
    showPattern1();
  }
  else if (currentPattern == 1)
  {
    showPattern4();
  }
  else if (currentPattern == 2)
  {
    showPattern6();
  }
  else if (currentPattern == 3)
  {
    showPattern3();
  }
  else if (currentPattern == 4)
  {
    showPattern2();
  }
  else if (currentPattern == 5)
  {
    showPattern5();
  }
}

void hideCurrentPattern()
{
  if (currentPattern == 0)
  {
    hidePattern1();
  }
  else if (currentPattern == 1)
  {
    hidePattern4();
  }
  else if (currentPattern == 2)
  {
    hidePattern6();
  }
  else if (currentPattern == 3)
  {
    hidePattern3();
  }
  else if (currentPattern == 4)
  {
    hidePattern2();
  }
  else if (currentPattern == 5)
  {
    hidePattern5();
  }
}

void loop()
{
  now = millis();
  MIDI.read();

  if (now >= nextLightInterval)
  {
    nextLightInterval = now + 10;
    showPixel();
  }

  if (now >= nextPotisInterval)
  {
    nextPotisInterval = now + 100;
    checkPotis();
  }

  if (now >= nextButtonsInterval)
  {
    nextButtonsInterval = now + 100;
    checkButtons();
  }

  if (now >= nextFadeOutInterval)
  {
    if (releaseValue < 10)
    {
      fadeOutPixel();
    }
    nextFadeOutInterval = now + (releaseValue * 13);
  }

  if (now >= nextFadeInInterval)
  {
    if (attackValue > 1)
    {
      fadeInPixel();
    }
    else
    {
      initAttackArray();
    }
    nextFadeInInterval = now + (attackValue * 23);
  }
}
