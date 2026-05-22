#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 6
#define DHTTYPE DHT11

// 蓝牙串口（软件串口）
// Arduino RX = Pin 10, Arduino TX = Pin 11
// 不要占用 0/1，这两个引脚是硬件串口，上传程序时会用到。
SoftwareSerial bleSerial(10, 11); // RX, TX
DHT dht(DHTPIN, DHTTYPE);

constexpr long USB_BAUD = 115200;
constexpr long BLE_BAUD = 9600;
constexpr unsigned long SEND_INTERVAL_MS = 2000;
unsigned long lastSendAt = 0;

void setup() {
  Serial.begin(USB_BAUD);
  bleSerial.begin(BLE_BAUD);
  dht.begin();

  Serial.println();
  Serial.println("RTL8762AG DHT11 JSON");
  Serial.print("USB baud: ");
  Serial.println(USB_BAUD);
  Serial.print("BLE baud: ");
  Serial.println(BLE_BAUD);
  Serial.println("Sending DHT11 JSON every 2 seconds.");
}

void loop() {
  while (bleSerial.available() > 0) {
    Serial.write(bleSerial.read());
  }

  if (millis() - lastSendAt >= SEND_INTERVAL_MS) {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("DHT11 read failed");
      lastSendAt = millis();
      return;
    }

    String payload = "{\"temp\":" + String(temperature, 1) +
                     ",\"hum\":" + String(humidity, 1) + "}";

    bleSerial.println(payload);
    Serial.print("Sent: ");
    Serial.println(payload);
    lastSendAt = millis();
  }
}
