#ifndef ROVER_COLLISION_H
#define ROVER_COLLISION_H

#include <Arduino.h>

void collisionInit();

// Returns true when collision module owns motor control (navigation must yield).
bool collisionUpdate(uint32_t now);

#endif
