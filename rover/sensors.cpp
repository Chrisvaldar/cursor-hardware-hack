#include "sensors.h"
#include "config.h"

// Cached readings refreshed by sensorsUpdate() — logic layer reads these only.
static float s_frontCm = 100.0f;
static float s_leftCm = 100.0f;
static float s_rightCm = 100.0f;

// -----------------------------------------------------------------------------
// TEAMMATE HOOK: implement hardware reads below.
// Return distance in centimetres. Use a large value (e.g. 100) when no obstacle.
// -----------------------------------------------------------------------------

static float readDistanceStub(const char* /*label*/) {
  // Replace with real sensor code, e.g.:
  //
  // Ultrasonic (HC-SR04 style):
  //   digitalWrite(TRIG_PIN, LOW);
  //   delayMicroseconds(2);  // OK in sensor read path if < 30us total blocking
  //   digitalWrite(TRIG_PIN, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(TRIG_PIN, LOW);
  //   long us = pulseIn(ECHO_PIN, HIGH, 30000);
  //   return us * 0.0343f / 2.0f;
  //
  // IR distance (ADC): map analogRead(pin) to cm via your calibration curve.
  // Camera: return nearest obstacle distance from vision pipeline.

  return 100.0f;
}

void sensorsInit() {
  // TEAMMATE HOOK: pinMode(), I2C begin, camera init, etc.
}

void sensorsUpdate() {
  s_frontCm = readDistanceStub("front");
  s_leftCm = readDistanceStub("left");
  s_rightCm = readDistanceStub("right");

  // Or call separate functions per sensor:
  // s_frontCm = readUltrasonicFront();
  // s_leftCm  = readIrLeft();
  // s_rightCm = readIrRight();
}

float readFrontDistance() {
  return s_frontCm;
}

float readLeftDistance() {
  return s_leftCm;
}

float readRightDistance() {
  return s_rightCm;
}
