#include "startup.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "uart.h"

void run_startup_procedures() {
    setup_uart_console();
    printf("System startup procedures completed.\n");
}