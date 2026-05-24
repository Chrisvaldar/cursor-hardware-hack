#include "types.h"

RoverState g_state = {
  100.0f, 100.0f, 100.0f,
  DriveCommand::Stop,
  0,
  CollisionPhase::Idle,
  NavPhase::Wander,
  0,
  "boot"
};

const char* driveCommandToString(DriveCommand cmd) {
  switch (cmd) {
    case DriveCommand::Stop:      return "stop";
    case DriveCommand::Forward:   return "forward";
    case DriveCommand::Reverse:   return "reverse";
    case DriveCommand::TurnLeft:  return "turn_left";
    case DriveCommand::TurnRight: return "turn_right";
  }
  return "unknown";
}

const char* collisionPhaseToString(CollisionPhase phase) {
  switch (phase) {
    case CollisionPhase::Idle:     return "idle";
    case CollisionPhase::Stop:     return "stop";
    case CollisionPhase::Evaluate: return "evaluate";
    case CollisionPhase::Turn:     return "turn";
    case CollisionPhase::Resume:   return "resume";
  }
  return "unknown";
}

const char* navPhaseToString(NavPhase phase) {
  switch (phase) {
    case NavPhase::Wander:       return "wander";
    case NavPhase::CorrectLeft:  return "correct_left";
    case NavPhase::CorrectRight: return "correct_right";
    case NavPhase::SeekWall:     return "seek_wall";
  }
  return "unknown";
}
