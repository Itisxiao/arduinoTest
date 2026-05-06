#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 定义 LCD 对象：地址 0x27，16 列，2 行
LiquidCrystal_I2C lcd(0x27, 16, 2); 
void setup() {
  Wire.begin(); 
  
  lcd.begin(16, 2);      // ✅ 必须传入 列, 行
  lcd.backlight();       // 开启背光
  
  lcd.setCursor(0, 0);
  lcd.print("  PlatformIO  ");
  lcd.setCursor(0, 1);
  lcd.print(" Hello Arduino! ");
}

void loop() {
  // 可以留空，或者在这里写让屏幕闪烁的代码
  delay(1000);
}