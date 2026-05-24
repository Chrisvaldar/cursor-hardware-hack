#ifndef ROVER_STATE_H
#define ROVER_STATE_H

#include <Arduino.h>

struct RoverState {
  uint8_t speed;
  unsigned long uptimeMs;
  const char* message;
};

extern RoverState g_state;

#endif
