#include "effects.h"

// 定义LED引脚
#define LED_PIN_RED 9
#define LED_PIN_GREEN 3
#define LED_PIN_BLUE 6

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(LED_PIN_RED, red);
  analogWrite(LED_PIN_GREEN, green);
  analogWrite(LED_PIN_BLUE, blue);
}

void fadeColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int intensity = 0; intensity <= 255; intensity++) {
    setColor(red ? intensity : 0,
             green ? intensity : 0,
             blue ? intensity : 0);
    delay(4);
  }
}

void setupEffects() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
}

void runEffects() {
  fadeColor(1, 0, 0); // 红色渐变
  fadeColor(0, 1, 0); // 绿色渐变
  fadeColor(0, 0, 1); // 蓝色渐变
}
