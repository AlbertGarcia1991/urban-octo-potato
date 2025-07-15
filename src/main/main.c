/**
 * @file main.c
 * @brief Entry point for the GymHand firmware application.
 *
 * This file contains the main application logic, including hardware initialization
 * and the main loop. It starts up all required subsystems and tasks.
 */

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "startup.h"
#include "tasks.h"

static const char *TAG = "MAIN";

/**
 * @brief Main application entry point.
 *
 * This function is called by the ESP-IDF runtime after boot.
 * It initializes hardware, runs startup procedures, and launches tasks.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Hardware started");

    // Run startup procedures (UART, NVS, BLE, etc.)
    run_startup_procedures();

    // Create and launch application tasks
    create_startup_tasks();

    // Main loop (can be used for background processing)
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}