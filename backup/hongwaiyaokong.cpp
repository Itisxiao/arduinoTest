#include <Arduino.h>
#include <IRremote.hpp> // 新版库使用 .hpp 后缀


// 定义接收引脚
// VS1838B 的 OUT 引脚连接到这里
// ESP32 常用 GPIO 4, ESP8266 常用 D2 (GPIO 4), Arduino Uno 常用 2 或 11
const int RECV_PIN = 11; 

// --- 自定义协议名称映射函数 ---
// 这样写永远不会有 "undefined" 错误，且不依赖库的版本特性
const char* getMyProtocolName(decode_type_t protocol) {
  switch (protocol) {
    case NEC:           return "NEC";
    case SONY:          return "SONY";
    case RC5:           return "RC5";
    case RC6:           return "RC6";
    case PANASONIC:     return "PANASONIC";
    case JVC:           return "JVC";
    case SAMSUNG:       return "SAMSUNG";
    case LG:            return "LG";
    case WHYNTER:       return "WHYNTER";
    default:            return "UNKNOWN";
  }
}

void setup() {
  Serial.begin(9600); // 初始化串口通信，波特率 9600
  
  // 等待串口连接 (可选，方便查看启动信息)
  while (!Serial); 

  Serial.println(F("=== IR Receiver Test Started ==="));
  Serial.print(F("Listening on pin: "));
  Serial.println(RECV_PIN);

  // 初始化红外接收
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  // 如果不需要板载 LED 反馈，可以将第二个参数改为 DISABLE_LED_FEEDBACK
  // 对于某些开发板，ENABLE_LED_FEEDBACK 可能会占用额外的 LED 引脚，若报错可尝试禁用
}

void loop() {
  // 检查是否收到了完整的红外信号
  if (IrReceiver.decode()) {
    
    // 打印接收到的详细信息
    Serial.print("Protocol: ");
    Serial.println(getMyProtocolName(IrReceiver.decodedIRData.protocol)); 
    
    Serial.print("Address: 0x");
    Serial.println(IrReceiver.decodedIRData.address, HEX);
    
    Serial.print("Command: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    
    Serial.print("Raw Data (bits): ");
    Serial.println(IrReceiver.decodedIRData.numberOfBits);
    
    Serial.println("------------------");

    // --- 在这里添加你的逻辑 ---
    // 例如：如果是 NEC 协议，且命令是 0x10 (假设这是电源键)
    /*
    if (IrReceiver.decodedIRData.protocol == NEC && IrReceiver.decodedIRData.command == 0x10) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // 切换内置 LED
      Serial.println("Toggle LED!");
    }
    */
    
    // 重要：接收完成后必须恢复接收状态，否则只能接收一次
    IrReceiver.resume(); 
  }
}