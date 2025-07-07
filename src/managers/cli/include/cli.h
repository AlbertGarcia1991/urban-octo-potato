#ifndef CLI_H
#define CLI_H
static const char *TAG = "CLI";

void setup_uart_console();

void cli_task(void *arg);

#endif