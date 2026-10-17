#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <network.h>
#include <shell.h>

void app_main() 
{

if (xTaskCreate (shellSrv, 
                "ShellTask", 
                1024, 
                NULL, 
                tskIDLE_PRIORITY + 1, 
                NULL) != pdPASS)
{
    ESP_ERROR_CHECK (ESP_FAIL);
}

}