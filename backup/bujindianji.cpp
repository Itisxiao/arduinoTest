#include <Arduino.h>
#include <AccelStepper.h>

// 定义引脚
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// 创建步进电机对象
// HALF4WIRE 表示 4 线半步模式
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  // 设置最大速度
  stepper.setMaxSpeed(1000);
  // 设置加速度
  stepper.setAcceleration(500);
  // 设置目标位置
  stepper.moveTo(2048);
  
  Serial.begin(9600);
}

void loop() {
  // 如果到达目标位置，改变方向
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition());
  }
  
  // 运行步进电机
  stepper.run();
}