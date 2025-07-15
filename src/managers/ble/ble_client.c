#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "sdkconfig.h"
#include "ble_client.h"                    // Project-specific BLE header
#include "tasks.h"                         // Project-specific task management

/**
 * @file ble_client.c
 * @brief BLE (Bluetooth Low Energy) CLIENT initialization and service definition for GymHand device.
 *
 * This file demonstrates how to set up a BLE GATT client, scan for servers, and handle basic read/write operations
 * using the NimBLE stack on ESP32. It is heavily commented for educational purposes.
 *
 * By client, we mean that this device scans for and connects to BLE servers (like sensors or peripherals).
 * The client can discover services and characteristics exposed by servers and perform read or write operations on them.
 */


// Global variable to hold the BLE address type (public or random)
uint8_t ble_addr_type;


// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_hs_adv_fields fields;

    switch (event->type)
    {
    // NimBLE event discovery
    case BLE_GAP_EVENT_DISC:
        char addr_str[18];
        snprintf(addr_str, sizeof(addr_str), "%02x:%02x:%02x:%02x:%02x:%02x",
            event->disc.addr.val[0], event->disc.addr.val[1],
            event->disc.addr.val[2], event->disc.addr.val[3],
            event->disc.addr.val[4], event->disc.addr.val[5]);
        // addr_str now contains the MAC address as a string
        ESP_LOGI("GAP", "GAP EVENT DISCOVERY: %s (%d)", addr_str, event->disc.rssi);
        ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data);
        if (fields.name_len > 0)
        {
            printf("Name: %.*s\n", fields.name_len, fields.name);
        }
        // Sometimes the name cannot be parsed, so the previousline may not print anything.
        break;
    default:
        break;
    }
    return 0;
}

void ble_app_scan(int32_t scan_timeout)
{
    if (scan_timeout <= 0) {
        scan_timeout = BLE_HS_FOREVER; // Default to forever if timeout is not specified
    }
    printf("Start scanning ...\n");

    struct ble_gap_disc_params disc_params;
    disc_params.filter_duplicates = 1;
    disc_params.passive = 0;
    disc_params.itvl = 0;
    disc_params.window = 0;
    disc_params.filter_policy = 0;
    disc_params.limited = 0;

    ble_gap_disc(ble_addr_type, scan_timeout, &disc_params, ble_gap_event, NULL);

    printf("Scan finished\n");
}

// The application
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically                     
}

/**
 * @brief Initialize the NimBLE stack and register services.
 *
 * This function should be called once during startup (after NVS is initialized).
 * It sets up the BLE device name, initializes GAP and GATT services, registers the GATT table,
 * and starts the NimBLE host task.
 */
void ble_nimble_client_init() {
    // Requires nvs_flash_init() to be called before this

    // Initialize the NimBLE host and controller stack
    nimble_port_init();

    // Set the BLE device name (visible to clients)
    ble_svc_gap_device_name_set("GymHand Device - CLIENT");

    // Initialize GAP and GATT services (standard BLE services)
    ble_svc_gap_init();

    // Set the callback to be called when the BLE stack is ready
    ble_hs_cfg.sync_cb = ble_app_on_sync;

    // Start the NimBLE host task (runs BLE protocol stack)
    nimble_port_freertos_init(ble_client_task);

    printf("BLE NimBLE CLIENT initialized\n");
}