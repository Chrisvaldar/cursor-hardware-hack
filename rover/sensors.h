#ifndef ROVER_SENSORS_H
#define ROVER_SENSORS_H

#include <Arduino.h>

void sensorsInit();
void sensorsUpdate();

float readFrontDistance();
float readLeftDistance();
float readRightDistance();

#endif
