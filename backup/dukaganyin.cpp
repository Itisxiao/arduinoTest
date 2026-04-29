#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

// 定义引脚 (根据你的接线)
#define RST_PIN         9     // 接 RC522 的 RST
#define SS_PIN          10    // 接 RC522 的 SDA (SS)

// 创建实例
MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(9600); // 初始化串口，波特率 9600
  while (!Serial);    // 等待串口连接 (某些板子需要)
  
  SPI.begin();        // 初始化 SPI 总线
  mfrc522.PCD_Init(); // 初始化 MFRC522 读卡器
  
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  
  // 检查读卡器是否连接成功
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (version == 0x00 || version == 0xFF) {
    Serial.println(F("FAILURE: Cannot find MFRC522 chip!"));
    Serial.println(F("Please check wiring (especially 3.3V and GND)."));
    while (true); // 卡死在这里，提示检查接线
  } else {
    Serial.print(F("MFRC522 Version: 0x"));
    Serial.println(version, HEX);
    Serial.println(F("System Ready. Waiting for card..."));
  }
}

void loop() {
  // 检测是否有新卡片进入磁场
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // 选择一张卡片并读取其序列号 (UID)
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // 打印 UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();

  // 打印卡片类型
  Serial.print(F("Card type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // 停止与当前卡片的通信
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(1000); // 稍微延时，避免重复读取
}
