# WIFI_MAP

An ESP32 and FlipperZero wifi mapping

Check the FlipperZero side at: [flipper0-wifi-map](https://github.com/carvilsi/flipper0-wifi-map)

## Build and flash

$ `idf.py flash`

### Build and flash and monitor

$ `idf.py flash monitor`

## Connection FlipperZero and ESP32

| Flipper Zero GPIO |    ESP32    |
|-------------------|-------------|
|      GND          |     GND     |
|      RX           | TXD GPIO_17 |
|      3V3          |     3V3     |
