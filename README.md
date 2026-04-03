# ESP32 Air Monitor

## Project Overview

ESP32 Air Monitor is an IoT-based environmental monitoring system that measures temperature, humidity, and air quality in real-time. The device displays readings on a local 4-digit display and supports two operational modes: offline Access Point mode for local monitoring and Cloud mode for remote monitoring via Arduino IoT Cloud.

## Features

- **Real-time Environmental Monitoring**: Continuously monitors temperature and humidity
- **Dual Operation Modes**: Choose between offline (AP) or cloud-connected operation
- **Local Display**: 4-digit TM1637 display for real-time readings
- **Flexible Connectivity**: Works offline or connects to WiFi and cloud services
- **Web Interface**: Access data via built-in web server in AP mode
- **Data Logging**: 30-minute logging system for offline mode
- **Manual Time Configuration**: Set device time via web interface

## Hardware Requirements

- **ESP32 Microcontroller**: Core processing unit
- **DHT22 Sensor**: Temperature and humidity sensor
- **TM1637 Display**: 4-digit 7-segment display for readings
- **Power Supply**: USB or battery-powered configuration

## Software Requirements

- Arduino IDE (with ESP32 board support)
- Required libraries:
  - DHT sensor library
  - TM1637 display library
  - Arduino IoT Cloud libraries (for Cloud Mode)

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

### 2. Cloud Mode (Arduino IoT Cloud)
**Best for**: Remote monitoring and cloud data storage

- Connects to existing WiFi network
- Real-time data sync with Arduino IoT Cloud
- Remote monitoring from anywhere with internet
- Requires internet connectivity and Arduino IoT credentials

## Quick Start

1. **Hardware Assembly**: Connect DHT22 and TM1637 display to ESP32
2. **Select Operating Mode**:
   - For AP Mode: Upload `AP_Mode/src/main.cpp`
   - For Cloud Mode: Upload `Cloud_Mode/src/main.cpp`
3. **Power On**: Device will start in selected mode
4. **Access Data**:
   - AP Mode: Connect to `ESP32_AirMonitor` WiFi and open local IP in browser
   - Cloud Mode: Monitor via Arduino IoT Cloud dashboard

## Usage

- **AP Mode**: After connecting to the hotspot, open a web browser and navigate to the device's IP address displayed on the TM1637 display
- **Cloud Mode**: Configure WiFi credentials and Arduino IoT Cloud settings then access your data remotely through the Arduino IoT Cloud platform







