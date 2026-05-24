#include "config.h"
#include "state.h"
#include "motors.h"
#include "web.h"

static uint32_t s_lastControlMs = 0;

void setup() {
  setCpuFrequencyMhz(80);

  motorsInit();
  webInit();

  g_state.message = "stopped — hold D-pad to drive";
}

void loop() {
  const uint32_t now = millis();

  webUpdate(now);

  if (now - s_lastControlMs >= CONTROL_TICK_MS) {
    remoteDriveMaintain(now);
    s_lastControlMs = now;
  }
}
