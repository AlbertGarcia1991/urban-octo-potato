#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#include <stdint.h>

void ble_nimble_client_init();
void ble_app_scan(int32_t scan_timeout);

#endif