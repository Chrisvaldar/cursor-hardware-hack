#include "motors.h"
#include "config.h"

// L293D truth table (per channel):
//   IN1  IN2  EN   Result
//   LOW  LOW  *    Coast / stop
//   HIGH LOW  PWM  Forward
//   LOW  HIGH PWM  Reverse
//   HIGH HIGH *    Brake (we avoid this; use LOW LOW instead)
//
// PWM: ESP32 Arduino 3.x uses ledcAttach(pin, ...); 2.x uses ledcSetup + ledcAttachPin.

static uint8_t s_currentSpeed = 0;

static void setMotorA(bool forward) {
  digitalWrite(MOTOR_A_IN1, forward ? HIGH : LOW);
  digitalWrite(MOTOR_A_IN2, forward ? LOW : HIGH);
}

static void setMotorB(bool forward) {
  digitalWrite(MOTOR_B_IN1, forward ? HIGH : LOW);
  digitalWrite(MOTOR_B_IN2, forward ? LOW : HIGH);
}

static void writePwm(uint8_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(MOTOR_A_EN, duty);
  ledcWrite(MOTOR_B_EN, duty);
#else
  ledcWrite(PWM_CHANNEL_A, duty);
  ledcWrite(PWM_CHANNEL_B, duty);
#endif
  s_currentSpeed = duty;
}

void motorsInit() {
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(MOTOR_A_EN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(MOTOR_B_EN, PWM_FREQ, PWM_RESOLUTION);
#else
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);
  ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_A_EN, PWM_CHANNEL_A);
  ledcAttachPin(MOTOR_B_EN, PWM_CHANNEL_B);
#endif

  motorsStop();
}

void motorsSetSpeed(uint8_t duty) {
  writePwm(duty);
}

void motorsForward() {
  setMotorA(true);
  setMotorB(true);
}

void motorsReverse() {
  setMotorA(false);
  setMotorB(false);
}

void motorsTurnLeft() {
  setMotorA(false);
  setMotorB(true);
}

void motorsTurnRight() {
  setMotorA(true);
  setMotorB(false);
}

void motorsStop() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);
  writePwm(0);
}

void motorsApply(DriveCommand cmd, uint8_t speed) {
  switch (cmd) {
    case DriveCommand::Stop:
      motorsStop();
      break;
    case DriveCommand::Forward:
      motorsForward();
      motorsSetSpeed(speed);
      break;
    case DriveCommand::Reverse:
      motorsReverse();
      motorsSetSpeed(speed);
      break;
    case DriveCommand::TurnLeft:
      motorsTurnLeft();
      motorsSetSpeed(speed);
      break;
    case DriveCommand::TurnRight:
      motorsTurnRight();
      motorsSetSpeed(speed);
      break;
  }

  g_state.drive = cmd;
  g_state.speed = (cmd == DriveCommand::Stop) ? 0 : speed;
}
