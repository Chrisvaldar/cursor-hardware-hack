#include "config.h"
#include "types.h"
#include "motors.h"
#include "sensors.h"
#include "collision.h"
#include "navigation.h"
#include "telemetry.h"

static uint32_t s_lastSensorMs = 0;
static uint32_t s_lastControlMs = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);  // brief serial monitor connect window at boot only

  motorsInit();
  sensorsInit();
  collisionInit();
  navigationInit();
  telemetryInit();

  g_state.statusMessage = "ready";
  Serial.println("Rover firmware ready.");
}

void loop() {
  const uint32_t now = millis();

  if (now - s_lastSensorMs >= SENSOR_READ_MS) {
    sensorsUpdate();
    g_state.frontCm = readFrontDistance();
    g_state.leftCm = readLeftDistance();
    g_state.rightCm = readRightDistance();
    s_lastSensorMs = now;
  }

  if (now - s_lastControlMs >= CONTROL_TICK_MS) {
    const bool collisionActive = collisionUpdate(now);
    if (!collisionActive) {
      navigationUpdate(now);
    }
    s_lastControlMs = now;
  }

  telemetryUpdate(now);
}
