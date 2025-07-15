#include "tasks.h"
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nimble/nimble_port.h"
#include "cli.h"

static const char *TAG = "TASKS";

void create_startup_tasks() {
    xTaskCreate(cli_task, "cli_task", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, NULL);
    ESP_LOGI(TAG, "Startup tasks launched");
}

void ble_client_task() {
    nimble_port_run();
    ESP_LOGI(TAG, "BLE CLIENT tasks launched");
}

void ble_server_task() {
    nimble_port_run();
    ESP_LOGI(TAG, "BLE SERVER tasks launched");
}