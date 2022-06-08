# BoatGauges
Example sketch for six different boat gauges namely:
1. Coolant pressure (psi)
2. Coolant temp (degree C)
3. Oil pressure (psi)
4. Speedo (km/h)
5. Tacho (rpm)
6. Battery voltage


For live demonstration visit [https://www.youtube.com/watch?v=y_H7HM0oyoo]

# Arduino IDE Setup
1. Install ESP32 boards library [guide from randomnerdtutorials](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
2. Install TFT_eSPI library through Library Manager in Arduino IDE [arduino reference](https://www.arduino.cc/reference/en/libraries/tft_espi/)

# Cofigure TFT_eSPI library
Open ``..\Documents\Arduino\libraries\TFT_eSPI\User_Setup.h`` and comment out line 44:
```
//#define ILI9341_DRIVER
```
On line 64 uncomment GC9A01 driver:
```
#define GC9A01_DRIVER
```

Open ``..\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h`` and comment out line 24:
```
//#include <User_Setups/Setup1_ILI9341.h>
```
On line 105 uncomment GC9A01 setup:
```
#include <User_Setups/Setup200_GC9A01.h>
```

Open ``..\Documents\Arduino\libraries\TFT_eSPI\User_Setups\Setup200_GC9A01.h`` and update pin assignments:
```
#define TFT_MISO 5
#define TFT_MOSI 2
#define TFT_SCLK 15
#define TFT_CS   17  // Chip select control pin
#define TFT_DC   16  // Data Command control pin
#define TFT_RST  4   // Reset pin (could connect to Arduino RESET pin)
```

# ESP32 & GC9A01 Connections

![image](https://user-images.githubusercontent.com/6739564/172683255-a640ba47-1d2f-4fc0-b8f3-acbada262f81.png)
> Image is curtesy of WOKWI.com and some MS Paint work
