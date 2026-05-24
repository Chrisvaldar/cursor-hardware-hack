// Minimal servo test — ONE file, no other modules.
// Wire: signal → SERVO_PIN, power → 5V (external supply recommended), GND → GND

#include <Arduino.h>
#include <ESP32Servo.h>

// Change this to match your wiring. Avoid motor pins in rover/config.h (12,14,26,27,32,33).
#define SERVO_PIN 13

Servo servo;

void setup() {
  Serial.begin(115200);
  delay(500);

  // ESP32 allows attaching servos to most GPIO pins
  servo.setPeriodHertz(50);
  if (!servo.attach(SERVO_PIN, 500, 2400)) {
    Serial.println("ERROR: servo attach failed — check pin number");
    return;
  }

  Serial.println("Servo test running.");
  Serial.println("Sweeping 0 -> 180 -> 90. Watch the horn move.");
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2) {
    servo.write(angle);
    Serial.printf("angle %d\n", angle);
    delay(15);
  }
  delay(500);

  for (int angle = 180; angle >= 0; angle -= 2) {
    servo.write(angle);
    Serial.printf("angle %d\n", angle);
    delay(15);
  }
  delay(500);

  servo.write(90);
  Serial.println("center 90");
  delay(2000);
}
