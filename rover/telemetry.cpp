#include "telemetry.h"
#include "config.h"
#include "types.h"

#include <WiFi.h>
#include <WebServer.h>

static WebServer s_server(HTTP_PORT);

static void handleRoot() {
  const String html =
    "<!DOCTYPE html><html><head>"
    "<meta charset='utf-8'>"
    "<meta http-equiv='refresh' content='1'>"
    "<title>Rover Telemetry</title></head><body>"
    "<h1>Rover Telemetry</h1>"
    "<p>Live JSON: <a href='/status'>/status</a></p>"
    "<p>Connect laptop to AP <b>" AP_SSID "</b>, then open "
    "<a href='http://192.168.4.1/status'>http://192.168.4.1/status</a></p>"
    "</body></html>";
  s_server.send(200, "text/html", html);
}

static void handleStatus() {
  char json[512];
  snprintf(json, sizeof(json),
    "{"
    "\"uptime_ms\":%lu,"
    "\"front_cm\":%.1f,"
    "\"left_cm\":%.1f,"
    "\"right_cm\":%.1f,"
    "\"drive\":\"%s\","
    "\"speed\":%u,"
    "\"collision\":\"%s\","
    "\"navigation\":\"%s\","
    "\"message\":\"%s\""
    "}",
    g_state.uptimeMs,
    g_state.frontCm,
    g_state.leftCm,
    g_state.rightCm,
    driveCommandToString(g_state.drive),
    g_state.speed,
    collisionPhaseToString(g_state.collision),
    navPhaseToString(g_state.navigation),
    g_state.statusMessage ? g_state.statusMessage : ""
  );
  s_server.send(200, "application/json", json);
}

void telemetryInit() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  s_server.on("/", handleRoot);
  s_server.on("/status", handleStatus);
  s_server.begin();

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void telemetryUpdate(uint32_t now) {
  g_state.uptimeMs = now;
  s_server.handleClient();
}
