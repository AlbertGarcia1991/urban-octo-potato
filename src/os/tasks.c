#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks.h"
#include "mincli.h"

// TODO: Set hardcoded values as defined constants
void create_startup_tasks() {
    xTaskCreate(cli_task, "cli_task", 4096, NULL, 5, NULL);
}