idf_component_register(SRCS "wifi_map.c"
                    PRIV_REQUIRES esp_wifi nvs_flash esp_driver_uart esp_driver_gpio
                    INCLUDE_DIRS ".")
