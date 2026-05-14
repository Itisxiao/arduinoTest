#include <Arduino.h>

#include <Keypad.h>

const byte ROWS = 4; // 4行
const byte COLS = 4; // 4列


char keys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};
// 配合：
byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {11, 10, 9, 8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
}