#include "freertos/FreeRTOS.h"
#include "startup.h"
#include "startup.h"
#include "tasks.h"


void app_main(void)
{
    printf("Hardware started\n");
    
    run_startup_procedures();
    create_startup_tasks();
    
    // Main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}