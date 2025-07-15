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

// Global variable to hold the BLE address type (public or random).
// This is set during initialization and used for scanning and connecting.
uint8_t ble_addr_type;

/**
 * @brief GAP (Generic Access Profile) event handler for the BLE client.
 *
 * This function is called by the NimBLE stack whenever a GAP event occurs.
 * For the client, the most important event is device discovery during scanning.
 *
 * @param event Pointer to the GAP event structure.
 * @param arg   User-defined argument (unused here).
 * @return Always returns 0.
 */
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_hs_adv_fields fields; // Structure to hold parsed advertising fields

    switch (event->type)
    {
    // BLE_GAP_EVENT_DISC is triggered when a BLE device is discovered during scanning
    case BLE_GAP_EVENT_DISC:
        {
            // Format the discovered device's MAC address as a string
            char addr_str[18];
            snprintf(addr_str, sizeof(addr_str), "%02x:%02x:%02x:%02x:%02x:%02x",
                event->disc.addr.val[0], event->disc.addr.val[1],
                event->disc.addr.val[2], event->disc.addr.val[3],
                event->disc.addr.val[4], event->disc.addr.val[5]);
            // Log the MAC address and RSSI (signal strength)
            ESP_LOGI("GAP", "GAP EVENT DISCOVERY: %s (%d)", addr_str, event->disc.rssi);

            // Parse the advertisement data to extract fields like the device name
            ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data);
            if (fields.name_len > 0)
            {
                // Print the device name if present in the advertisement
                printf("Name: %.*s\n", fields.name_len, fields.name);
            }
            // Sometimes the name cannot be parsed, so the previous line may not print anything.
        }
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief Start scanning for BLE devices.
 *
 * This function initiates a BLE scan using the NimBLE stack. It will print all discovered devices
 * and their names (if available) to the console.
 *
 * @param scan_timeout The scan duration in milliseconds. If <= 0, scan forever.
 */
void ble_app_scan(int32_t scan_timeout)
{
    if (scan_timeout <= 0) {
        scan_timeout = BLE_HS_FOREVER; // Default to forever if timeout is not specified
    }
    printf("Start scanning ...\n");

    // Set up scan parameters
    struct ble_gap_disc_params disc_params;
    disc_params.filter_duplicates = 1; // Filter duplicate advertisements
    disc_params.passive = 0;           // Active scanning (request scan response)
    disc_params.itvl = 0;              // Use default scan interval
    disc_params.window = 0;            // Use default scan window
    disc_params.filter_policy = 0;     // No whitelist filtering
    disc_params.limited = 0;           // General discovery mode

    // Start scanning. The ble_gap_event callback will be called for each discovered device.
    ble_gap_disc(ble_addr_type, scan_timeout, &disc_params, ble_gap_event, NULL);

    printf("Scan finished\n");
}

/**
 * @brief BLE stack synchronization callback.
 *
 * This function is called when the BLE host stack is synchronized and ready.
 * It determines the best address type (public or random) for the device.
 */
void ble_app_on_sync(void)
{
    // Determines the best address type automatically (public or random static)
    ble_hs_id_infer_auto(0, &ble_addr_type);                     
}

/**
 * @brief Initialize the NimBLE stack for the BLE client.
 *
 * This function should be called once during startup (after NVS is initialized).
 * It sets up the BLE device name, initializes GAP services, and starts the NimBLE host task.
 */
void ble_nimble_client_init() {
    // Requires nvs_flash_init() to be called before this

    // Initialize the NimBLE host and controller stack
    nimble_port_init();

    // Set the BLE device name (visible to other devices during scanning)
    ble_svc_gap_device_name_set("GymHand Device - CLIENT");

    // Initialize GAP service (standard BLE service for device discovery and connection)
    ble_svc_gap_init();

    // Set the callback to be called when the BLE stack is ready
    ble_hs_cfg.sync_cb = ble_app_on_sync;

    // Start the NimBLE host task (runs BLE protocol stack)
    nimble_port_freertos_init(ble_client_task);

    printf("BLE NimBLE CLIENT initialized\n");
}