#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <driver/uart.h>
#include <soc/gpio_num.h>
#include <esp_log.h>
#include <shell.h>

#ifndef UART_H
#define UART_H

#define UART_PORT   UART_NUM_1
#define UART_TX     GPIO_NUM_7
#define UART_RX     GPIO_NUM_6
#define UART_BAUD   115200
#define UART_RX_BUFF_SIZE (SHELL_LINE_MAX_BUFF_SIZE * 2)
#define UART_LOG_TAG "UART"

extern esp_err_t uartInitDrv (  uart_port_t port, 
                                gpio_num_t tx, 
                                gpio_num_t rx, 
                                int baudrate);

extern int uartWrite (uart_port_t port, char * strToWrite);
extern int uartWriteByte (uart_port_t port, char * charToWrite);
extern int uartRead (uart_port_t port, char * readStr);

#endif