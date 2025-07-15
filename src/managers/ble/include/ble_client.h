#ifndef BLE_CLIENT_H
#define BLE_CLIENT_H

#include <stdint.h>

void ble_nimble_client_init();
void ble_app_scan(int32_t scan_timeout);

#endif // BLE_CLIENT_H