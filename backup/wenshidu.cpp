#include <Arduino.h>
#include <DHT.h> // 引入新的库

// 配置引脚和传感器类型
#define DHTPIN 6    // 数据引脚连接到 D2
#define DHTTYPE DHT11 // 定义传感器型号为 DHT11

// 初始化 DHT 对象
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 Test - Adafruit Library")); // F()宏用于节省内存

  // 启动传感器
  dht.begin();
}

void loop() {
  // 1. 读取湿度
  float h = dht.readHumidity();
  // 2. 读取温度 (摄氏度)
  float t = dht.readTemperature();
  // 3. 读取温度 (华氏度，如果不需可忽略)
  // float f = dht.readTemperature(true); 

  // === 关键：错误检查 ===
  // 如果读到的值是 NaN (Not a Number)，说明读取失败
  if (isnan(h) || isnan(t)) {
    Serial.println(F("读取失败！请检查接线或传感器是否损坏。"));
    return; // 结束本次循环，等待下次
  }

  // === 输出结果 ===
  Serial.print("湿度: ");
  Serial.print(h, 1); // 保留1位小数
  Serial.print("%\t | 温度: ");
  Serial.print(t, 1);
  Serial.println("°C");

  // 间隔 2 秒再次读取 (DHT11 最佳采样间隔)
  delay(2000);
}