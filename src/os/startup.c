#include "startup.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "uart.h"
#include "nvs.h"
#include "ble_client.h"
#include "ble_server.h"

void run_startup_procedures() {
    setup_uart_console();
    nvs_manager_init();
    ble_nimble_client_init();
    printf("System startup procedures completed.\n");
}