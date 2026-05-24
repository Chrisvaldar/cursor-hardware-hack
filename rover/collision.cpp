#include "collision.h"
#include "config.h"
#include "motors.h"
#include "types.h"

static CollisionPhase s_phase = CollisionPhase::Idle;
static uint32_t s_phaseEnteredMs = 0;
static DriveCommand s_turnCommand = DriveCommand::TurnRight;

static void enterPhase(CollisionPhase phase, uint32_t now, const char* message) {
  s_phase = phase;
  s_phaseEnteredMs = now;
  g_state.collision = phase;
  g_state.statusMessage = message;
}

static bool phaseElapsed(uint32_t now, uint32_t durationMs) {
  return (now - s_phaseEnteredMs) >= durationMs;
}

void collisionInit() {
  s_phase = CollisionPhase::Idle;
  s_phaseEnteredMs = 0;
  g_state.collision = CollisionPhase::Idle;
}

bool collisionUpdate(uint32_t now) {
  const float front = g_state.frontCm;
  const float left = g_state.leftCm;
  const float right = g_state.rightCm;

  switch (s_phase) {
    case CollisionPhase::Idle:
      if (front < OBSTACLE_THRESHOLD_CM) {
        motorsApply(DriveCommand::Stop, 0);
        enterPhase(CollisionPhase::Stop, now, "obstacle ahead — stopping");
        return true;
      }
      return false;

    case CollisionPhase::Stop:
      motorsApply(DriveCommand::Stop, 0);
      if (phaseElapsed(now, STOP_SETTLE_MS)) {
        enterPhase(CollisionPhase::Evaluate, now, "choosing turn direction");
      }
      return true;

    case CollisionPhase::Evaluate: {
      const float leftScore = left - CLEARANCE_MARGIN_CM;
      const float rightScore = right - CLEARANCE_MARGIN_CM;
      if (leftScore >= rightScore) {
        s_turnCommand = DriveCommand::TurnLeft;
      } else {
        s_turnCommand = DriveCommand::TurnRight;
      }
      motorsApply(s_turnCommand, COLLISION_TURN_SPEED);
      enterPhase(CollisionPhase::Turn, now, "turning away from obstacle");
      return true;
    }

    case CollisionPhase::Turn:
      motorsApply(s_turnCommand, COLLISION_TURN_SPEED);
      if (front >= OBSTACLE_THRESHOLD_CM && phaseElapsed(now, TURN_DURATION_MS)) {
        enterPhase(CollisionPhase::Resume, now, "path clear — resuming");
      } else if (phaseElapsed(now, TURN_DURATION_MS * 2)) {
        enterPhase(CollisionPhase::Resume, now, "turn timeout — resuming");
      }
      return true;

    case CollisionPhase::Resume:
      motorsApply(DriveCommand::Forward, RESUME_SPEED);
      if (phaseElapsed(now, RESUME_CREEP_MS)) {
        enterPhase(CollisionPhase::Idle, now, "collision clear");
        return false;
      }
      return true;

    default:
      return false;
  }
}
