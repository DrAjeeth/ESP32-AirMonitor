# Indoor Climate Monitor HA Mode

This mode is for an `ESP8266 NodeMCU` board using a `DHT22` sensor with native `ESPHome` integration into `Home Assistant`.

## Included

- Native Home Assistant discovery through `api`
- OTA updates
- Wi-Fi fallback captive portal
- DHT22 temperature and humidity entities
- Extra diagnostics: Wi-Fi RSSI, uptime, IP, SSID, MAC, online status

## Default Wiring

- `DHT22 VCC` -> `3V3`
- `DHT22 GND` -> `GND`
- `DHT22 DATA` -> `D2` (`GPIO4`)

If your module needs it, place a `4.7k` to `10k` pull-up resistor between `DATA` and `3V3`.

## Setup

1. Create `ESPHome_HA_Mode/secrets.yaml` from `ESPHome_HA_Mode/secrets.example.yaml`.
2. Fill in your Wi-Fi credentials.
3. Generate an API encryption key with `openssl rand -base64 32`.
4. Flash `ESPHome_HA_Mode/indoor_climate_monitor_esp8266_dht22.yaml` from the ESPHome Dashboard or CLI.
5. In Home Assistant, open `Settings` -> `Devices & Services` and add the discovered ESPHome device if it does not appear automatically.

## Notes

- The default board is `nodemcuv2`.
- The default sensor pin is `D2`. Change `dht_pin` in the YAML if your wiring is different.
- The sensor update interval is `30s`. Change `dht_update_interval` if you want slower or faster updates.
