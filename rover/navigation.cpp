#include "navigation.h"
#include "config.h"
#include "motors.h"
#include "types.h"

static NavPhase s_phase = NavPhase::Wander;
static uint32_t s_phaseEnteredMs = 0;

static void enterPhase(NavPhase phase, uint32_t now, const char* message) {
  s_phase = phase;
  s_phaseEnteredMs = now;
  g_state.navigation = phase;
  g_state.statusMessage = message;
}

static bool phaseElapsed(uint32_t now, uint32_t durationMs) {
  return (now - s_phaseEnteredMs) >= durationMs;
}

static bool isOpenSpace(float left, float right, float front) {
  return left > OPEN_SPACE_CM && right > OPEN_SPACE_CM && front > OPEN_SPACE_CM;
}

void navigationInit() {
  s_phase = NavPhase::Wander;
  s_phaseEnteredMs = 0;
  g_state.navigation = NavPhase::Wander;
}

void navigationUpdate(uint32_t now) {
  const float left = g_state.leftCm;
  const float right = g_state.rightCm;
  const float front = g_state.frontCm;

  const float wallLow = WALL_TARGET_CM - WALL_HYSTERESIS_CM;
  const float wallHigh = WALL_TARGET_CM + WALL_HYSTERESIS_CM;

  switch (s_phase) {
    case NavPhase::Wander:
      if (isOpenSpace(left, right, front)) {
        motorsApply(DriveCommand::Forward, NAV_SPEED);
        g_state.statusMessage = "open space — wandering forward";
        if (left > OPEN_SPACE_CM) {
          enterPhase(NavPhase::SeekWall, now, "no left wall — seeking wall");
        }
        break;
      }

      if (left < wallLow) {
        enterPhase(NavPhase::CorrectRight, now, "too close to left wall");
        break;
      }
      if (left > wallHigh) {
        enterPhase(NavPhase::CorrectLeft, now, "too far from left wall");
        break;
      }

      motorsApply(DriveCommand::Forward, NAV_SPEED);
      g_state.statusMessage = "following left wall";
      break;

    case NavPhase::CorrectLeft:
      motorsApply(DriveCommand::TurnLeft, NAV_TURN_SPEED);
      if (phaseElapsed(now, NAV_CORRECT_MS) || left <= WALL_TARGET_CM) {
        enterPhase(NavPhase::Wander, now, "wall distance corrected");
      }
      break;

    case NavPhase::CorrectRight:
      motorsApply(DriveCommand::TurnRight, NAV_TURN_SPEED);
      if (phaseElapsed(now, NAV_CORRECT_MS) || left >= WALL_TARGET_CM) {
        enterPhase(NavPhase::Wander, now, "wall distance corrected");
      }
      break;

    case NavPhase::SeekWall:
      motorsApply(DriveCommand::TurnLeft, SEEK_WALL_SPEED);
      if (left < OPEN_SPACE_CM || phaseElapsed(now, SEEK_WALL_MS)) {
        enterPhase(NavPhase::Wander, now, "wall acquired");
      }
      break;
  }
}
