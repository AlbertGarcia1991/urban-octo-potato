#ifndef CLI_H
#define CLI_H

#define ASCII_BACKSPACE 0x08
#define ASCII_DEL       0x7F
#define CLI_BUF_SIZE 128

void setup_uart_console();
void cli_task();

#endif