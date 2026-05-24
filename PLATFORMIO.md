# PlatformIO quick start (VS Code)

## One-time setup

1. Install **PlatformIO IDE** extension in VS Code (you did this).
2. **File → Open Folder** → select `cursor-hardware-hack` (the folder that contains `platformio.ini`).
3. Wait for PlatformIO to finish installing the ESP32 platform (first time only — can take a few minutes; bottom status bar shows progress).

## Every time you flash

1. Plug ESP32 in via USB.
2. Bottom blue bar → click the **environment** name → pick **`freenove_esp32`** (or `esp32wrover` if you enabled that env in `platformio.ini`).
3. Click **→ Upload** (checkmark arrow icon on the bottom bar), or run task **PlatformIO: Upload**.
4. Click **plug / Serial Monitor** icon, or **PlatformIO: Monitor** — baud is **115200**.

**Success:** Serial shows `AP IP: 192.168.4.1` and `Rover firmware ready.`

## If upload fails

- Unplug/replug USB; check Device Manager for COM port.
- Install CH340 driver from Freenove kit: `CH340/Windows/`
- Hold **BOOT**, tap **EN/RST**, release **BOOT**, upload again.
- Try the other env in `platformio.ini` (`esp32wrover` vs `freenove_esp32`).

## Set COM port manually (if auto-detect fails)

Create `platformio.ini` line under your env:

```ini
upload_port = COM3
monitor_port = COM3
```

Replace `COM3` with your port from Device Manager.

## Commands (optional terminal)

From project folder, with PlatformIO CLI on PATH:

```bash
pio run -e freenove_esp32
pio run -e freenove_esp32 -t upload
pio device monitor -b 115200
```

## Arduino IDE

The same source in `rover/` still works in Arduino IDE if someone prefers it. PlatformIO uses that folder via `src_dir = rover`.
