#include "freertos/FreeRTOS.h"


void app_main(void)
{
    printf("Hello world!\n");
    
    // Initialize the console
    printf("Console initialized.\n");
    
    // Main loop
    while (1) {
        // Simulate some work
        printf("Running main loop...\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}