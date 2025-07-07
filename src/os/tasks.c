#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks.h"
#include "mincli.h"

void create_startup_tasks() {
    xTaskCreate(cli_task, "cli_task", CLI_TASK_STACK_SIZE, NULL, CLI_TASK_PRIORITY, NULL);
}