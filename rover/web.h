#ifndef ROVER_WEB_H
#define ROVER_WEB_H

#include <Arduino.h>

void webInit();
void webUpdate(uint32_t now);
void remoteControlExtend(uint32_t now);
bool remoteControlActive(uint32_t now);
void remoteDriveMaintain(uint32_t now);

#endif
