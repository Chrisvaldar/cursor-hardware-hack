#ifndef ROVER_NAVIGATION_H
#define ROVER_NAVIGATION_H

#include <Arduino.h>

void navigationInit();
void navigationUpdate(uint32_t now);

#endif
