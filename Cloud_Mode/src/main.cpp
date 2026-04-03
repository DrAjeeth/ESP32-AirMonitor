#include <WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TM1637Display.h>
#include "DHT.h"

// ================= WIFI + ADAFRUIT IO =================
#define WIFI_SSID       " "
#define WIFI_PASS       " "

#define IO_USERNAME     " "
#define IO_KEY          " "

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed    = io.feed("humidity");

// ================= DHT =================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ================= TM1637 =================
#define CLK 18
#define DIO 19
TM1637Display display(CLK, DIO);

// ================= NTP =================
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST offset

// ================= VARIABLES =================
unsigned long lastDisplaySwitch = 0;
unsigned long lastUpload = 0;
int displayMode = 0;  // 0=time, 1=temp, 2=humidity

float temp = 0;
float hum = 0;

void setup() {

  Serial.begin(115200);

  display.setBrightness(1);

  dht.begin();

  // Connect to Adafruit IO
  io.connect();

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected to Adafruit IO");

  timeClient.begin();
}

void loop() {

  io.run();   // keep cloud connection alive
  timeClient.update();

  // ===== Read Sensor =====
  temp = dht.readTemperature();
  hum  = dht.readHumidity();

  // ===== DISPLAY SWITCH (every 10 sec) =====
  if (millis() - lastDisplaySwitch > 10000) {
    displayMode++;
    if (displayMode > 2) displayMode = 0;
    lastDisplaySwitch = millis();
  }


// ===== TIME (12 HR FORMAT) =====
if (displayMode == 0) {

  int hours24 = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  int hours12 = hours24 % 12;
  if (hours12 == 0) hours12 = 12;

  int displayTime = (hours12 * 100) + minutes;

  display.showNumberDecEx(displayTime, 0b01000000, true); 
}

// ===== TEMPERATURE (25.C) =====
if (displayMode == 1 && !isnan(temp)) {

  int t = (int)temp;

  uint8_t data[] = {
    display.encodeDigit((t / 10) % 10),
    display.encodeDigit(t % 10) | 0b10000000, // decimal point
    display.encodeDigit(0),  // blank
    0x39  // C
  };

  display.setSegments(data);
}


// ===== HUMIDITY =====
if (displayMode == 2 && !isnan(hum)) {

  int h = (int)hum;
  display.showNumberDec(h, false);
}

  // ===== CLOUD UPLOAD (every 15 sec safe) =====
  if (millis() - lastUpload > 15000) {

    if (!isnan(temp) && !isnan(hum)) {
      temperatureFeed->save(temp);
      humidityFeed->save(hum);

      Serial.println("Data uploaded to cloud");
    }

    lastUpload = millis();
  }

  delay(500);
}