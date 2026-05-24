#ifndef ROVER_TYPES_H
#define ROVER_TYPES_H

#include <Arduino.h>

enum class DriveCommand : uint8_t {
  Stop,
  Forward,
  Reverse,
  TurnLeft,
  TurnRight
};

enum class CollisionPhase : uint8_t {
  Idle,
  Stop,
  Evaluate,
  Turn,
  Resume
};

enum class NavPhase : uint8_t {
  Wander,
  CorrectLeft,
  CorrectRight,
  SeekWall
};

struct RoverState {
  float frontCm;
  float leftCm;
  float rightCm;
  DriveCommand drive;
  uint8_t speed;
  CollisionPhase collision;
  NavPhase navigation;
  unsigned long uptimeMs;
  const char* statusMessage;
};

extern RoverState g_state;

const char* driveCommandToString(DriveCommand cmd);
const char* collisionPhaseToString(CollisionPhase phase);
const char* navPhaseToString(NavPhase phase);

#endif
