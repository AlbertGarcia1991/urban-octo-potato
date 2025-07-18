/**
 * @file startup.c
 * @brief System startup procedures for GymHand firmware.
 *
 * This file contains initialization routines for hardware peripherals and subsystems,
 * such as UART, NVS, and BLE. It is called once at boot to prepare the system.
 */

#include "startup.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "ble_client.h"
#include "ble_server.h"
#include "nvs.h"
#include "uart.h"

static const char *TAG = "STARTUP";

/**
 * @brief Run all startup procedures required for system initialization.
 *
 * This function initializes the UART console, NVS storage, and BLE client.
 * Additional startup routines can be added here as needed.
 */
void run_startup_procedures(void) {
    // Initialize UART for console I/O
    setup_uart_console();

    // Initialize Non-Volatile Storage (NVS)
    nvs_manager_init();

    // Initialize BLE client (can be conditional based on build config)
#if defined(BUILD_SERVER)
    ble_nimble_server_init();
#elif defined(BUILD_CLIENT)
    ble_nimble_client_init();
#elif defined(BUILD_SERVER) && defined(BUILD_CLIENT)
    "Multiple definitions for BUILD_TARGET"
#else
    #error "You must define compiler parameter BUILD_TARGET to either SERVER or CLIENT to succesfully"
#endif

    ESP_LOGI(TAG, "System startup procedures completed");
}