/**
 * @file uart.c
 * @brief UART console initialization for GymHand firmware.
 *
 * This file sets up the UART peripheral for console I/O, enabling communication
 * with the device over serial.
 */

#include "uart.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_vfs_dev.h"

static const char *TAG = "UART";

/**
 * @brief Initialize UART console for serial communication.
 *
 * This function configures the UART peripheral and installs the driver.
 */
void setup_uart_console(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    ESP_LOGI(TAG, "UART console setup completed");
}