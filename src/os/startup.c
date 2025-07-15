#include "startup.h"
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "ble_client.h"
#include "ble_server.h"
#include "nvs.h"
#include "uart.h"

static const char *TAG = "STARTUP";

void run_startup_procedures() {
    setup_uart_console();
    nvs_manager_init();
    ble_nimble_client_init();
    ESP_LOGI(TAG, "System startup procedures completed");
}