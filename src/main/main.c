#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "startup.h"
#include "tasks.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Hardware started");
    run_startup_procedures();
    create_startup_tasks();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}