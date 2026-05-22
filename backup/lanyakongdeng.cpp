#include <Arduino.h>
#include <SoftwareSerial.h>

// 江协科技蓝牙串口控制接线：
// 蓝牙模块 TX -> Arduino D10
// 蓝牙模块 RX -> Arduino D11
// 避免占用 0/1，便于下载程序和串口调试。
constexpr uint8_t BLE_RX_PIN = 10;
constexpr uint8_t BLE_TX_PIN = 11;
constexpr uint8_t LIGHT_PIN = LED_BUILTIN;

constexpr long USB_BAUD = 115200;
constexpr long BLE_BAUD = 9600;

SoftwareSerial bleSerial(BLE_RX_PIN, BLE_TX_PIN);
String commandBuffer;

void setLight(bool isOn) {
  digitalWrite(LIGHT_PIN, isOn ? HIGH : LOW);
  bleSerial.println(isOn ? "LIGHT ON" : "LIGHT OFF");
  Serial.println(isOn ? "LIGHT ON" : "LIGHT OFF");
}

String normalizeCommand(String command) {
  command.trim();
  command.toLowerCase();
  return command;
}

void handleCommand(const String& rawCommand) {
  String command = normalizeCommand(rawCommand);
  if (command.length() == 0) {
    return;
  }

  if (command == "1" || command == "on" || command == "open") {
    setLight(true);
    return;
  }

  if (command == "0" || command == "off" || command == "close") {
    setLight(false);
    return;
  }

  bleSerial.print("UNKNOWN: ");
  bleSerial.println(rawCommand);
  Serial.print("UNKNOWN: ");
  Serial.println(rawCommand);
}

void readBleCommand() {
  while (bleSerial.available() > 0) {
    char ch = static_cast<char>(bleSerial.read());

    if (ch == '\r' || ch == '\n') {
      if (commandBuffer.length() > 0) {
        handleCommand(commandBuffer);
        commandBuffer = "";
      }
      continue;
    }

    commandBuffer += ch;

    // 兼容按钮直接发送单字符且不带换行的配置。
    if (commandBuffer == "1" || commandBuffer == "0") {
      handleCommand(commandBuffer);
      commandBuffer = "";
    }
  }
}

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);

  Serial.begin(USB_BAUD);
  bleSerial.begin(BLE_BAUD);

  Serial.println();
  Serial.println("Bluetooth light controller ready");
  Serial.print("USB baud: ");
  Serial.println(USB_BAUD);
  Serial.print("BLE baud: ");
  Serial.println(BLE_BAUD);
  Serial.println("Commands: 1/on/open, 0/off/close");

  bleSerial.println("Bluetooth light controller ready");
  bleSerial.println("Commands: 1/on/open, 0/off/close");
}

void loop() {
  readBleCommand();
}
