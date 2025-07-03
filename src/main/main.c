#include "freertos/FreeRTOS.h"
#include "tasks.h"


void app_main(void)
{
    printf("Hello world!\n");
    
    create_startup_tasks();
    
    // Main loop
    while (1) {
        // Simulate some work
        printf("Running main loop...\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}