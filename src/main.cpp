#include <Arduino.h>

// 引脚配置（可改）
const uint8_t TRIG_PIN = 2;
const uint8_t ECHO_PIN = 3;

// 测距参数
const float MAX_DISTANCE_CM = 400.0;
const unsigned long TIMEOUT_US = static_cast<unsigned long>(MAX_DISTANCE_CM * 2.0 / 0.0343);
const unsigned long MEASURE_INTERVAL_MS = 200;

unsigned long lastMeasureTime = 0;

// 滑动平均滤波缓存
constexpr uint8_t FILTER_SIZE = 5;
float filterBuffer[FILTER_SIZE] = {0};
uint8_t filterIndex = 0;

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT_US);
  if (duration == 0) return -1.0; // 超时/无回波
  return duration * 0.01715;      // 0.0343 / 2
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("📡 HC-SR04 测距初始化完成...");
}

void loop() {
  // 非阻塞定时
  if (millis() - lastMeasureTime >= MEASURE_INTERVAL_MS) {
    lastMeasureTime = millis();
    float d = readDistance();

    if (d > 0 && d <= MAX_DISTANCE_CM) {
      filterBuffer[filterIndex] = d;
      filterIndex = (filterIndex + 1) % FILTER_SIZE;

      float sum = 0;
      for (uint8_t i = 0; i < FILTER_SIZE; i++) sum += filterBuffer[i];
      float avgDist = sum / FILTER_SIZE;

      Serial.print("距离: ");
      Serial.print(avgDist, 1);
      Serial.println(" cm");
    } else {
      Serial.println("⚠️ 超出量程或无回波");
    }
  }
}