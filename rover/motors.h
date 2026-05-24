#ifndef ROVER_MOTORS_H
#define ROVER_MOTORS_H

#include <Arduino.h>
#include "types.h"

void motorsInit();
void motorsForward();
void motorsReverse();
void motorsTurnLeft();
void motorsTurnRight();
void motorsStop();
void motorsSetSpeed(uint8_t duty);
void motorsApply(DriveCommand cmd, uint8_t speed);

#endif
