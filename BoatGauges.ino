#include <TFT_eSPI.h>
#include "gauge1.h"
#include "gauge2.h"
#include "gauge3.h"
#include "gauge4.h"
#include "gauge5.h"
#include "gauge6.h"
#include "font.h"

TFT_eSPI tft;
TFT_eSprite img;

const int pwmLedChannelTFT = 0;

int chosenOne = 0;
int minValue[6] = {0, 20, 0, 0, 0, 80};
int maxValue[6] = {40, 100, 60, 80, 70, 160};

void setup() {
  ledcSetup(pwmLedChannelTFT, 5000, 8);
  ledcAttachPin(5, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 90);

  pinMode(12, INPUT_PULLUP);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_ORANGE);
  img.createSprite(240, 240);
  img.setSwapBytes(true);

  int r = 76;
  int xCenter = 120;
  int yCenter = 120;

  for (int angle = 136; angle != 44; angle = (angle + 1) % 360) {
    float rad = angle * 0.01745;
    int x = r * cos(rad) + xCenter;
    int y = r * sin(rad) + yCenter;
    int x2 = (r - 20) * cos(rad) + xCenter;
    int y2 = (r - 20) * sin(rad) + yCenter;
    img.fillTriangle(x, y, x2, y2, x2, y2, TFT_RED);
  }
}

void loop() {
  if (digitalRead(12) == LOW && ++chosenOne >= 6) chosenOne = 0;
  int result = map(analogRead(14), 0, 4095, minValue[chosenOne], maxValue[chosenOne]);
  int angle = map(result, minValue[chosenOne], maxValue[chosenOne], 0, 267);

  img.pushImage(0, 0, 240, 240, chosenOne == 0 ? gauge1 :
                                chosenOne == 1 ? gauge2 :
                                chosenOne == 2 ? gauge3 :
                                chosenOne == 3 ? gauge4 :
                                chosenOne == 4 ? gauge5 : gauge6);

  if (chosenOne == 5)
    img.drawFloat(result * 0.1, 2, 120, 114);
  else if (chosenOne == 4)
    img.drawString(String(result * 100), 120, 114);
  else
    img.drawString(String(result), 120, 114);

  img.pushSprite(0, 0);
}
