#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

//majicDesigns/MD_Parola@^3.7.0
//majicDesigns/MD_MAX72xx@^3.5.1

// 引脚定义 (根据实际接线修改)
// DIN -> Data In, CLK -> Clock, CS -> Chip Select (LOAD)
#define DATA_PIN  11
#define CLK_PIN   13
#define CS_PIN    10
#define MAX_DEVICES 4 
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
    P.begin();
    P.displayText("deng", PA_CENTER, 100, 0, PA_PRINT, PA_NO_EFFECT);
}

void loop() {
    P.displayAnimate();
}