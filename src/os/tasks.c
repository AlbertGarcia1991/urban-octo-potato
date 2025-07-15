/**
 * @file tasks.c
 * @brief Task management for GymHand firmware.
 *
 * This file defines FreeRTOS tasks for CLI, BLE client, and BLE server operations.
 * Tasks are created and managed here to enable concurrent execution of subsystems.
 */

#include "tasks.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cli.h"
#include "nimble/nimble_port.h"

static const char *TAG = "TASKS";

/**
 * @brief Create and launch startup tasks.
 *
 * This function creates the CLI task and logs the startup event.
 */
void create_startup_tasks(void) {
    xTaskCreate(cli_task, "cli_task", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, NULL);
    ESP_LOGI(TAG, "Startup tasks launched");
}

/**
 * @brief BLE client task entry point.
 *
 * This function runs the NimBLE protocol stack for BLE client operations.
 */
void ble_client_task(void) {
    nimble_port_run();
    ESP_LOGI(TAG, "BLE CLIENT tasks launched");
}

/**
 * @brief BLE server task entry point.
 *
 * This function runs the NimBLE protocol stack for BLE server operations.
 */
void ble_server_task(void) {
    nimble_port_run();
    ESP_LOGI(TAG, "BLE SERVER tasks launched");
}