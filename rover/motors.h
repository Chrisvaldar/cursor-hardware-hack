#ifndef ROVER_MOTORS_H
#define ROVER_MOTORS_H

#include <Arduino.h>

void motorsInit();
void motorsStop();
void motorsForward(uint8_t speed);
void motorsBack(uint8_t speed);
void motorsLeft(uint8_t speed);
void motorsRight(uint8_t speed);

#endif
