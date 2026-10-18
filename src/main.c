#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#include <network.h>
#include <shell.h>
#include <uart.h>

#define MAIN_TAG "Main"

void app_main() 
{

    ESP_ERROR_CHECK (gpio_set_direction (GPIO_NUM_8, GPIO_MODE_OUTPUT));

    ESP_ERROR_CHECK (uartInitDrv (UART_PORT, UART_TX, UART_RX, UART_BAUD));

	ESP_LOGI (MAIN_TAG, "uartInitDrv ready");

    if (xTaskCreate (shellSrv, 
                    SHELL_TASK_NAME, 
                    SHELL_TASK_STACK, 
                    NULL, 
                    tskIDLE_PRIORITY + 1, 
                    NULL) != pdPASS)
    {
        ESP_ERROR_CHECK (ESP_FAIL);
    }
	
    while (true)
    {
		// blink led to check program status
        ESP_ERROR_CHECK (gpio_set_level (GPIO_NUM_8, 1));
        vTaskDelay (pdMS_TO_TICKS(1000));
        ESP_ERROR_CHECK (gpio_set_level (GPIO_NUM_8, 0));
        vTaskDelay (pdMS_TO_TICKS(1000));
    }

}