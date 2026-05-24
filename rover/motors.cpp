#include "motors.h"
#include "config.h"
#include "state.h"

static void writeEnablePwm(uint8_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(MOTOR_RIGHT_EN, duty);
  ledcWrite(MOTOR_LEFT_EN, duty);
#else
  ledcWrite(PWM_CHANNEL_A, duty);
  ledcWrite(PWM_CHANNEL_B, duty);
#endif
}

static void setLeftSide(int8_t dir) {
  if (dir > 0) {
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
  } else if (dir < 0) {
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    digitalWrite(MOTOR_LEFT_IN1, LOW);
  } else {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
  }
}

static void setRightSide(int8_t dir) {
  if (dir > 0) {
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
  } else if (dir < 0) {
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
  } else {
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
  }
}

static void stopPins() {
  setLeftSide(0);
  setRightSide(0);
}

void motorsInit() {
  pinMode(MOTOR_RIGHT_IN1, OUTPUT);
  pinMode(MOTOR_RIGHT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(MOTOR_RIGHT_EN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(MOTOR_LEFT_EN, PWM_FREQ, PWM_RESOLUTION);
#else
  pinMode(MOTOR_RIGHT_EN, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);
  ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_RIGHT_EN, PWM_CHANNEL_A);
  ledcAttachPin(MOTOR_LEFT_EN, PWM_CHANNEL_B);
#endif

  stopPins();
  writeEnablePwm(0);
}

void motorsStop() {
  stopPins();
  writeEnablePwm(0);
  g_state.speed = 0;
}

void motorsForward(uint8_t speed) {
  setLeftSide(1);
  setRightSide(1);
  writeEnablePwm(speed);
  g_state.speed = speed;
}

void motorsBack(uint8_t speed) {
  setLeftSide(-1);
  setRightSide(-1);
  writeEnablePwm(speed);
  g_state.speed = speed;
}

void motorsLeft(uint8_t speed) {
  setLeftSide(-1);
  setRightSide(1);
  writeEnablePwm(speed);
  g_state.speed = speed;
}

void motorsRight(uint8_t speed) {
  setLeftSide(1);
  setRightSide(-1);
  writeEnablePwm(speed);
  g_state.speed = speed;
}
