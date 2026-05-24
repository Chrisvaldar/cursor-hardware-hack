# ESP32 Autonomous Rover Firmware

Arduino C firmware for ESP32-WROVER with modular motor control, sensor abstraction, collision avoidance, wall-following navigation, and Wi-Fi telemetry.

## Quick start (Arduino IDE)

1. Install **ESP32 board support** (Board Manager → esp32 by Espressif).
2. Select board: **ESP32 Wrover Module** (or closest match).
3. Open `rover/rover.ino` in Arduino IDE (opens the whole sketch folder).
4. Flash via USB.
5. Open Serial Monitor at **115200 baud** for boot messages and AP IP.
6. On your laptop, join Wi-Fi network **`RoverTelemetry`** (password: `rover2026`).
7. Open **http://192.168.4.1/status** for live JSON telemetry.

## Project structure

| File | Role |
|------|------|
| `config.h` | Pin map, thresholds, timing, Wi-Fi AP credentials |
| `types.h` / `types.cpp` | Shared enums and `g_state` snapshot |
| `motors.cpp` | L293D motor driver (PWM + direction) |
| `sensors.cpp` | **Teammates edit here** for real sensor reads |
| `collision.cpp` | Non-blocking obstacle avoidance FSM |
| `navigation.cpp` | Left wall-following / wander FSM |
| `telemetry.cpp` | SoftAP + HTTP `/status` JSON |
| `rover.ino` | Main loop orchestration |

## Pin map (placeholders — update in `config.h`)

| Signal | GPIO |
|--------|------|
| Motor A IN1 | 26 |
| Motor A IN2 | 27 |
| Motor A EN (PWM) | 14 |
| Motor B IN1 | 32 |
| Motor B IN2 | 33 |
| Motor B EN (PWM) | 12 |

## Teammate integration

### Sensors (`sensors.cpp`)

Implement hardware reads inside `sensorsUpdate()` or replace `readDistanceStub()`. Keep returning **centimetres**. Use **100** (or similar) when no obstacle is detected.

Logic modules call only:

- `readFrontDistance()`
- `readLeftDistance()`
- `readRightDistance()`

### Pins (`config.h`)

Adjust motor and (future) sensor pins to match your schematic in one place.

### Tuning (`config.h`)

- `OBSTACLE_THRESHOLD_CM` — front stop distance
- `WALL_TARGET_CM` / `WALL_HYSTERESIS_CM` — wall follow spacing
- `NAV_SPEED`, `COLLISION_TURN_SPEED` — drive speeds (0–255 PWM)

## Telemetry API

**GET /** — simple HTML dashboard (auto-refresh)

**GET /status** — JSON example:

```json
{
  "uptime_ms": 12345,
  "front_cm": 42.0,
  "left_cm": 18.0,
  "right_cm": 65.0,
  "drive": "forward",
  "speed": 160,
  "collision": "idle",
  "navigation": "wander",
  "message": "following left wall"
}
```

### Python poll example

```python
import time
import urllib.request

URL = "http://192.168.4.1/status"
while True:
    with urllib.request.urlopen(URL, timeout=2) as r:
        print(r.read().decode())
    time.sleep(0.5)
```

## Control flow

Each `loop()` iteration (non-blocking, `millis()` only):

1. Read sensors (every 50 ms)
2. Run collision FSM (every 20 ms) — **priority over navigation**
3. Run navigation FSM when collision is idle
4. Serve HTTP telemetry

No `delay()` in the main loop (single short delay at boot for Serial only).

## Behaviour summary

- **Navigation:** left wall-following; open-space forward wander with `SeekWall` turn.
- **Collision:** stop if front &lt; threshold → compare left/right clearance → turn toward clearer side → short forward resume → idle.
