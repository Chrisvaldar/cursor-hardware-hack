#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

// =============================================================================
// Motor pins — L293D dual H-bridge (adjust to match your wiring schematic)
// Motor A = typically left wheel, Motor B = right wheel
// Avoid strapping pins 0, 2, 15 on ESP32; confirm against WROVER pinout.
// =============================================================================
#define MOTOR_A_IN1  26
#define MOTOR_A_IN2  27
#define MOTOR_A_EN   14

#define MOTOR_B_IN1  32
#define MOTOR_B_IN2  33
#define MOTOR_B_EN   12

// LEDC PWM for enable pins (channels only used on ESP32 Arduino core 2.x)
#define PWM_FREQ        5000
#define PWM_RESOLUTION  8
#define PWM_CHANNEL_A   0  // legacy core 2.x
#define PWM_CHANNEL_B   1

// =============================================================================
// Timing — all non-blocking via millis()
// =============================================================================
#define SENSOR_READ_MS    50
#define CONTROL_TICK_MS   20
#define TELEMETRY_MS      200

#define STOP_SETTLE_MS    100
#define TURN_DURATION_MS  400
#define RESUME_CREEP_MS   150
#define NAV_CORRECT_MS    300
#define SEEK_WALL_MS      500

// =============================================================================
// Collision avoidance thresholds
// =============================================================================
#define OBSTACLE_THRESHOLD_CM   25.0f
#define CLEARANCE_MARGIN_CM      5.0f
#define OPEN_SPACE_CM          80.0f   // all readings above = open area

#define COLLISION_TURN_SPEED   180
#define RESUME_SPEED           140

// =============================================================================
// Navigation — left wall following
// =============================================================================
#define WALL_TARGET_CM         15.0f
#define WALL_HYSTERESIS_CM      3.0f
#define NAV_SPEED              160
#define NAV_TURN_SPEED         140
#define SEEK_WALL_SPEED        120

// =============================================================================
// Wi-Fi access point (rover hosts hotspot for laptop dashboard)
// =============================================================================
#define AP_SSID      "RoverTelemetry"
#define AP_PASSWORD  "rover2026"
#define HTTP_PORT    80

#define SERIAL_BAUD  115200

#endif
