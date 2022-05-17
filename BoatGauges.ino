#include <TFT_eSPI.h> 
#include "gauge1.h"
#include "gauge2.h"
#include "gauge3.h"
#include "gauge4.h"
#include "gauge5.h"
#include "gauge6.h"
#include "font.h"
TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite ln = TFT_eSprite(&tft);

double rad=0.01745;
int angle;

int sx=120;
int sy=120;
int r=76;

float x[360];
float y[360];
float x2[360];
float y2[360];

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

int chosenOne=0;
int minValue[6]={0,20,0,0,0,80};
int maxValue[6]={40,100,60,80,70,160};
int dbounce=0;

void setup() {

    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
    ledcAttachPin(5, pwmLedChannelTFT);
    ledcWrite(pwmLedChannelTFT, 90);

    pinMode(12,INPUT_PULLUP);
   
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    img.setSwapBytes(true);
    tft.fillScreen(TFT_ORANGE);
    img.createSprite(240, 240);
    

  tft.setPivot(60,60);
  img.setTextDatum(4);
  img.setTextColor(TFT_BLACK,0xAD55);
  img.setFreeFont(&Orbitron_Medium_28);

  int i=0;
  int a=136;

  while(a!=44)
  {
   x[i]=r*cos(rad*a)+sx;
   y[i]=r*sin(rad*a)+sy;
   x2[i]=(r-20)*cos(rad*a)+sx;
   y2[i]=(r-20)*sin(rad*a)+sy;
   i++;
   a++;
   if(a==360)
   a=0;
  }
}

//min angle 136 or 137
//max angle 43

int a1,a2;
int result=0;

void loop() {

  if(digitalRead(12)==0)
  {
    if(dbounce==0)
       {
        dbounce=1;
        chosenOne++;
        if(chosenOne>=6)
        chosenOne=0;
        }
    }else dbounce=0;
  result=map(analogRead(14),0,4095,minValue[chosenOne],maxValue[chosenOne]);
  angle=map(result,minValue[chosenOne],maxValue[chosenOne],0,267);
  


 if(chosenOne==0)
 img.pushImage(0,0,240,240,gauge1);
  if(chosenOne==1)
 img.pushImage(0,0,240,240,gauge2);
  if(chosenOne==2)
 img.pushImage(0,0,240,240,gauge3);
  if(chosenOne==3)
 img.pushImage(0,0,240,240,gauge4);
  if(chosenOne==4)
 img.pushImage(0,0,240,240,gauge5);
  if(chosenOne==5)
 img.pushImage(0,0,240,240,gauge6);

 if(chosenOne==5)
  img.drawFloat(result/10.00,2,120,114);
 else if(chosenOne==4)
 img.drawString(String(result*100),120,114);
 else
 img.drawString(String(result),120,114);
 //img.drawString(String(analogRead(22)), 30,10,2);

 a1=angle-4;
 a2=angle+4;

 if(a1<0)
 a1=angle-4+359;
  if(a2>=359)
 a2=angle+4-359;

 if(result<=minValue[chosenOne]+4)
 img.fillTriangle(x[angle],y[angle],x2[angle],y2[angle],x2[a2+2],y2[a2+2],TFT_RED);
 else if(result>=maxValue[chosenOne]-4)
 img.fillTriangle(x[angle],y[angle],x2[a1-2],y2[a1-2],x2[angle],y2[angle],TFT_RED);
 else
 img.fillTriangle(x[angle],y[angle],x2[a1],y2[a1],x2[a2],y2[a2],TFT_RED);

 
 img.pushSprite(0, 0);
}
