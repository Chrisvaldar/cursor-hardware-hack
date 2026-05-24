# Wyde

**Wyde** is an affordable, beginner-friendly rover platform for learning robotics and autonomous driving—without the cost barrier of high-end kits (for context, NASA’s open-source rover is on the order of **$1,600**).

This repository is the **hardware hack / firmware** implementation: an ESP32-controlled tank-drive rover you drive from a laptop over Wi‑Fi. The long-term vision (sensors, autonomy, classroom use) is described in the project pitch deck.

**Pitch deck:** [docs/Wyde.pdf](docs/Wyde.pdf) — *Introducing Wyde* by Cizor (8 slides).

---

## What Wyde is (from the presentation)

| Topic | Summary |
|--------|---------|
| **Problem** | Robotics is expensive and hard to approach without prior experience. |
| **Goal** | A low-cost rover so first-timers can learn **autonomous systems** hands-on. |
| **Hardware direction** | ESP32-WROOM, all-wheel drive for flexible movement, ultrasonic sensing for awareness. |
| **Philosophy** | Built to be **adapted** for classrooms, clubs, and experiments—not a locked black box. |
| **Roadmap** | Stronger battery and motors; richer autonomy as the platform matures. |

The deck frames Wyde as an **autonomous driving learning platform**. This repo is the practical foundation: motor control, Wi‑Fi teleop, and room to grow back into sensing and autonomy.

---

## What this repo does today

The current firmware is intentionally **minimal and low-power**:

- **Tank drive** via one L293D H-bridge (left/right motor pairs).
- **Wi‑Fi access point** on the ESP32 (`RoverTelemetry`).
- **Browser D-pad** (`dashboard_remote.html`) → `GET /cmd?dir=up|down|left|right|stop`.
- **Conservative motor PWM** to reduce brownouts when powered from USB or small packs.

Sensors and autonomy modules were removed in favor of stable manual driving; they can be reintroduced as the electrical and mechanical stack improves (see [rover/README.md](rover/README.md)).

---

## Repository layout

```
cursor-hardware-hack/
├── README.md                 ← you are here
├── dashboard_remote.html     ← laptop controller (open in browser on rover Wi‑Fi)
├── platformio.ini            ← build config (source in rover/)
├── rover/                    ← ESP32 firmware
│   ├── rover.ino
│   ├── motors.cpp / web.cpp / drive.cpp
│   └── config.h              ← pins, speeds, Wi‑Fi credentials
├── docs/
│   ├── Wyde.pdf              ← presentation slides
│   └── l293d_schematic.html  ← motor wiring reference
└── servo_test/               ← separate PlatformIO project for servo bring-up
```

---

## Quick start

### 1. Flash firmware

- Open this folder in **VS Code + PlatformIO** (or Arduino IDE → `rover/rover.ino`).
- Build/upload env **`freenove_esp32`** (generic ESP32 dev board).
- Wiring: see [docs/l293d_schematic.html](docs/l293d_schematic.html).

### 2. Connect and drive

1. Join Wi‑Fi **`RoverTelemetry`** (password **`rover2026`** — change in `rover/config.h` if needed).
2. Open **`dashboard_remote.html`** from this repo (double-click or “Open with Live Server”).
3. Hold **Up / Down / Left / Right** on the D-pad or arrow keys; **Space** or **Stop** to halt.

The rover’s IP as AP is **`192.168.4.1`**.

### 3. Power tips

If the ESP32 **resets when motors start**, the supply is likely sagging:

- Use a dedicated **5 V** source that can deliver motor current (not only laptop USB).
- Keep `DRIVE_SPEED` / `TURN_SPEED` low in `rover/config.h` and raise gradually.

---

## API (current firmware)

| Endpoint | Description |
|----------|-------------|
| `GET /status` | JSON: `dir`, `speed`, `message`, `uptime_ms` |
| `GET /cmd?dir=stop\|up\|down\|left\|right` | Drive command (aliases like `forward`, `steer_center` still accepted) |

Details and tuning: [rover/README.md](rover/README.md).

---

## Hardware overview

| Component | Role |
|-----------|------|
| **ESP32-WROOM** | Wi‑Fi, PWM, GPIO for L293D |
| **L293D** | Dual H-bridge: right pair (GPIO 25/33, EN 14), left pair (18/19, EN 13) |
| **DC gear motors** | Tank drive (two wheels per side wired in parallel on each bridge half) |
| **Power** | 5 V rail for logic + motors (adequate current required under load) |

Ultrasonic and full autonomy align with the [Wyde.pdf](docs/Wyde.pdf) vision; wire them back in when you extend this baseline.


