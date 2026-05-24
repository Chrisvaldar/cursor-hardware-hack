#include "web.h"
#include "config.h"
#include "state.h"
#include "drive.h"
#include "motors.h"

#include <WiFi.h>
#include <WebServer.h>

static WebServer s_server(HTTP_PORT);
static uint32_t s_remoteUntil = 0;
static DriveDir s_remoteDir = DriveDir::Stop;

static uint8_t speedForDir(DriveDir dir) {
  if (dir == DriveDir::Left || dir == DriveDir::Right) {
    return TURN_SPEED;
  }
  if (dir == DriveDir::Up || dir == DriveDir::Down) {
    return DRIVE_SPEED;
  }
  return 0;
}

static void driveMotors(DriveDir dir) {
  const uint8_t spd = speedForDir(dir);
  switch (dir) {
    case DriveDir::Up:    motorsForward(spd); break;
    case DriveDir::Down:  motorsBack(spd); break;
    case DriveDir::Left:  motorsLeft(spd); break;
    case DriveDir::Right: motorsRight(spd); break;
    default:              motorsStop(); break;
  }
}

void remoteControlExtend(uint32_t now) {
  s_remoteUntil = now + MANUAL_CONTROL_MS;
}

bool remoteControlActive(uint32_t now) {
  return now < s_remoteUntil;
}

static void applyDriveDir(DriveDir dir) {
  s_remoteDir = dir;
  driveMotors(dir);
  g_state.message = driveDirToString(dir);
}

void remoteDriveMaintain(uint32_t now) {
  if (!remoteControlActive(now)) {
    if (s_remoteDir != DriveDir::Stop) {
      motorsStop();
      s_remoteDir = DriveDir::Stop;
      g_state.message = "stop";
    }
    return;
  }
  if (s_remoteDir != DriveDir::Stop) {
    driveMotors(s_remoteDir);
  }
}

static void addCorsHeaders() {
  s_server.sendHeader("Access-Control-Allow-Origin", "*");
  s_server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
}

static void handleStatus() {
  if (s_server.method() == HTTP_OPTIONS) {
    addCorsHeaders();
    s_server.send(204);
    return;
  }

  char json[160];
  snprintf(json, sizeof(json),
    "{\"uptime_ms\":%lu,\"dir\":\"%s\",\"speed\":%u,\"message\":\"%s\"}",
    g_state.uptimeMs,
    driveDirToString(s_remoteDir),
    g_state.speed,
    g_state.message ? g_state.message : ""
  );
  addCorsHeaders();
  s_server.send(200, "application/json", json);
}

static void handleCmd() {
  if (s_server.method() == HTTP_OPTIONS) {
    addCorsHeaders();
    s_server.send(204);
    return;
  }

  DriveDir dir = DriveDir::Stop;
  bool parsed = false;

  if (s_server.hasArg("dir")) {
    parsed = parseDriveDir(s_server.arg("dir"), dir);
  } else if (s_server.hasArg("action")) {
    parsed = parseDriveDir(s_server.arg("action"), dir);
  }

  if (!parsed) {
    addCorsHeaders();
    s_server.send(400, "text/plain", "use ?dir=stop|up|down|left|right");
    return;
  }

  remoteControlExtend(millis());
  applyDriveDir(dir);

  addCorsHeaders();
  s_server.send(200, "text/plain", "ok");
}

void webInit() {
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(true);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  s_server.on("/status", handleStatus);
  s_server.on("/cmd", handleCmd);
  s_server.begin();
}

void webUpdate(uint32_t now) {
  g_state.uptimeMs = now;
  s_server.handleClient();
}
