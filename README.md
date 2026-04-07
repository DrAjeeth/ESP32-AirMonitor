# ESP indoor climate monitor

## Project Overview

ESP Air Monitor is an IoT-based environmental monitoring system that measures temperature and humidity in real-time. The repository now includes ESP32 firmware modes plus an ESP8266 ESPHome mode for direct Home Assistant integration.

## Features

- **Real-time Environmental Monitoring**: Continuously monitors temperature and humidity
- **Multiple Operation Modes**: Choose between offline, cloud-connected, or Home Assistant-integrated operation
- **Local Display**: 4-digit TM1637 display for real-time readings
- **Flexible Connectivity**: Works offline or connects to WiFi and cloud services
- **Web Interface**: Access data via built-in web server in AP mode
- **Data Logging**: 30-minute logging system for offline mode
- **Manual Time Configuration**: Set device time via web interface
- **ESPHome Support**: Native ESPHome API integration for Home Assistant

## Hardware Requirements

- **ESP32 or ESP8266 Microcontroller**: Core processing unit, depending on the selected mode
- **DHT22 Sensor**: Temperature and humidity sensor
- **TM1637 Display**: 4-digit 7-segment display for readings
- **Power Supply**: USB or battery-powered configuration

## Software Requirements

- Arduino IDE for the ESP32 sketch modes
- ESPHome Dashboard or CLI for the ESP8266 Home Assistant mode
- Required libraries for the sketch modes:
  - DHT sensor library
  - TM1637 display library
  - Adafruit IO libraries (for Cloud Mode)

## Operational Modes

### 1. AP Mode (Offline/Access Point)
**Best for**: Local monitoring without internet connectivity

- Creates a WiFi hotspot
- Access readings via local web server
- No internet connection required
- 30-minute data logging capability
- Manual time setting through web interface

**WiFi Credentials**:
- SSID: `ESP32_AirMonitor`
- Password: `12345678`
- Access URL: Local IP address displayed on device

### 2. Cloud Mode (Adafruit IO)
**Best for**: Remote monitoring and cloud data upload with Adafruit IO

- Connects to existing WiFi network
- Real-time data sync with Adafruit IO
- Remote monitoring from anywhere with internet
- Requires internet connectivity and Adafruit IO credentials

### 3. ESPHome + Home Assistant Mode (ESP8266 NodeMCU)
**Best for**: Local smart-home integration with Home Assistant

- Uses ESPHome on `ESP8266 NodeMCU`
- Native Home Assistant integration through the ESPHome API
- OTA updates and fallback captive portal
- DHT22 entities appear directly inside Home Assistant
- Configuration lives in `ESPHome_HA_Mode/air_monitor_esp8266_dht22.yaml`

## Quick Start

1. **Hardware Assembly**:
   - ESP32 modes: connect DHT22 and TM1637 display to ESP32
   - ESPHome mode: connect DHT22 to ESP8266 NodeMCU
2. **Select Operating Mode**:
   - For AP Mode: Upload `AP_Mode/src/main.cpp`
   - For Cloud Mode: Upload `Cloud_Mode/src/main.cpp`
   - For ESPHome + HA Mode: Flash `ESPHome_HA_Mode/air_monitor_esp8266_dht22.yaml`
3. **Power On**: Device will start in selected mode
4. **Access Data**:
   - AP Mode: Connect to `ESP32_AirMonitor` WiFi and open local IP in browser
   - Cloud Mode: Monitor via Adafruit IO dashboard
   - ESPHome + HA Mode: Add the device in Home Assistant via the ESPHome integration

## Usage

- **AP Mode**: After connecting to the hotspot, open a web browser and navigate to the device's IP address displayed on the TM1637 display
- **Cloud Mode**: Configure WiFi credentials and Adafruit IO settings, then access your data remotely through Adafruit IO
- **ESPHome + HA Mode**: Fill `ESPHome_HA_Mode/secrets.yaml`, flash the YAML config, then adopt the device in Home Assistant







