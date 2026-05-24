# Rover firmware

ESP32 tank drive over Wi‑Fi — remote only, low motor power to reduce brownouts.

## Flash

1. PlatformIO env **`freenove_esp32`** (or Arduino IDE → `rover/rover.ino`).
2. Join **`RoverTelemetry`** / **`rover2026`**.
3. Open **`dashboard_remote.html`**.

## API

- `GET /status` — `{"dir":"stop","speed":0,...}`
- `GET /cmd?dir=up|down|left|right|stop`

## Power

Motor PWM caps in `config.h`: `DRIVE_SPEED` (straight), `TURN_SPEED` (pivot). Raise slowly if the ESP stays up.

If it still resets when moving: use a beefier 5 V supply (not laptop USB only), or lower `DRIVE_SPEED` further.
