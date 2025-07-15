#ifndef TASKS_H
#define TASKS_H

#define CLI_TASK_PRIORITY 5
#define CLI_TASK_STACK_SIZE 4096

void create_startup_tasks();
void ble_client_task();
void ble_server_task();

#endif