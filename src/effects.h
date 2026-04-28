#ifndef EFFECTS_H
#define EFFECTS_H

#include <Arduino.h>

void setupEffects();
void runEffects();
void setColor(uint8_t red, uint8_t green, uint8_t blue);  // 新增声明
void fadeColor(uint8_t red, uint8_t green, uint8_t blue); // 新增声明

#endif // EFFECTS_H
