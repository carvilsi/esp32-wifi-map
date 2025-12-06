#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "rom/ets_sys.h"
#include <time.h>

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define DEFAULT_SCAN_LIST_SIZE 20
#define MS_DELAY 1000 // scanning wifi every 1s


static const char *TAG = "WiFiMap";

static unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

float calc_dist_rssi(int rssi) {
        float m = pow(10, (float)(-44 - rssi)/24);
        return m;
}

const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
};

void init_uart(void) {
        uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
        uart_param_config(UART_NUM_2, &uart_config);
        uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* data) {
        const int len = strlen(data);
        const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
        return txBytes;
}

static void wifi_init() {
        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
}

static void wifi_scan(void *pvParameters) {
        TickType_t xDelay = MS_DELAY / portTICK_PERIOD_MS;
        for (;;) {
                vTaskDelay(xDelay);
                uint16_t number = DEFAULT_SCAN_LIST_SIZE;
                wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
                uint16_t ap_count = 0;
                memset(ap_info, 0, sizeof(ap_info));

                time_t now;
                esp_wifi_scan_start(NULL, true);
                ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
                ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
                for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
                        time(&now);
                        char *hashed = (char *)malloc(sizeof(ap_info[i].ssid) + sizeof(int) * 2);
                        sprintf(hashed, "%s_%d_%d", ap_info[i].ssid,  ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
                        int hsh = hash(hashed);
                        float dst = calc_dist_rssi(ap_info[i].rssi);
                        ESP_LOG_DEBUG("AP MAC: %s", ap_info[i]);
                        ESP_LOGD(TAG, "AP: %s x%x @: %.2fm",ap_info[i].ssid, hsh, dst);
                        char *ap = (char *)malloc(sizeof(hsh) + sizeof(dst) + sizeof(ap_info[i].authmode) + 10);
                        sprintf(ap, "%x;%.2f;%d;%lld\n", hsh, dst, ap_info[i].authmode, now);
                        sendData(ap);
                        free(hashed);
                        free(ap);
                }
                esp_wifi_scan_stop();
        }
}

void app_main(void) {
        init_uart();
        esp_log_level_set(TAG, ESP_LOG_DEBUG);
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);
        wifi_init();
        xTaskCreate(wifi_scan, TAG, 4500, NULL, tskIDLE_PRIORITY, NULL);
}

