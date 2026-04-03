#include <WiFi.h>
#include <WebServer.h>
#include <TM1637Display.h>
#include "DHT.h"

// ================= ACCESS POINT =================
const char* ssid = "ESP32_AirMonitor";
const char* password = "12345678";

WebServer server(80);

// ================= DHT =================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ================= TM1637 =================
#define CLK 18
#define DIO 19
TM1637Display display(CLK, DIO);

// ================= TIME (MANUAL) =================
unsigned long baseMillis = 0;
int baseHour = 12;
int baseMinute = 0;

// ================= HISTORY =================
#define MAX_READINGS 20

struct Data {
  String time;
  float temp;
  float hum;
};

Data history[MAX_READINGS];
int indexHistory = 0;

// ================= TIMERS =================
unsigned long lastDisplaySwitch = 0;
unsigned long lastLiveRead = 0;
unsigned long lastLog = 0;

#define LIVE_INTERVAL 3000        // 3 sec
#define LOG_INTERVAL 1800000      // 30 min

int displayMode = 0;

float temp = 0;
float hum = 0;

// ================= TIME FUNCTIONS =================
void getTime(int &h, int &m) {
  unsigned long elapsed = (millis() - baseMillis) / 1000;

  int totalMinutes = baseHour * 60 + baseMinute + (elapsed / 60);

  h = (totalMinutes / 60) % 24;
  m = totalMinutes % 60;
}

String getTimeString() {
  int h, m;
  getTime(h, m);

  String period = (h >= 12) ? "PM" : "AM";

  int h12 = h % 12;
  if (h12 == 0) h12 = 12;

  String hh = (h12 < 10) ? "0" + String(h12) : String(h12);
  String mm = (m < 10) ? "0" + String(m) : String(m);

  return hh + ":" + mm + " " + period;
}

// ================= HANDLE TIME SET =================
void handleSetTime() {
  if (server.hasArg("h") && server.hasArg("m")) {
    baseHour = server.arg("h").toInt();
    baseMinute = server.arg("m").toInt();
    baseMillis = millis();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

// ================= WEBPAGE =================
String getHTML() {

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='5'>";

  html += "<style>";
  html += "body{font-family:Arial;text-align:center;background:#111;color:#fff;}";
  html += ".card{background:#222;margin:10px;padding:15px;border-radius:10px;font-size:22px;}";
  html += "input{padding:8px;margin:5px;width:60px;}";
  html += "button{padding:10px;border:none;background:#0af;color:#fff;border-radius:5px;}";
  html += "table{width:100%;border-collapse:collapse;margin-top:10px;}";
  html += "td,th{padding:8px;border-bottom:1px solid #444;}";
  html += "</style>";

  html += "</head><body>";

  html += "<h2>🌡 ESP32 Air Monitor</h2>";

  html += "<div class='card'>Temp: " + String(temp) + " °C</div>";
  html += "<div class='card'>Humidity: " + String(hum) + " %</div>";
  html += "<div class='card'>Time: " + getTimeString() + "</div>";

  // ===== TIME SET =====
  html += "<h3>Set Time</h3>";
  html += "<form action='/set'>";
  html += "H:<input name='h' type='number' min='0' max='23'>";
  html += "M:<input name='m' type='number' min='0' max='59'>";
  html += "<br><button type='submit'>Set</button></form>";

  // ===== HISTORY =====
  html += "<h3>30 Min Logs</h3><table>";
  html += "<tr><th>Time</th><th>Temp</th><th>Hum</th></tr>";

  for (int i = 0; i < MAX_READINGS; i++) {
    int idx = (indexHistory + i) % MAX_READINGS;

    if (history[idx].time != "") {
      html += "<tr><td>" + history[idx].time + "</td>";
      html += "<td>" + String(history[idx].temp) + "</td>";
      html += "<td>" + String(history[idx].hum) + "</td></tr>";
    }
  }

  html += "</table>";

  html += "</body></html>";

  return html;
}

// ================= ROUTES =================
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);

  dht.begin();
  display.setBrightness(1);

  WiFi.softAP(ssid, password);

  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetTime);
  server.begin();

  baseMillis = millis();
}

// ================= LOOP =================
void loop() {

  server.handleClient();

  // ===== LIVE SENSOR =====
  if (millis() - lastLiveRead > LIVE_INTERVAL) {

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      temp = t;
      hum = h;
    }

    lastLiveRead = millis();
  }

  // ===== LOG EVERY 30 MIN =====
  if (millis() - lastLog > LOG_INTERVAL) {

    history[indexHistory].time = getTimeString();
    history[indexHistory].temp = temp;
    history[indexHistory].hum  = hum;

    indexHistory = (indexHistory + 1) % MAX_READINGS;

    Serial.println("Logged data (30 min)");

    lastLog = millis();
  }

  // ===== DISPLAY SWITCH =====
  if (millis() - lastDisplaySwitch > 10000) {
    displayMode++;
    if (displayMode > 2) displayMode = 0;
    lastDisplaySwitch = millis();
  }

  int h, m;
  getTime(h, m);

  // ===== DISPLAY =====
if (displayMode == 0) {
  int h, m;
  getTime(h, m);

  int h12 = h % 12;
  if (h12 == 0) h12 = 12;

  display.showNumberDecEx((h12 * 100) + m, 0b01000000, true);
}

  if (displayMode == 1) {
    int t = (int)temp;

    uint8_t data[] = {
      display.encodeDigit((t / 10) % 10),
      display.encodeDigit(t % 10) | 0b10000000,
      display.encodeDigit(0),
      0x39
    };

    display.setSegments(data);
  }

  if (displayMode == 2) {
    display.showNumberDec((int)hum, false);
  }
}