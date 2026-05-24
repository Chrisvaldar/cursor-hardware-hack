#include "drive.h"

const char* driveDirToString(DriveDir dir) {
  switch (dir) {
    case DriveDir::Up:    return "up";
    case DriveDir::Down:  return "down";
    case DriveDir::Left:  return "left";
    case DriveDir::Right: return "right";
  }
  return "stop";
}

bool parseDriveDir(const String& token, DriveDir& out) {
  if (token == "up" || token == "forward") {
    out = DriveDir::Up;
    return true;
  }
  if (token == "down" || token == "reverse" || token == "back") {
    out = DriveDir::Down;
    return true;
  }
  if (token == "left" || token == "steer_left") {
    out = DriveDir::Left;
    return true;
  }
  if (token == "right" || token == "steer_right") {
    out = DriveDir::Right;
    return true;
  }
  if (token == "stop" || token == "none" || token == "center" ||
      token == "steer_center" || token == "steer_stop") {
    out = DriveDir::Stop;
    return true;
  }
  return false;
}
