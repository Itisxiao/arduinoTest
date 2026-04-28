#include <Arduino.h>
#include "kaiguan.h"
#include "effects.h"

void setup() {
  setupEffects();
  pinMode(TOUCH_PIN, INPUT);  // 设置触摸传感器引脚为输入
}

void loop() {
  runKaiEffects();
}
