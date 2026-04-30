#include <Arduino.h>
#include <LedControl.h>

// 引脚定义 (根据实际接线修改)
// DIN -> Data In, CLK -> Clock, CS -> Chip Select (LOAD)
#define DATA_PIN  11
#define CLK_PIN   13
#define CS_PIN    10

// 初始化：参数为 (DIN, CLK, CS, 模块数量)
// 4 合 1 屏幕通常由 4 个 8x8 模块级联组成
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 4);

// 字模数据 (8x8 点阵: 1, 2, 3, 4)
// 每个数组 8 个字节，代表 8 行
const byte digit_1[] = {0x00, 0x00, 0x40, 0xC0, 0x40, 0x40, 0x40, 0xE0};
const byte digit_2[] = {0x00, 0x60, 0x90, 0x90, 0x60, 0xC0, 0xF0, 0x00 };
const byte digit_3[] = {0x00, 0x0E, 0x01, 0x0E, 0x01, 0x01, 0x0E, 0x00};
const byte digit_4[] = {0x00, 0x09, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x00};

// 辅助函数：向指定模块写入一个字模
void displayDigit(int moduleIndex, const byte* digitData) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(moduleIndex, row, digitData[row]);
  }
}

void setup() {
  // 唤醒并初始化所有 4 个模块
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);    // 关闭省电模式 (唤醒)
    lc.setIntensity(i, 8);    // 设置亮度 (0-15)
    lc.clearDisplay(i);       // 清屏
  }

  // 显示 "1234"
  // 注意：模块索引 0 通常是离单片机数据线 (DIN) 最近的那个模块
  // 如果物理顺序反了，请调整下面的索引顺序 (例如把 digit_1 发给模块 3)
  
  displayDigit(0, digit_1); // 第1个模块显示 '1'
  displayDigit(1, digit_2); // 第2个模块显示 '2'
  displayDigit(2, digit_3); // 第3个模块显示 '3'
  displayDigit(3, digit_4); // 第4个模块显示 '4'
}

void loop() {
  // 静态显示，loop 中无需操作
  // 如果需要动态效果，可在此添加代码
}