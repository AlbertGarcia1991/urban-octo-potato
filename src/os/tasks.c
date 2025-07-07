#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks.h"
#include "mincli.h"

void create_startup_tasks() {
    xTaskCreate(cli_task, "cli_task", 4096, NULL, 5, NULL);
}