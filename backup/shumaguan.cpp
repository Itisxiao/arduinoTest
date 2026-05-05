#include <Arduino.h>
#include <TM1637Display.h> // 确认是这个头文件

// 引脚定义
#define CLK_PIN  3
#define DIO_PIN  2

// 初始化对象
TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  // 设置亮度 (0x00 - 0x0f)
  display.setBrightness(0x0f); 
  display.clear();
}

void loop() {
  // --- 显示 1234 ---
  // 参数：(数字，是否显示前导零，有效位数)
  display.showNumberDec(1234, false, 4);
  
  delay(2000);

  // --- 显示带小数点的 12.34 ---
  // 参数：(数字，小数点掩码，是否前导零，有效位数)
  // 0x02 表示在第 2 位后面显示小数点 (二进制 0010)
  display.showNumberDecEx(1234, 0x02, false, 4);
  
  delay(2000);

  // --- 清屏 ---
  display.clear();
  delay(1000);
}