#include "kaiguan.h"
#include "effects.h"

static int colorState = 0;  // 0: 红色, 1: 绿色, 2: 蓝色
static bool lastTouchState = LOW;

void runKaiEffects() {
    bool currentTouch = digitalRead(TOUCH_PIN);
    
    // 检测触摸上升沿（从 LOW 到 HIGH）
    if (currentTouch == HIGH && lastTouchState == LOW) {
        colorState = (colorState + 1) % 3;  // 切换到下一个颜色
    }
    
    lastTouchState = currentTouch;
    
    // 根据状态设置颜色
    if (colorState == 0) {
        setColor(255, 0, 0);  // 红色
    } else if (colorState == 1) {
        setColor(0, 255, 0);  // 绿色
    } else {
        setColor(0, 0, 255);  // 蓝色
    }
}