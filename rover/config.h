#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

// L293D tank drive — see docs/l293d_schematic.html
#define MOTOR_RIGHT_IN1  25
#define MOTOR_RIGHT_IN2  33
#define MOTOR_RIGHT_EN   14
#define MOTOR_LEFT_IN1   18
#define MOTOR_LEFT_IN2   19
#define MOTOR_LEFT_EN    13

// PWM 0–255. Keep low to avoid USB/battery brownout resets.
#define DRIVE_SPEED       72
#define TURN_SPEED        48

#define MANUAL_CONTROL_MS 2500
#define CONTROL_TICK_MS   40

#define PWM_FREQ        5000
#define PWM_RESOLUTION  8
#define PWM_CHANNEL_A   0
#define PWM_CHANNEL_B   1

#define AP_SSID         "RoverTelemetry"
#define AP_PASSWORD     "rover2026"
#define HTTP_PORT       80

#endif
