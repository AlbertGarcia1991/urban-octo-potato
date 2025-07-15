#include "nimble/nimble_port.h"            // NimBLE porting layer for ESP32
#include "nimble/nimble_port_freertos.h"   // NimBLE FreeRTOS integration
#include "host/ble_hs.h"                   // NimBLE host stack API
#include "services/gap/ble_svc_gap.h"      // Generic Access Profile (GAP) service
#include "services/gatt/ble_svc_gatt.h"    // Generic Attribute Profile (GATT) service
#include "ble.h"                           // Project-specific BLE header
#include "tasks.h"                         // Project-specific task management

/**
 * @file ble.c
 * @brief BLE (Bluetooth Low Energy) initialization and service definition for GymHand device.
 *
 * This file demonstrates how to set up a BLE GATT server, advertise it, and handle basic read/write operations
 * using the NimBLE stack on ESP32. It is heavily commented for educational purposes.
 */

// Global variable to hold the BLE address type (public or random)
uint8_t ble_addr_type;


/**
 * @brief GAP event handler.
 *
 * This function is called by the NimBLE stack whenever a GAP (Generic Access Profile) event occurs.
 * It handles connection events and advertising completion events.
 *
 * @param event Pointer to the GAP event structure.
 * @param arg   User-defined argument (unused here).
 * @return Always returns 0.
 */
static int ble_gap_event(struct ble_gap_event *event, void *arg) {
    switch (event->type)
    {
    // Called when a connection is established or fails
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        // If connection failed, restart advertising
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;
    // Called when advertising completes (e.g., times out or is stopped)
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE GAP EVENT");
        // Restart advertising
        ble_app_advertise();
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief Start BLE advertising.
 *
 * This function sets up the advertising data (including the device name) and starts advertising.
 * Advertising makes the device discoverable and connectable by BLE clients (e.g., smartphones).
 */
void ble_app_advertise(void) {
    // Prepare advertising fields (what information is broadcasted)
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Get the current BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields); // Set the advertising data

    // Set advertising parameters (connectable, discoverable, etc.)
    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // Undirected connectable advertising
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // General discoverable mode

    // Start advertising forever (until stopped or connected)
    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

/**
 * @brief GATT characteristic write callback (Write from Per to Cen).
 *
 * This function is called when a BLE client writes data to the writable characteristic.
 *
 * @param conn_handle  Connection handle.
 * @param attr_handle  Attribute handle.
 * @param ctxt         GATT access context (contains the data).
 * @param arg          User-defined argument (unused here).
 * @return 0 on success.
 */
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
    // Print the data received from the client
    printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    return 0;
}

/**
 * @brief GATT characteristic read callback (Read form Per from Cen).
 *
 * This function is called when a BLE client reads from the readable characteristic.
 *
 * @param conn_handle  Connection handle.
 * @param attr_handle  Attribute handle.
 * @param ctxt         GATT access context (used to send data back).
 * @param arg          User-defined argument (unused here).
 * @return 0 on success.
 */
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
    // Respond with a static string
    os_mbuf_append(ctxt->om, "Data from the server", strlen("Data from the server"));
    return 0;
}

/**
 * @brief GATT service and characteristic definitions.
 *
 * This array defines the GATT services and characteristics exposed by the device.
 * - One primary service (UUID 0x180)
 *   - Read characteristic (UUID 0xFEF4)
 *   - Write characteristic (UUID 0xDEAD)
 */
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,           // Primary service
        .uuid = BLE_UUID16_DECLARE(0x180),           // Service UUID
        .characteristics = (struct ble_gatt_chr_def[]){
            {
                .uuid = BLE_UUID16_DECLARE(0xFEF4),  // Read characteristic UUID
                .flags = BLE_GATT_CHR_F_READ,        // Readable
                .access_cb = device_read             // Read callback
            },
            {
                .uuid = BLE_UUID16_DECLARE(0xDEAD),  // Write characteristic UUID
                .flags = BLE_GATT_CHR_F_WRITE,       // Writable
                .access_cb = device_write            // Write callback
            },
            {0} // End of characteristics
        }
    },
    {0} // End of services
};

/**
 * @brief Called when the NimBLE host stack is synchronized and ready.
 *
 * This function is set as the sync callback and is called once the BLE host stack is initialized.
 * It infers the best address type and starts advertising.
 */
void ble_app_on_sync(void)
{
    // Determine the best address type (public or random)
    ble_hs_id_infer_auto(0, &ble_addr_type);
    // Start advertising
    ble_app_advertise();
}

/**
 * @brief Initialize the NimBLE stack and register services.
 *
 * This function should be called once during startup (after NVS is initialized).
 * It sets up the BLE device name, initializes GAP and GATT services, registers the GATT table,
 * and starts the NimBLE host task.
 */
void ble_nimble_init() {
    // Requires nvs_flash_init() to be called before this

    // Initialize the NimBLE host and controller stack
    nimble_port_init();

    // Set the BLE device name (visible to clients)
    ble_svc_gap_device_name_set("GymHand Device");

    // Initialize GAP and GATT services (standard BLE services)
    ble_svc_gap_init();
    ble_svc_gatt_init();

    // Register custom GATT services and characteristics
    ble_gatts_count_cfg(gatt_svcs); // Count attributes
    ble_gatts_add_svcs(gatt_svcs);  // Add services

    // Set the callback to be called when the BLE stack is ready
    ble_hs_cfg.sync_cb = ble_app_on_sync;

    // Start the NimBLE host task (runs BLE protocol stack)
    nimble_port_freertos_init(ble_task);

    printf("BLE NimBLE initialized\n");
}