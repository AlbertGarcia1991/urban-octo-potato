/**
 * @file cli.c
 * @brief Command Line Interface (CLI) implementation for GymHand firmware.
 *
 * This file implements a simple CLI for interacting with the device over UART.
 * It supports basic commands for device info, BLE scanning, and system restart.
 */

#include "cli.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "ble_client.h"

static const char *TAG = "CLI";

/**
 * @brief CLI task entry point.
 *
 * This function runs an interactive command line interface over UART.
 * It supports editing, command history, and basic device commands.
 */
void cli_task(void) {
    char line[CLI_BUF_SIZE];
    int length = 0;
    int cursor = 0;

    while (1) {
        printf("> ");
        fflush(stdout);
        length = 0;
        cursor = 0;

        while (true) {
            char c;
            int n = fread(&c, 1, 1, stdin);
            if (n <= 0) continue;

            if (c == '\r' || c == '\n') {
                printf("\r\n");
                line[length] = '\0';
                break;
            }

            // Handle escape sequences (arrow keys)
            if (c == 0x1b) {  // ESC
                char seq[2];
                fread(&seq[0], 1, 1, stdin);  // should be '['
                fread(&seq[1], 1, 1, stdin);  // 'D' or 'C'

                if (seq[0] == '[') {
                    if (seq[1] == 'D') { // Left arrow
                        if (cursor > 0) {
                            printf("\b");
                            fflush(stdout);
                            cursor--;
                        }
                    } else if (seq[1] == 'C') { // Right arrow
                        if (cursor < length) {
                            printf("%c", line[cursor]);
                            fflush(stdout);
                            cursor++;
                        }
                    }
                }
                continue;
            }

            // Handle backspace/delete
            if ((c == ASCII_BACKSPACE || c == ASCII_DEL) && cursor > 0) {
                memmove(&line[cursor - 1], &line[cursor], length - cursor);
                cursor--;
                length--;

                printf("\b");
                fwrite(&line[cursor], 1, length - cursor, stdout);
                printf(" ");
                for (int i = 0; i <= (length - cursor); i++) printf("\b");
                fflush(stdout);
            }
            // Handle printable characters
            else if (c >= 32 && c <= 126 && length < CLI_BUF_SIZE - 1) {
                memmove(&line[cursor + 1], &line[cursor], length - cursor);
                line[cursor] = c;
                length++;
                cursor++;

                fwrite(&line[cursor - 1], 1, length - cursor + 1, stdout);
                for (int i = 0; i < (length - cursor); i++) printf("\b");
                fflush(stdout);
            }
        }

        // Command processing
        if (strcmp(line, "help") == 0) {
            printf("Commands:\n");
            printf("  help    - Show this message\n");
            printf("  info    - Show heap info\n");
            printf("  ble_scan    - Start BLE scanning from Client\n");
            printf("  restart - Reboot the chip\n");
        } else if (strcmp(line, "info") == 0) {
            printf("Heap: %" PRIu32 " bytes\n", esp_get_free_heap_size());
        } else if (strcmp(line, "restart") == 0) {
            printf("Restarting...\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            esp_restart();
        } else if (strcmp(line, "ble_scan") == 0) {
            printf("Starting BLE scan...\n");
            ble_app_scan(5000); // Scan for 5 seconds
        } else {
            printf("Unknown command: %s\n", line);
        }
    }
}