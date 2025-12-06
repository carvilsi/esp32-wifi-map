# WIFI_MAP

An ESP32 and FlipperZero wifi mapping

Check the FlipperZero side at: [flipper0-wifi-map](https://github.com/carvilsi/flipper0-wifi-map)

## Colected Data

This is the colected data that will be send to FlipperZero, prepare for CSV semicolon separated:

`AP hash;Distance (meters);AP auth mode;Time from start (seconds)`

### AP hash

As the name suggests the data is hashed, trying here to maintain some kind of privacy, and based on the next APs retrieved concatenaded data:

- [ap_info.ssid](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv4N16wifi_ap_record_t4ssidE)
- [ap_info.pairwise_cipher](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv4N16wifi_ap_record_t15pairwise_cipherE)
- [ap_info.group_cipher](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv4N16wifi_ap_record_t12group_cipherE)
- [ap_info.bssid](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv4N16wifi_ap_record_t5bssidE)

The output looks like *2d2facaa*.

### Distance (meters)

An estimation of the distance in meters between ESP32 and the AP.

### AP auth mode

Data from: [ap_info.authmode](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html#_CPPv4N16wifi_ap_record_t8authmodeE)

### Time from start (seconds)

Seconds elapsed since ESP32 has been connected to FlipperZero GPIO

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

## Notes

### ESP32 WiFi

- [api-reference: network - wifi](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html)
