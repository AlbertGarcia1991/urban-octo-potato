#include "driver/uart.h"

#define ASCII_BACKSPACE 0x08
#define ASCII_DEL       0x7F
#define CLI_BUF_SIZE 128


void setup_uart_console() {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Install UART driver for interrupt-driven reads
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Set up VFS
    esp_vfs_dev_uart_use_driver(UART_NUM_0);
}

void cli_task(void *arg) {
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

            // Escape sequence (e.g. arrow keys)
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

            if ((c == ASCII_BACKSPACE || c == ASCII_DEL) && cursor > 0) {
                // Shift text left from cursor
                memmove(&line[cursor - 1], &line[cursor], length - cursor);
                cursor--;
                length--;

                // Redraw line
                printf("\b");
                fwrite(&line[cursor], 1, length - cursor, stdout);
                printf(" ");
                for (int i = 0; i <= (length - cursor); i++) printf("\b");
                fflush(stdout);
            }

            else if (c >= 32 && c <= 126 && length < CLI_BUF_SIZE - 1) {
                // Insert character
                memmove(&line[cursor + 1], &line[cursor], length - cursor);
                line[cursor] = c;
                length++;
                cursor++;

                // Print from insertion point to end
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
            printf("  restart - Reboot the chip\n");
        } else if (strcmp(line, "info") == 0) {
            printf("Heap: %" PRIu32 " bytes\n", esp_get_free_heap_size());
        } else if (strcmp(line, "restart") == 0) {
            printf("Restarting...\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            esp_restart();
        } else if (strlen(line) > 0) {
            printf("Unknown command: %s\n", line);
        }
    }
}