#include <TFT_eSPI.h>
#include "gauge1.h"
#include "gauge2.h"
#include "gauge3.h"
#include "gauge4.h"
#include "gauge5.h"
#include "gauge6.h"
#include "font.h"

TFT_eSPI tft = TFT_eSPI();           // invoke tft library
TFT_eSprite img = TFT_eSprite(&tft); // sprite for gauge
TFT_eSprite ln = TFT_eSprite(&tft);  // sprite for ??

double rad = 0.01745; // constant: number of radian per degree
int angle;

// circle constants (screen position and size)
int sx = 120; // x position
int sy = 120; // y position
int r = 76;   // radius

// array for storing bounding box coordinates for every degree alog the circle
float x[360];
float y[360];
float x2[360];
float y2[360];

// ??
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

// gauge params
int chosenOne = 0;                              // selected gauge index
int minValue[6] = {  0,  20,  0,  0,  0,  80 }; // min values for different gauges
int maxValue[6] = { 40, 100, 60, 80, 70, 160 }; // max values for different gauges

int dbounce = 0; // push button debounce param

void setup() {
  // ??
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(5, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 90);

  pinMode(12, INPUT_PULLUP); // button

  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);
  img.setSwapBytes(true);

  // set background color, image
  tft.fillScreen(TFT_ORANGE);
  img.createSprite(240, 240);

  // set gauge text position, size, color, font
  tft.setPivot(60, 60);
  img.setTextDatum(4);
  img.setTextColor(TFT_BLACK, 0xAD55);
  img.setFreeFont(&Orbitron_Medium_28);

  // while loop params
  int i = 0;   // loop counter
  int a = 136; // starting angle in degrees

  // while loop sweeps an arc along the circle for every degree to calculate
  // positions for bounding boxes that will be used to position red triangle indicator
  // sweep starts from 136 degrees and moves to 360 degrees
  // then resets (as circle only has 360 degrees) to 0 degrees and continues to 44 degrees
  // for a total of 268 degree arc
  while (a != 44) {
    x[i] = r * cos(rad * a) + sx;
    y[i] = r * sin(rad * a) + sy;
    x2[i] = (r - 20) * cos(rad * a) + sx;
    y2[i] = (r - 20) * sin(rad * a) + sy;

    i++;
    a++;

    // reset "a" on overflow
    if (a == 360) {
      a = 0;
    }
  }
}

//min angle 136 or 137
//max angle 43

int a1, a2;     // min/max angles for red triangle bounding box
int result = 0; // potentiometer value

void loop() {
  // check button state with debounce logic
  if (digitalRead(12) == 0) {
    if (dbounce == 0) {
      dbounce = 1;
      chosenOne++; // select next gauge

      // reset gauge index on overflow
      if (chosenOne >= 6) {
        chosenOne = 0;
      }
    }
  } else {
    dbounce = 0;
  }

  // read potentiometer value and map it for selected gauge min and max
  result = map(analogRead(14), 0, 4095, minValue[chosenOne], maxValue[chosenOne]);

  // map obtained result to corresponding angle on a circle
  angle = map(result, minValue[chosenOne], maxValue[chosenOne], 0, 267);

  // push currently selected gauge image to "img" sprite
  if (chosenOne == 0) {
    img.pushImage(0, 0, 240, 240, gauge1);
  }

  if (chosenOne == 1) {
    img.pushImage(0, 0, 240, 240, gauge2);
  }

  if (chosenOne == 2) {
    img.pushImage(0, 0, 240, 240, gauge3);
  }

  if (chosenOne == 3) {
    img.pushImage(0, 0, 240, 240, gauge4);
  }

  if (chosenOne == 4) {
    img.pushImage(0, 0, 240, 240, gauge5);
  }

  if (chosenOne == 5) {
    img.pushImage(0, 0, 240, 240, gauge6);
  }

  // push measured value to "img" sprite
  if (chosenOne == 5) {
    img.drawFloat(result / 10.00, 2, 120, 114);
  } else if (chosenOne == 4) {
    img.drawString(String(result * 100), 120, 114);
  }  else {
    img.drawString(String(result), 120, 114);
  }

  // debug: push raw potentiometer ADC value to "img" sprite
  //img.drawString(String(analogRead(22)), 30, 10, 2);

  // set min & max bounding box angles
  a1 = angle - 4;
  a2 = angle + 4;

  // check angle limits (end stops)
  if (a1 < 0) {
    a1 = angle - 4 + 359; // rotate angle clockwise
  }

  if (a2 >= 359) {
    a2 = angle + 4 - 359; // rotate angle counter clockwise
  }

  // draw red triangle corresponding to measure value on "img" sprite
  if (result <= minValue[chosenOne] + 4) {
    // draw triangle on the left end stop
    img.fillTriangle(x[angle], y[angle], x2[angle], y2[angle], x2[a2 + 2], y2[a2 + 2], TFT_RED);
  } else if (result >= maxValue[chosenOne] - 4) {
    // draw triangle on the right end stop
    img.fillTriangle(x[angle], y[angle], x2[a1 - 2], y2[a1 - 2], x2[angle], y2[angle], TFT_RED);
  } else {
    img.fillTriangle(x[angle], y[angle], x2[a1], y2[a1], x2[a2], y2[a2], TFT_RED);
  }

  // push resulting gauge sprite (gauge image + measure value + red triangle needle) to display
  img.pushSprite(0, 0);
}
