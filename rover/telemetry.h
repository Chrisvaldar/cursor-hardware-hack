#ifndef ROVER_TELEMETRY_H
#define ROVER_TELEMETRY_H

#include <Arduino.h>

void telemetryInit();
void telemetryUpdate(uint32_t now);

#endif
