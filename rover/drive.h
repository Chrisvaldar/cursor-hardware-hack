#ifndef ROVER_DRIVE_H
#define ROVER_DRIVE_H

#include <Arduino.h>

// D-pad / remote direction (matches dashboard ?dir= and action= aliases)
enum class DriveDir : uint8_t {
  Stop = 0,
  Up,
  Down,
  Left,
  Right
};

const char* driveDirToString(DriveDir dir);
bool parseDriveDir(const String& token, DriveDir& out);

#endif
