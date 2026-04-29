#include <Arduino.h>
#include <LedControl.h>

// --- 配置引脚 ---
#define DATA_PIN  11
#define CLK_PIN   13
#define CS_PIN    3
#define NUM_DEVICES 4

// --- 全局对象 ---
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, NUM_DEVICES);
const char* message = "hello word"; 
const int scrollSpeed = 150; 

// --- 函数声明 (解决作用域问题) ---
uint8_t getFontPixel(char c, int colIdx);
void renderFrame(int offset);

void setup() {
  // 初始化所有芯片
  for (int i = 0; i < NUM_DEVICES; i++) {
    lc.shutdown(i, false);      // 唤醒
    lc.setIntensity(i, 8);      // 亮度 (0-15)
    lc.clearDisplay(i);         // 清屏
  }
}

void loop() {
  int len = strlen(message);
  int totalWidth = len * 6; // 每个字符占 6 列 (5 数据 + 1 间隔)
  
  // 滚动范围：从屏幕右侧外 -> 到屏幕左侧外
  // 起始位置：NUM_DEVICES * 8 (完全在右侧外面)
  // 结束位置：-totalWidth (完全在左侧外面)
  for (int offset = NUM_DEVICES * 8; offset >= -totalWidth; offset--) {
    renderFrame(offset);
    delay(scrollSpeed);
  }
  
  delay(1000); // 滚动结束后暂停 1 秒
}

/**
 * 渲染单帧画面
 * @param offset 滚动偏移量
 */
void renderFrame(int offset) {
  // 遍历每个芯片 (0 到 3)
  for (int dev = 0; dev < NUM_DEVICES; dev++) {
    // 遍历每一行 (0 到 7)
    for (int row = 0; row < 8; row++) {
      byte rowData = 0;
      
      // 遍历该芯片的每一列 (0 到 7)
      for (int col = 0; col < 8; col++) {
        // 计算当前像素在整个长条屏幕上的绝对坐标
        int globalCol = (dev * 8) + col;
        
        // 应用滚动偏移
        int charCol = globalCol + offset; 
        
        // 只有当坐标在有效范围内才计算
        if (charCol >= 0) {
          int charIndex = charCol / 6;       // 第几个字符
          int colInChar = charCol % 6;       // 字符内的第几列
          
          // 【修复警告】：将 strlen 结果转为 int 进行比较，避免有符号/无符号混用
          int msgLen = (int)strlen(message);
          
          if (charIndex < msgLen) {
            char c = message[charIndex];
            uint8_t pixelByte = getFontPixel(c, colInChar);
            
            // 检查当前行是否点亮
            // 字体定义中：高位 (0x80) 对应第 0 行
            if (pixelByte & (0x80 >> row)) { 
              // 设置 rowData 的对应位 (高位对应左边的列)
              rowData |= (0x80 >> col); 
            }
          }
        }
      }
      // 写入该行数据到具体的芯片
      lc.setRow(dev, row, rowData);
    }
  }
}

/**
 * 获取简易字体点阵数据
 * 返回一个字节，代表该字符某一列的垂直像素分布
 */
uint8_t getFontPixel(char c, int colIdx) {
  if (colIdx == 5) return 0x00; // 第 6 列是间隔，全灭
  if (colIdx > 5) return 0x00;  // 安全防御
  
  switch(c) {
    case 'h': { const uint8_t f[] = {0x08, 0x08, 0x08, 0x08, 0x0F}; return f[colIdx]; }
    case 'e': { const uint8_t f[] = {0x0A, 0x0A, 0x0A, 0x0A, 0x02}; return f[colIdx]; }
    case 'l': { const uint8_t f[] = {0x08, 0x08, 0x08, 0x08, 0x08}; return f[colIdx]; }
    case 'o': { const uint8_t f[] = {0x04, 0x0A, 0x0A, 0x0A, 0x04}; return f[colIdx]; }
    case 'w': { const uint8_t f[] = {0x08, 0x08, 0x08, 0x08, 0x05}; return f[colIdx]; } // 简化版 w
    case 'r': { const uint8_t f[] = {0x08, 0x08, 0x08, 0x0A, 0x04}; return f[colIdx]; }
    case 'd': { const uint8_t f[] = {0x02, 0x0A, 0x0A, 0x0A, 0x06}; return f[colIdx]; }
    default: return 0x00;
  }
}